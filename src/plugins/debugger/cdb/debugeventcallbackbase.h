/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** No Commercial Usage
**
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
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
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#ifndef DEBUGEVENTCALLBACKBASE_H
#define DEBUGEVENTCALLBACKBASE_H

#include "cdbcom.h"

#include <QtCore/QSharedPointer>

namespace CdbCore {

class CoreEngine;

// Base class for event callbacks that takes care
// Active X magic. Provides base implementations with
// the exception of GetInterestMask(). The base class
// needs to do some book-keeping on the modules loaded to
// be able to detect the startup/completed attach of a
// debuggee (see CoreEngine::modulesLoaded()).
// So, the interest mask must be at least baseInterestMask()
// and handleModuleLoad/Unload must be called from derived
// classes when overwriting the handlers.
class DebugEventCallbackBase : public IDebugEventCallbacksWide
{
protected:
    DebugEventCallbackBase();
public:
    virtual ~DebugEventCallbackBase();
    // IUnknown.
    STDMETHOD(QueryInterface)(
            THIS_
            IN REFIID InterfaceId,
            OUT PVOID* Interface
            );
    STDMETHOD_(ULONG, AddRef)(
            THIS
            );
    STDMETHOD_(ULONG, Release)(
            THIS
            );

    // IDebugEventCallbacks.

    STDMETHOD(Breakpoint)(
            THIS_
            __in PDEBUG_BREAKPOINT2 Bp
            );

    STDMETHOD(Exception)(
            THIS_
            __in PEXCEPTION_RECORD64 Exception,
            __in ULONG FirstChance
            );

    STDMETHOD(CreateThread)(
            THIS_
            __in ULONG64 Handle,
            __in ULONG64 DataOffset,
            __in ULONG64 StartOffset
            );
    STDMETHOD(ExitThread)(
            THIS_
            __in ULONG ExitCode
            );

    STDMETHOD(CreateProcess)(
            THIS_
            __in ULONG64 ImageFileHandle,
            __in ULONG64 Handle,
            __in ULONG64 BaseOffset,
            __in ULONG ModuleSize,
            __in_opt PCWSTR ModuleName,
            __in_opt PCWSTR ImageName,
            __in ULONG CheckSum,
            __in ULONG TimeDateStamp,
            __in ULONG64 InitialThreadHandle,
            __in ULONG64 ThreadDataOffset,
            __in ULONG64 StartOffset
            );

    STDMETHOD(ExitProcess)(
            THIS_
            __in ULONG ExitCode
            );

    // Call handleModuleLoad() when reimplementing this
    STDMETHOD(LoadModule)(
            THIS_
            __in ULONG64 ImageFileHandle,
            __in ULONG64 BaseOffset,
            __in ULONG ModuleSize,
            __in_opt PCWSTR ModuleName,
            __in_opt PCWSTR ImageName,
            __in ULONG CheckSum,
            __in ULONG TimeDateStamp
            );

    // Call handleModuleUnload() when reimplementing this
    STDMETHOD(UnloadModule)(
            THIS_
            __in_opt PCWSTR ImageBaseName,
            __in ULONG64 BaseOffset
            );

    STDMETHOD(SystemError)(
            THIS_
            __in ULONG Error,
            __in ULONG Level
            );

    STDMETHOD(SessionStatus)(
            THIS_
            __in ULONG Status
            );

    STDMETHOD(ChangeDebuggeeState)(
            THIS_
            __in ULONG Flags,
            __in ULONG64 Argument
            );

    STDMETHOD(ChangeEngineState)(
            THIS_
            __in ULONG Flags,
            __in ULONG64 Argument
            );

    STDMETHOD(ChangeSymbolState)(
            THIS_
            __in ULONG Flags,
            __in ULONG64 Argument
            );


    static IDebugEventCallbacksWide *getEventCallback(CIDebugClient *clnt);

    unsigned moduleCount() const;
    void setModuleCount(unsigned m);

protected:
    void handleModuleLoad();
    void handleModuleUnload();
    ULONG baseInterestMask() const;

private:
    unsigned m_moduleCount;
};

// Utility class to temporarily redirect events to another handler
// as long as in scope
class EventCallbackRedirector {
    Q_DISABLE_COPY(EventCallbackRedirector)
public:
    typedef QSharedPointer<DebugEventCallbackBase> DebugEventCallbackBasePtr;

    explicit EventCallbackRedirector(CoreEngine *engine, const DebugEventCallbackBasePtr &cb);
    ~EventCallbackRedirector();

private:
    CoreEngine *m_engine;
    const DebugEventCallbackBasePtr m_oldCallback;
};

} // namespace CdbCore

#endif // DEBUGEVENTCALLBACKBASE_H