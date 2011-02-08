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
#ifndef GLSLHIGHLIGHTER_H
#define GLSLHIGHLIGHTER_H

#include <texteditor/syntaxhighlighter.h>

namespace GLSLEditor {
class GLSLTextEditor;

namespace Internal {

class Highlighter : public TextEditor::SyntaxHighlighter
{
    Q_OBJECT

public:
    enum Formats {
        GLSLNumberFormat,
        GLSLStringFormat,
        GLSLTypeFormat,
        GLSLKeywordFormat,
        GLSLOperatorFormat,
        GLSLPreprocessorFormat,
        GLSLLabelFormat,
        GLSLCommentFormat,
        GLSLDoxygenCommentFormat,
        GLSLDoxygenTagFormat,
        GLSLVisualWhitespace,
        GLSLReservedKeyword,
        NumGLSLFormats
    };

    explicit Highlighter(GLSLTextEditor *editor, QTextDocument *parent);
    virtual ~Highlighter();

    void setFormats(const QVector<QTextCharFormat> &formats);

protected:
    void highlightBlock(const QString &text);
    void highlightLine(const QString &text, int position, int length, const QTextCharFormat &format);
    bool isPPKeyword(const QStringRef &text) const;

private:
    QTextCharFormat m_formats[NumGLSLFormats];
    GLSLTextEditor *m_editor;
};

} // namespace Internal
} // namespace GLSLEditor

#endif // GLSLHIGHLIGHTER_H