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

#ifndef QMLREFACTORINGCHANGES_H
#define QMLREFACTORINGCHANGES_H

#include <qmljs/qmljsdocument.h>

#include <texteditor/refactoringchanges.h>

namespace QmlJS {
class ModelManagerInterface;
} // namespace QmlJS

namespace QmlJSEditor {

class QmlJSRefactoringChanges;

class QmlJSRefactoringFile: public TextEditor::RefactoringFile
{
public:
    QmlJSRefactoringFile();
    QmlJSRefactoringFile(const QString &fileName, QmlJSRefactoringChanges *refactoringChanges);
    QmlJSRefactoringFile(TextEditor::BaseTextEditor *editor, QmlJS::Document::Ptr document);

    QmlJS::Document::Ptr qmljsDocument() const;

    /*!
        \returns the offset in the document for the start position of the given
                 source location.
     */
    unsigned startOf(const QmlJS::AST::SourceLocation &loc) const;

    bool isCursorOn(QmlJS::AST::UiObjectMember *ast) const;
    bool isCursorOn(QmlJS::AST::UiQualifiedId *ast) const;

private:
    QmlJSRefactoringChanges *refactoringChanges() const;

    mutable QmlJS::Document::Ptr m_qmljsDocument;
};


class QmlJSRefactoringChanges: public TextEditor::RefactoringChanges
{
public:
    QmlJSRefactoringChanges(QmlJS::ModelManagerInterface *modelManager,
                            const QmlJS::Snapshot &snapshot);

    const QmlJS::Snapshot &snapshot() const;

    QmlJSRefactoringFile file(const QString &fileName);

private:
    virtual void indentSelection(const QTextCursor &selection) const;
    virtual void fileChanged(const QString &fileName);

private:
    QmlJS::ModelManagerInterface *m_modelManager;
    QmlJS::Snapshot m_snapshot;
};

} // namespace QmlJSEditor

#endif // QMLREFACTORINGCHANGES_H