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
** Nokia at info@qt.nokia.com.
**
**************************************************************************/

#ifndef DESIGNER_EDITORWIDGET_H
#define DESIGNER_EDITORWIDGET_H

#include "designerconstants.h"

#include <utils/fancymainwindow.h>

QT_BEGIN_NAMESPACE
class QDesignerFormWindowInterface;
QT_END_NAMESPACE

namespace SharedTools {
    class WidgetHost;
}
namespace Core {
    class IEditor;
}
namespace Designer {
class FormWindowEditor;

namespace Internal {
struct EditorData;
class FormEditorStack;
class FormEditorW;

// Design mode main view.
class EditorWidget : public Utils::FancyMainWindow
{
    Q_OBJECT

public:
    explicit EditorWidget(FormEditorW *fe, QWidget *parent = 0);

    QDockWidget* const* designerDockWidgets() const;

    // Form editor stack API
    void add(const EditorData &d);
    void removeFormWindowEditor(Core::IEditor *xmlEditor);
    bool setVisibleEditor(Core::IEditor *xmlEditor);
    SharedTools::WidgetHost *formWindowEditorForXmlEditor(const Core::IEditor *xmlEditor) const;
    SharedTools::WidgetHost *formWindowEditorForFormWindow(const QDesignerFormWindowInterface *fw) const;

    EditorData activeEditor() const;

public slots:
    void resetToDefaultLayout();

private:
    FormEditorStack *m_stack;
    QDockWidget *m_designerDockWidgets[Designer::Constants::DesignerSubWindowCount];
};

} // namespace Internal
} // namespace Designer

#endif // DESIGNER_EDITORWIDGET_H
