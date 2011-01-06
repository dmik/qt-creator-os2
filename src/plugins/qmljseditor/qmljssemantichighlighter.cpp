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

#include "qmljssemantichighlighter.h"

#include <qmljs/qmljsmodelmanagerinterface.h>
#include <qmljs/qmljsdocument.h>
#include <qmljs/qmljscheck.h>
#include <qmljs/qmljsinterpreter.h>
#include <qmljs/qmljslink.h>

namespace QmlJSEditor {
namespace Internal {

SemanticHighlighter::SemanticHighlighter(QObject *parent)
        : QThread(parent),
          m_done(false),
          m_modelManager(0)
{
}

SemanticHighlighter::~SemanticHighlighter()
{
}

void SemanticHighlighter::abort()
{
    QMutexLocker locker(&m_mutex);
    m_done = true;
    m_condition.wakeOne();
}

void SemanticHighlighter::rehighlight(const SemanticHighlighterSource &source)
{
    QMutexLocker locker(&m_mutex);
    m_source = source;
    m_condition.wakeOne();
}

bool SemanticHighlighter::isOutdated()
{
    QMutexLocker locker(&m_mutex);
    const bool outdated = ! m_source.fileName.isEmpty() || m_done;
    return outdated;
}

void SemanticHighlighter::run()
{
    setPriority(QThread::LowestPriority);

    forever {
        m_mutex.lock();

        while (! (m_done || ! m_source.fileName.isEmpty()))
            m_condition.wait(&m_mutex);

        const bool done = m_done;
        const SemanticHighlighterSource source = m_source;
        m_source.clear();

        m_mutex.unlock();

        if (done)
            break;

        const SemanticInfo info = semanticInfo(source);

        if (! isOutdated()) {
            m_mutex.lock();
            m_lastSemanticInfo = info;
            m_mutex.unlock();

            emit changed(info);
        }
    }
}

SemanticInfo SemanticHighlighter::semanticInfo(const SemanticHighlighterSource &source)
{
    m_mutex.lock();
    const int revision = m_lastSemanticInfo.revision();
    m_mutex.unlock();

    QmlJS::Snapshot snapshot;
    QmlJS::Document::Ptr doc;

    if (! source.force && revision == source.revision) {
        m_mutex.lock();
        snapshot = m_lastSemanticInfo.snapshot;
        doc = m_lastSemanticInfo.document;
        m_mutex.unlock();
    }

    if (! doc) {
        snapshot = source.snapshot;
        doc = snapshot.documentFromSource(source.code, source.fileName);
        doc->setEditorRevision(source.revision);
        doc->parse();
        snapshot.insert(doc);
    }

    SemanticInfo semanticInfo;
    semanticInfo.snapshot = snapshot;
    semanticInfo.document = doc;

    QmlJS::Interpreter::Context *ctx = new QmlJS::Interpreter::Context;
    QmlJS::Link link(ctx, doc, snapshot, QmlJS::ModelManagerInterface::instance()->importPaths());
    semanticInfo.m_context = QSharedPointer<const QmlJS::Interpreter::Context>(ctx);
    semanticInfo.semanticMessages = link.diagnosticMessages();

    QStringList importPaths;
    if (m_modelManager)
        importPaths = m_modelManager->importPaths();
    QmlJS::Check checker(doc, snapshot, ctx);
    semanticInfo.semanticMessages.append(checker());

    return semanticInfo;
}

void SemanticHighlighter::setModelManager(QmlJS::ModelManagerInterface *modelManager)
{
    m_modelManager = modelManager;
}

} // namespace Internal
} // namespace QmlJSEditor