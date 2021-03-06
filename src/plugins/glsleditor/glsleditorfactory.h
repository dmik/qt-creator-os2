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

#ifndef GLSLEDITORFACTORY_H
#define GLSLEDITORFACTORY_H

#include <coreplugin/editormanager/ieditorfactory.h>

#include <QtCore/QStringList>

namespace TextEditor {
class TextEditorActionHandler;
}

namespace GLSLEditor {
namespace Internal {

class GLSLEditorActionHandler;

class GLSLEditorFactory : public Core::IEditorFactory
{
    Q_OBJECT

public:
    GLSLEditorFactory(QObject *parent);
    ~GLSLEditorFactory();

    virtual QStringList mimeTypes() const;
    // IEditorFactory
    QString id() const;
    QString displayName() const;
    Core::IFile *open(const QString &fileName);
    Core::IEditor *createEditor(QWidget *parent);

private slots:
    void updateEditorInfoBar(Core::IEditor *editor);

private:
    QStringList m_mimeTypes;
};

} // namespace Internal
} // namespace GLSLEditor

#endif // GLSLEDITORFACTORY_H
