/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef DEBUGGER_SESSIONENGINE_H
#define DEBUGGER_SESSIONENGINE_H

#include "debuggerengine.h"

namespace Debugger {
namespace Internal {

// This class contains data serving as a template for debugger engines
// started during a session.

class SessionEngine : public DebuggerEngine
{
    Q_OBJECT

public:
    SessionEngine();

    virtual void setupEngine() {}
    virtual void setupInferior() {}
    virtual void runEngine() {}
    virtual void shutdownEngine() {}
    virtual void shutdownInferior() {}
    virtual void executeDebuggerCommand(const QString &command);
    virtual unsigned debuggerCapabilities() const;

    virtual bool isSessionEngine() const { return true; }

    void loadSessionData();
    void saveSessionData();
};


} // namespace Internal
} // namespace Debugger

#endif // DEBUGGER_SESSIONENGINE_H