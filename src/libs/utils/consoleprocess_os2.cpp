/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (info@qt.nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "consoleprocess.h"

#include "environment.h"
#include "qtcprocess.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QThread>
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QLocalServer>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define INCL_BASE
#include <os2.h>

//using namespace Utils;

namespace Utils {

struct ConsoleProcessPrivate {
    ConsoleProcessPrivate();
    void killStub();

    ConsoleProcess::Mode m_mode;
    bool m_bRunning;
    qint64 m_stubPid;
    qint64 m_appPid;
    int m_appCode;
    QString m_executable;
    QProcess::ExitStatus m_appStatus;
    QLocalServer m_stubServer;
    QLocalSocket *m_stubSocket;
};

ConsoleProcessPrivate::ConsoleProcessPrivate() :
    m_mode(ConsoleProcess::Run),
    m_bRunning(false),
    m_stubPid(0),
    m_appPid(0),
    m_appCode(-1),
    m_appStatus(QProcess::NormalExit),
    m_stubServer(0),
    m_stubSocket(0)
{
}

void ConsoleProcessPrivate::killStub()
{
    RESULTCODES retCodes;
    PID pid = (PID)m_stubPid;

    DosKillProcess(DKP_PROCESSTREE, pid);
    DosWaitChild(DCWA_PROCESS, DCWW_WAIT, &retCodes, &pid, pid);

    m_bRunning = false;
    m_stubPid = 0;
}



class StubWatcher : public QThread
{
    HQUEUE      m_queue;

public:
    StubWatcher(HQUEUE queue, QObject *parent = 0)
      : QThread(parent), m_queue(queue) {};

    virtual void run ()
    {
        BYTE prio;
        APIRET rc;
        ULONG resultSize;
        PULONG resultData;
        REQUESTDATA queueReq;

        // We are not really interrested in the result data. We
        // just need a way to tell when the stub process has ended.
        rc = DosReadQueue(m_queue, &queueReq,
                          &resultSize, (PPVOID)&resultData,
                          0, DCWW_WAIT, &prio, NULLHANDLE);
        if (rc == NO_ERROR)
            DosFreeMem(resultData);

        DosCloseQueue(m_queue);
    };
};




ConsoleProcess::ConsoleProcess(QObject *parent)  :
    QObject(parent), d(new ConsoleProcessPrivate)
{
    connect(&d->m_stubServer, SIGNAL(newConnection()), SLOT(stubConnectionAvailable()));
}

ConsoleProcess::~ConsoleProcess()
{
    stop();
}

void ConsoleProcess::setMode(Mode m)
{
    d->m_mode = m;
}

ConsoleProcess::Mode ConsoleProcess::mode() const
{
    return d->m_mode;
}

qint64 ConsoleProcess::applicationPID() const
{
    return d->m_appPid;
}

int ConsoleProcess::exitCode() const
{
    return d->m_appCode;
} // This will be the signal number if exitStatus == CrashExit

QProcess::ExitStatus ConsoleProcess::exitStatus() const
{
    return d->m_appStatus;
}

bool ConsoleProcess::start(const QString &program, const QString &args)
{
    APIRET rc;
    STARTDATA sd;
    HQUEUE hTermQ;
    ULONG sid, pid;
    CHAR termQName[64];

    if (isRunning())
        return false;

    QStringList	argList;
    QtcProcess::SplitError splitErr;
    argList = QtcProcess::prepareArgs(args, &splitErr, &m_environment, &m_workingDir);
    if (splitErr != QtcProcess::SplitOk) {
        emit processMessage(tr("Quoting error in command."), true);
        return false;
    }

    const QString err = stubServerListen();
    if (!err.isEmpty()) {
        emit processMessage(msgCommChannelFailed(err), true);
        return false;
    }

    QString workDir = m_workingDir;
    if (!workDir.isEmpty() && !workDir.endsWith('/'))
        workDir.append('/');

    QString stubProgram = QDir::toNativeSeparators(
        QCoreApplication::applicationDirPath() +
        QLatin1String("/qtcreator_process_stub.exe"));

    QStringList stubArgs;
    stubArgs << modeOption(d->m_mode)
             << d->m_stubServer.fullServerName()
             << msgPromptToClose()
             << QDir::toNativeSeparators(workDir)
             << QDir::toNativeSeparators(program)
             << createOs2ArgList(argList);

    QByteArray os2Name = stubProgram.toLocal8Bit();
    QByteArray os2Args = createOs2ArgList(stubArgs);
    QByteArray os2Env  = createOs2Environment(
                                fixOs2Environment(m_environment.toStringList()));

    // !!! DosStartSession() requires some params in low memory !!!
    memset(&sd, 0, sizeof(sd));
    sd.Length = sizeof(sd);
    sd.Related = SSF_RELATED_CHILD;
    sd.FgBg = SSF_FGBG_FORE;
    sd.TraceOpt = SSF_TRACEOPT_NONE;
    sd.PgmName  = (PSZ)alloca(os2Name.length() + 1);
    sd.PgmInputs = (PSZ)alloca(os2Args.length() + 1);
    sd.Environment = (PBYTE)alloca(os2Env.size());
    sd.TermQ = (PBYTE)termQName;
    sd.InheritOpt = SSF_INHERTOPT_SHELL;
    sd.SessionType = SSF_TYPE_WINDOWABLEVIO;
    sd.PgmControl = SSF_CONTROL_VISIBLE;

    strcpy((char *)sd.PgmName, os2Name);
    strcpy((char *)sd.PgmInputs, os2Args);
    memcpy(sd.Environment, os2Env, os2Env.size());

    sprintf(termQName, "\\queues\\qtcreator.%u", (unsigned)qApp->applicationPid());
    rc = DosCreateQueue(&hTermQ, QUE_FIFO, (PSZ)termQName);
    if (rc == NO_ERROR) {
        rc = DosStartSession(&sd, &sid, &pid);
        if (rc == NO_ERROR) {
            d->m_stubPid = pid;
            d->m_bRunning = true;
            d->m_executable = program;
            emit wrapperStarted();

            StubWatcher *watcher = new StubWatcher(hTermQ, this);
            connect(watcher, SIGNAL(finished()),
                    SLOT(stubExited()), Qt::QueuedConnection);
            watcher->start();

            return true;
        }

        DosCloseQueue(hTermQ);
    }

    stubServerShutdown();
    emit processMessage(tr("Cannot start console process '%1'.").arg(stubProgram), true);
    return false;
}

void ConsoleProcess::stop()
{
    if (!isRunning())
        return;

    stubServerShutdown();
    d->m_appPid = 0;

    d->killStub();
}

bool ConsoleProcess::isRunning() const
{
    return d->m_bRunning;
}

QString ConsoleProcess::stubServerListen()
{
    QString stubServer = QString("qtcreator.%1").arg(qApp->applicationPid());

    if (!d->m_stubServer.listen(stubServer)) {
        return tr("Cannot create socket '%1': %2").arg(stubServer, d->m_stubServer.errorString());
    }
    return QString();
}

void ConsoleProcess::stubServerShutdown()
{
    delete d->m_stubSocket;
    d->m_stubSocket = 0;
    if (d->m_stubServer.isListening()) {
        d->m_stubServer.close();
    }
}

void ConsoleProcess::stubConnectionAvailable()
{
    d->m_stubSocket = d->m_stubServer.nextPendingConnection();
    connect(d->m_stubSocket, SIGNAL(readyRead()), SLOT(readStubOutput()));
}

static QString errorMsg(int code)
{
    return QString::fromLocal8Bit(strerror(code));
}

void ConsoleProcess::readStubOutput()
{
    while (d->m_stubSocket->canReadLine()) {
        QByteArray out = d->m_stubSocket->readLine();
        out.chop(1); // \n
        if (out.startsWith("err:chdir ")) {
            emit processMessage(msgCannotChangeToWorkDir(workingDirectory(), errorMsg(out.mid(10).toInt())), true);
        } else if (out.startsWith("err:exec ")) {
            emit processMessage(msgCannotExecute(d->m_executable, errorMsg(out.mid(9).toInt())), true);
        } else if (out.startsWith("pid ")) {
            d->m_appPid = out.mid(4).toInt();
            emit processStarted();
        } else if (out.startsWith("exit ")) {
            d->m_appStatus = QProcess::NormalExit;
            d->m_appCode = out.mid(5).toInt();
            d->m_appPid = 0;
            emit processStopped();
        } else if (out.startsWith("crash ")) {
            d->m_appStatus = QProcess::CrashExit;
            d->m_appCode = out.mid(6).toInt();
            d->m_appPid = 0;
            emit processStopped();
        } else {
            emit processMessage(msgUnexpectedOutput( out ), true);
            d->killStub();
            break;
        }
    }
}

void ConsoleProcess::stubExited()
{
    // The stub exit might get noticed before we read the error status.
    if (d->m_stubSocket && d->m_stubSocket->state() == QLocalSocket::ConnectedState)
        d->m_stubSocket->waitForDisconnected();
    stubServerShutdown();

    d->m_bRunning = false;
    d->m_stubPid = 0;

    if (d->m_appPid) {
        d->m_appStatus = QProcess::CrashExit;
        d->m_appCode = -1;
        d->m_appPid = 0;
        emit processStopped(); // Maybe it actually did not, but keep state consistent
    }
    emit wrapperStopped();
}

} // namespace Utils
