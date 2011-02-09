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

#include "debuggeragents.h"

#include "debuggerengine.h"
#include "debuggerplugin.h"
#include "debuggerstringutils.h"
#include "stackframe.h"

#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/mimedatabase.h>
#include <coreplugin/icore.h>

#include <texteditor/basetexteditor.h>
#include <texteditor/plaintexteditor.h>
#include <texteditor/basetextmark.h>
#include <texteditor/itexteditor.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/basetextdocument.h>

#include <utils/qtcassert.h>

#include <QtCore/QDebug>
#include <QtCore/QMetaObject>

#include <QtGui/QMessageBox>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QTextCursor>

#include <limits.h>

using namespace Core;

namespace Debugger {
namespace Internal {

///////////////////////////////////////////////////////////////////////
//
// MemoryViewAgent
//
///////////////////////////////////////////////////////////////////////

/*!
    \class MemoryViewAgent

    Objects form this class are created in response to user actions in
    the Gui for showing raw memory from the inferior. After creation
    it handles communication between the engine and the bineditor.
*/

namespace { const int DataRange = 1024 * 1024; }

MemoryViewAgent::MemoryViewAgent(Debugger::DebuggerEngine *engine, quint64 addr)
    : QObject(engine), m_engine(engine)
{
    QTC_ASSERT(engine, /**/);
    createBinEditor(addr);
}

MemoryViewAgent::MemoryViewAgent(Debugger::DebuggerEngine *engine, const QString &addr)
    : QObject(engine), m_engine(engine)
{
    QTC_ASSERT(engine, /**/);
    bool ok = true;
    createBinEditor(addr.toULongLong(&ok, 0));
    //qDebug() <<  " ADDRESS: " << addr <<  addr.toUInt(&ok, 0);
}

MemoryViewAgent::~MemoryViewAgent()
{
    EditorManager *editorManager = EditorManager::instance();
    QList<IEditor *> editors;
    foreach (QPointer<IEditor> editor, m_editors)
        if (editor)
            editors.append(editor.data());
    editorManager->closeEditors(editors);
}

void MemoryViewAgent::createBinEditor(quint64 addr)
{
    EditorManager *editorManager = EditorManager::instance();
    QString titlePattern = tr("Memory $");
    IEditor *editor = editorManager->openEditorWithContents(
        Core::Constants::K_DEFAULT_BINARY_EDITOR_ID,
        &titlePattern);
    if (editor) {
        connect(editor->widget(),
            SIGNAL(lazyDataRequested(Core::IEditor *, quint64,bool)),
            SLOT(fetchLazyData(Core::IEditor *, quint64,bool)));
        connect(editor->widget(),
            SIGNAL(newWindowRequested(quint64)),
            SLOT(createBinEditor(quint64)));
        connect(editor->widget(),
            SIGNAL(newRangeRequested(Core::IEditor *, quint64)),
            SLOT(provideNewRange(Core::IEditor*,quint64)));
        connect(editor->widget(),
            SIGNAL(startOfFileRequested(Core::IEditor *)),
            SLOT(handleStartOfFileRequested(Core::IEditor*)));
        connect(editor->widget(),
            SIGNAL(endOfFileRequested(Core::IEditor *)),
            SLOT(handleEndOfFileRequested(Core::IEditor*)));
        m_editors << editor;
        editorManager->activateEditor(editor);
        QMetaObject::invokeMethod(editor->widget(), "setNewWindowRequestAllowed");
        QMetaObject::invokeMethod(editor->widget(), "setLazyData",
            Q_ARG(quint64, addr), Q_ARG(int, DataRange), Q_ARG(int, BinBlockSize));
    } else {
        DebuggerEngine::showMessageBox(QMessageBox::Warning,
            tr("No memory viewer available"),
            tr("The memory contents cannot be shown as no viewer plugin "
               "for binary data has been loaded."));
        deleteLater();
    }
}

void MemoryViewAgent::fetchLazyData(IEditor *editor, quint64 block, bool sync)
{
    Q_UNUSED(sync); // FIXME: needed support for incremental searching
    m_engine->fetchMemory(this, editor, BinBlockSize * block, BinBlockSize);
}

void MemoryViewAgent::addLazyData(QObject *editorToken, quint64 addr,
                                  const QByteArray &ba)
{
    IEditor *editor = qobject_cast<IEditor *>(editorToken);
    if (editor && editor->widget()) {
        Core::EditorManager::instance()->activateEditor(editor);
        QMetaObject::invokeMethod(editor->widget(), "addLazyData",
            Q_ARG(quint64, addr / BinBlockSize), Q_ARG(QByteArray, ba));
    }
}

void MemoryViewAgent::provideNewRange(IEditor *editor, quint64 address)
{
    QMetaObject::invokeMethod(editor->widget(), "setLazyData",
        Q_ARG(quint64, address), Q_ARG(int, DataRange),
        Q_ARG(int, BinBlockSize));
}

// Since we are not dealing with files, we take these signals to mean
// "move to start/end of range". This seems to make more sense than
// jumping to the start or end of the address space, respectively.
void MemoryViewAgent::handleStartOfFileRequested(IEditor *editor)
{
    QMetaObject::invokeMethod(editor->widget(),
        "setCursorPosition", Q_ARG(int, 0));
}

void MemoryViewAgent::handleEndOfFileRequested(IEditor *editor)
{
    QMetaObject::invokeMethod(editor->widget(),
        "setCursorPosition", Q_ARG(int, DataRange - 1));
}



///////////////////////////////////////////////////////////////////////
//
// DisassemblerViewAgent
//
///////////////////////////////////////////////////////////////////////

// Used for the disassembler view
class LocationMark2 : public TextEditor::ITextMark
{
public:
    LocationMark2() {}

    QIcon icon() const { return DebuggerPlugin::instance()->locationMarkIcon(); }
    void updateLineNumber(int /*lineNumber*/) {}
    void updateBlock(const QTextBlock & /*block*/) {}
    void removedFromEditor() {}
    void documentClosing() {}
};

struct DisassemblerViewAgentPrivate
{
    DisassemblerViewAgentPrivate();
    void configureMimeType();

    QPointer<TextEditor::ITextEditor> editor;
    StackFrame frame;
    bool tryMixed;
    QPointer<DebuggerEngine> engine;
    LocationMark2 *locationMark;
    QHash<QString, QString> cache;
    QString mimeType;
};

DisassemblerViewAgentPrivate::DisassemblerViewAgentPrivate() :
    editor(0),
    tryMixed(true),
    locationMark(new LocationMark2),
    mimeType(_("text/x-qtcreator-generic-asm"))
{
}

/*!
    \class DisassemblerViewAgent

     Objects from this class are created in response to user actions in
     the Gui for showing disassembled memory from the inferior. After creation
     it handles communication between the engine and the editor.
*/

DisassemblerViewAgent::DisassemblerViewAgent(DebuggerEngine *engine)
    : QObject(0), d(new DisassemblerViewAgentPrivate)
{
    d->engine = engine;
}

DisassemblerViewAgent::~DisassemblerViewAgent()
{
    EditorManager *editorManager = EditorManager::instance();
    if (d->editor)
        editorManager->closeEditors(QList<IEditor *>() << d->editor);
    d->editor = 0;
    delete d->locationMark;
    d->locationMark = 0;
    delete d;
    d = 0;
}

void DisassemblerViewAgent::cleanup()
{
    d->cache.clear();
}

void DisassemblerViewAgent::resetLocation()
{
    if (d->editor)
        d->editor->markableInterface()->removeMark(d->locationMark);
}

QString frameKey(const StackFrame &frame)
{
    return _("%1:%2:%3").arg(frame.function).arg(frame.file).arg(frame.from);
}

const StackFrame &DisassemblerViewAgent::frame() const
{
    return d->frame;
}

bool DisassemblerViewAgent::isMixed() const
{
    return d->tryMixed
        && d->frame.line > 0
        && !d->frame.function.isEmpty()
        && d->frame.function != _("??");
}

void DisassemblerViewAgent::setFrame(const StackFrame &frame, bool tryMixed)
{
    d->frame = frame;
    d->tryMixed = tryMixed;
    if (isMixed()) {
        QHash<QString, QString>::ConstIterator it = d->cache.find(frameKey(frame));
        if (it != d->cache.end()) {
            QString msg = _("Use cache disassembler for '%1' in '%2'")
                .arg(frame.function).arg(frame.file);
            d->engine->showMessage(msg);
            setContents(*it);
            return;
        }
    }
    d->engine->fetchDisassembler(this);
}

void DisassemblerViewAgentPrivate::configureMimeType()
{
    QTC_ASSERT(editor, return);

    TextEditor::BaseTextDocument *doc = qobject_cast<TextEditor::BaseTextDocument *>(editor->file());
    QTC_ASSERT(doc, return);
    doc->setMimeType(mimeType);

    TextEditor::PlainTextEditor *pe = qobject_cast<TextEditor::PlainTextEditor *>(editor->widget());
    QTC_ASSERT(pe, return);

    if (const MimeType mtype = ICore::instance()->mimeDatabase()->findByType(mimeType)) {
        pe->configure(mtype);
    } else {
        qWarning("Assembler mimetype '%s' not found.", qPrintable(mimeType));
    }
}

QString DisassemblerViewAgent::mimeType() const
{
    return d->mimeType;
}

void DisassemblerViewAgent::setMimeType(const QString &mt)
{
    if (mt == d->mimeType)
        return;
    d->mimeType = mt;
    if (d->editor)
       d->configureMimeType();
}

// Return a pair of <linenumber [1..n], character position> of an address
// in assembly code, assuming lines start with a sane hex address.
static QPair<int, int> lineNumberOfAddress(const QString &disassembly, quint64 address)
{
    if (disassembly.isEmpty())
        return QPair<int, int>(-1, -1);

    int pos = 0;
    const QChar newLine = QLatin1Char('\n');

    const int size = disassembly.size();
    for (int lineNumber = 1; pos < size; lineNumber++) {
        int endOfLinePos = disassembly.indexOf(newLine, pos + 1);
        if (endOfLinePos == -1)
            endOfLinePos = size;
        const QString line = disassembly.mid(pos, endOfLinePos - pos);
        if (DisassemblerViewAgent::addressFromDisassemblyLine(line) == address)
            return QPair<int, int>(lineNumber, pos);
        pos = endOfLinePos + 1;
    }
    return QPair<int, int>(-1, -1);;
}

void DisassemblerViewAgent::setContents(const QString &contents)
{
    QTC_ASSERT(d, return);
    using namespace Core;
    using namespace TextEditor;

    d->cache.insert(frameKey(d->frame), contents);
    QPlainTextEdit *plainTextEdit = 0;
    EditorManager *editorManager = EditorManager::instance();
    if (!d->editor) {
        QString titlePattern = "Disassembler";
        d->editor = qobject_cast<ITextEditor *>(
            editorManager->openEditorWithContents(
                Core::Constants::K_DEFAULT_TEXT_EDITOR_ID,
                &titlePattern));
        QTC_ASSERT(d->editor, return);
        d->editor->setProperty("OpenedByDebugger", true);
        d->editor->setProperty("DisassemblerView", true);
        d->configureMimeType();
    }

    editorManager->activateEditor(d->editor);

    plainTextEdit = qobject_cast<QPlainTextEdit *>(d->editor->widget());
    if (plainTextEdit) {
        plainTextEdit->setPlainText(contents);
        plainTextEdit->setReadOnly(true);
    }

    d->editor->markableInterface()->removeMark(d->locationMark);
    d->editor->setDisplayName(_("Disassembler (%1)").arg(d->frame.function));

    const QPair<int, int> lineNumberPos = lineNumberOfAddress(contents, d->frame.address);
    if (lineNumberPos.first > 0) {
        d->editor->markableInterface()->addMark(d->locationMark, lineNumberPos.first);
        if (plainTextEdit) {
            QTextCursor tc = plainTextEdit->textCursor();
            tc.setPosition(lineNumberPos.second);
            plainTextEdit->setTextCursor(tc);
        }
    }
}

bool DisassemblerViewAgent::contentsCoversAddress(const QString &contents) const
{
    QTC_ASSERT(d, return false);

    return lineNumberOfAddress(contents, d->frame.address).first > 0;
}

quint64 DisassemblerViewAgent::address() const
{
    return d->frame.address;
}

// Return address of an assembly line "0x0dfd  bla"
quint64 DisassemblerViewAgent::addressFromDisassemblyLine(const QString &line)
{
    // Mac gdb has an overflow reporting 64bit addresses causing the instruction
    // to follow the last digit "0x000000013fff4810mov 1,1". Truncate here.
    const int pos = qMin(line.indexOf(QLatin1Char(' ')), 19);
    if (pos < 0)
        return 0;
    QString addressS = line.left(pos);
    if (addressS.startsWith(QLatin1String("0x")))
        addressS.remove(0, 2);
    bool ok;
    const quint64 address = addressS.toULongLong(&ok, 16);
    return ok ? address : quint64(0);
}

} // namespace Internal
} // namespace Debugger