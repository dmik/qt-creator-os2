/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#ifndef DEBUGGER_CDBENGINE_H
#define DEBUGGER_CDBENGINE_H

#include "debuggerengine.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QProcess>
#include <QtCore/QVariant>
#include <QtCore/QMap>
#include <QtCore/QTime>

namespace Debugger {
namespace Cdb {

class DisassemblerViewAgent;
struct CdbBuiltinCommand;
struct CdbExtensionCommand;
struct CdbOptions;

class CdbEngine : public Debugger::DebuggerEngine
{
    Q_OBJECT

public:
    typedef QSharedPointer<CdbOptions> OptionsPtr;

    enum CommandFlags { QuietCommand = 0x1 };
    // Flag bits for a sequence of commands
    enum CommandSequenceFlags {
        CommandListStack = 0x1,
        CommandListThreads = 0x2,
        CommandListRegisters = 0x4,
        CommandListModules = 0x8
    };

    typedef QSharedPointer<CdbBuiltinCommand> CdbBuiltinCommandPtr;
    typedef QSharedPointer<CdbExtensionCommand> CdbExtensionCommandPtr;
    typedef void (CdbEngine::*BuiltinCommandHandler)(const CdbBuiltinCommandPtr &);
    typedef void (CdbEngine::*ExtensionCommandHandler)(const CdbExtensionCommandPtr &);

    explicit CdbEngine(const DebuggerStartParameters &sp, const OptionsPtr &options);
    virtual ~CdbEngine();
    // Factory function that returns 0 if the debug engine library cannot be found.

    virtual void setToolTipExpression(const QPoint &mousePos, TextEditor::ITextEditor *editor, int cursorPos);
    virtual void setupEngine();
    virtual void setupInferior();
    virtual void runEngine();
    virtual void shutdownInferior();
    virtual void shutdownEngine();
    virtual void detachDebugger();
    virtual void updateWatchData(const Debugger::Internal::WatchData &data,
                                 const Debugger::Internal::WatchUpdateFlags & flags = Debugger::Internal::WatchUpdateFlags());
    virtual unsigned debuggerCapabilities() const;
    virtual void setRegisterValue(int regnr, const QString &value);

    virtual void executeStep();
    virtual void executeStepOut();
    virtual void executeNext();
    virtual void executeStepI();
    virtual void executeNextI();

    virtual void continueInferior();
    virtual void interruptInferior();

    virtual void executeRunToLine(const QString &fileName, int lineNumber);
    virtual void executeRunToFunction(const QString &functionName);
    virtual void executeJumpToLine(const QString &fileName, int lineNumber);
    virtual void assignValueInDebugger(const Debugger::Internal::WatchData *w, const QString &expr, const QVariant &value);
    virtual void executeDebuggerCommand(const QString &command);

    virtual void activateFrame(int index);
    virtual void selectThread(int index);

    virtual bool stateAcceptsBreakpointChanges() const;
    virtual bool acceptsBreakpoint(BreakpointId id) const;
    virtual void attemptBreakpointSynchronization();

    virtual void fetchDisassembler(Debugger::Internal::DisassemblerViewAgent *agent);
    virtual void fetchMemory(Debugger::Internal::MemoryViewAgent *, QObject *, quint64 addr, quint64 length);

    virtual void reloadModules();
    virtual void loadSymbols(const QString &moduleName);
    virtual void loadAllSymbols();
    virtual void requestModuleSymbols(const QString &moduleName);

    virtual void reloadRegisters();
    virtual void reloadSourceFiles();
    virtual void reloadFullStack();

    static QString extensionLibraryName(bool is64Bit);

private slots:
    void readyReadStandardOut();
    void readyReadStandardError();
    void processError();
    void processFinished();
    void postCommand(const QByteArray &cmd, unsigned flags);
    void postBuiltinCommand(const QByteArray &cmd,
                            unsigned flags,
                            BuiltinCommandHandler handler,
                            unsigned nextCommandFlag = 0,
                            const QVariant &cookie = QVariant());

    void postExtensionCommand(const QByteArray &cmd,
                              const QByteArray &arguments,
                              unsigned flags,
                              ExtensionCommandHandler handler,
                              unsigned nextCommandFlag = 0,
                              const QVariant &cookie = QVariant());

    void postCommandSequence(unsigned mask);
    void operateByInstructionTriggered(bool);

private:
    enum SpecialStopMode { NoSpecialStop, SpecialStopSynchronizeBreakpoints };

    void handleExtensionMessage(char t, int token, const QByteArray &what, const QByteArray &message);
    bool doSetupEngine(QString *errorMessage);
    void handleSessionAccessible(unsigned long cdbExState);
    void handleSessionInaccessible(unsigned long cdbExState);
    void handleSessionIdle(const QByteArray &message);
    void doInterruptInferior(SpecialStopMode sm);
    void doContinueInferior();
    inline void parseOutputLine(QByteArray line);
    inline bool isCdbProcessRunning() const { return m_process.state() != QProcess::NotRunning; }
    bool canInterruptInferior() const;
    void syncOperateByInstruction(bool operateByInstruction);

    // Builtin commands
    void dummyHandler(const CdbBuiltinCommandPtr &);
    void handleStackTrace(const CdbExtensionCommandPtr &);
    void handleRegisters(const CdbBuiltinCommandPtr &);
    void handleDisassembler(const CdbBuiltinCommandPtr &);
    void handleJumpToLineAddressResolution(const CdbBuiltinCommandPtr &);
    // Extension commands
    void handleThreads(const CdbExtensionCommandPtr &);
    void handlePid(const CdbExtensionCommandPtr &reply);
    void handleLocals(const CdbExtensionCommandPtr &reply);
    void handleExpandLocals(const CdbExtensionCommandPtr &reply);
    void handleRegisters(const CdbExtensionCommandPtr &reply);
    void handleModules(const CdbExtensionCommandPtr &reply);
    void handleMemory(const CdbExtensionCommandPtr &);

    QString normalizeFileName(const QString &f);
    void updateLocalVariable(const QByteArray &iname);
    int elapsedLogTime() const;

    const QByteArray m_creatorExtPrefix;
    const QByteArray m_tokenPrefix;
    const OptionsPtr m_options;

    QProcess m_process;
    QByteArray m_outputBuffer;
    unsigned long m_inferiorPid;
    // Debugger accessible (expecting commands)
    bool m_accessible;
    SpecialStopMode m_specialStopMode;
    int m_nextCommandToken;
    int m_nextBreakpointNumber;
    QList<CdbBuiltinCommandPtr> m_builtinCommandQueue;
    int m_currentBuiltinCommandIndex; // Current command whose output is recorded.
    QList<CdbExtensionCommandPtr> m_extensionCommandQueue;
    QMap<QString, QString> m_normalizedFileCache;
    const QByteArray m_extensionCommandPrefixBA; // Library name used as prefix
    bool m_operateByInstructionPending; // Creator operate by instruction action changed.
    bool m_operateByInstruction;
    bool m_notifyEngineShutdownOnTermination;
    bool m_hasDebuggee;
    QTime m_logTime;
    mutable int m_elapsedLogTime;
};

} // namespace Cdb
} // namespace Debugger

#endif // DEBUGGER_CDBENGINE_H