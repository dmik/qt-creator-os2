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

#include "abstractprocess.h"

#define INCL_DOSMISC
#define INCL_DOSERRORS
#include <os2.h>

namespace Utils {

QStringList AbstractProcess::fixOs2Environment(const QStringList &env)
{
    QStringList envStrings = env;
    static struct { ULONG type; const char *name; } vars[] = {
        { 0,             "PATH" },
        { BEGIN_LIBPATH, "QTC_BEGINLIBPATH" },
        { END_LIBPATH,   "QTC_ENDLIBPATH" },
        { LIBPATHSTRICT, "QTC_LIBPATHSTRICT" },
    };

    // add PATH, BEGINLIBPATH, ENDLIBPATH and LIBPATHSTRICT if necessary (for DLL loading)
    // ATTENTION: LIBPATH entries aren't regular environment variables. We prepend a 'QTC_' to
    // avoid confusion. These variables will be filtered out in process_stub_os2.c.
    for (int i = 0; i < (int)(sizeof(vars)/sizeof(vars[0])); i++) {
        QLatin1String   name(vars[i].name);
        if (envStrings.filter(QRegExp(QString("^%1=").arg(name),Qt::CaseInsensitive)).isEmpty()) {
            QByteArray val = qgetenv(name.latin1());

            if (val.isEmpty() && vars[i].type != 0) {
                char extLibPath[1024];
                if (DosQueryExtLIBPATH((PSZ)extLibPath, vars[i].type) == NO_ERROR)
                    val = QByteArray(extLibPath);
            }

            if (!val.isEmpty())
                envStrings.prepend(QString("%1=%2").arg(name).arg(QString::fromLocal8Bit(val)));
        }
    }

    return envStrings;
}

QByteArray AbstractProcess::createOs2ArgList(const QStringList &args)
{
    const QChar doubleQuote = QLatin1Char('"');
    const QChar blank = QLatin1Char(' ');
    const QChar backSlash = QLatin1Char('\\');

    QString argList;
    for (int i = 0; i < args.size(); ++i) {
        QString tmp = args.at(i);
        // in the case of \" already being in the string the \ must also be escaped
        tmp.replace(QLatin1String("\\\""), QLatin1String("\\\\\""));
        // escape a single " because the arguments will be parsed
        tmp.replace(QString(doubleQuote), QLatin1String("\\\""));
        if (tmp.isEmpty() || tmp.contains(blank) || tmp.contains('\t')) {
            // The argument must not end with a \ since this would be interpreted
            // as escaping the quote -- rather put the \ behind the quote: e.g.
            // rather use "foo"\ than "foo\"
            QString endQuote(doubleQuote);
            int i = tmp.length();
            while (i > 0 && tmp.at(i - 1) == backSlash) {
                --i;
                endQuote += backSlash;
            }
            argList += QLatin1String(" \"");
            argList += tmp.left(i);
            argList += endQuote;
        } else {
            argList += blank;
            argList += tmp;
        }
    }
    return argList.toLocal8Bit().mid(1);
}

QByteArray AbstractProcess::createOs2Environment(const QStringList &env)
{
    QByteArray envlist;
    int pos = 0;
    foreach (const QString &tmp, env) {
        const uint tmpSize = tmp.length() + 1;
        envlist.resize(envlist.size() + tmpSize);
        memcpy(envlist.data() + pos, tmp.toLocal8Bit(), tmpSize);
        pos += tmpSize;
    }
    envlist.resize(envlist.size() + 1);
    envlist[pos++] = 0;
    return envlist;
}

} //namespace Utils
