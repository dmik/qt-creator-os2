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

#ifndef DEBUGGER_BREAKPOINTMARKER_H
#define DEBUGGER_BREAKPOINTMARKER_H

#include <QtCore/QString>
#include <QtGui/QIcon>

#include "breakpoint.h"
#include "breakhandler.h"
#include <texteditor/basetextmark.h>

namespace Debugger {
namespace Internal {

// The red blob on the left side in the cpp editor.
class BreakpointMarker : public TextEditor::BaseTextMark
{
    Q_OBJECT
public:
    BreakpointMarker(BreakpointId id, const QString &fileName, int lineNumber);
    ~BreakpointMarker();
    QIcon icon() const;
    void updateBlock(const QTextBlock &);
    void removedFromEditor();
    void updateLineNumber(int lineNumber);
private:
    BreakpointId m_id;
    friend class BreakHandler;
};

} // namespace Internal
} // namespace Debugger

#endif