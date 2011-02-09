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

#ifndef SNIPPETSCOMPLETION_H
#define SNIPPETSCOMPLETION_H

#include <texteditor/icompletioncollector.h>

#include <QtCore/QDir>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtGui/QIcon>

namespace TextEditor {
class ITextEditable;
class ITextEditor;
}

namespace Snippets {
namespace Internal {

class SnippetsWindow;
class SnippetSpec;

class SnippetsCompletion : public TextEditor::ICompletionCollector
{
    Q_OBJECT
public:
    SnippetsCompletion(QObject *parent);
    ~SnippetsCompletion();

    // ICompletionCollector
    bool triggersCompletion(TextEditor::ITextEditable *editor);
    int startCompletion(TextEditor::ITextEditable *editor);
    void completions(QList<TextEditor::CompletionItem *> *completions);

    QString text(TextEditor::CompletionItem *item) const;
    QString details(TextEditor::CompletionItem *item) const;
    QIcon icon(TextEditor::CompletionItem *item) const;

    void complete(const TextEditor::CompletionItem &item);
    bool partiallyComplete(const QList<TextEditor::CompletionItem> &);
    void cleanup();

    void completions(QList<TextEditor::CompletionItem>*);

private slots:
    void updateCompletions();

private:
    static int findStartOfName(const TextEditor::ITextEditor *editor);

    TextEditor::ITextEditable *m_editor;
    int m_startPosition;  // Position of the cursor from which completion started

    SnippetsWindow *m_snippetsWindow;

    QMultiMap<QString, TextEditor::CompletionItem *> m_autoCompletions;

    static const QIcon m_fileIcon;
};

} // namespace Internal
} // namespace Snippets

#endif // SNIPPETSCOMPLETION_H
