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

#include "itemcreatortool.h"
#include "formeditorscene.h"
#include "formeditorview.h"

#include <metainfo.h>
#include <itemlibrarywidget.h>

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QtDebug>
#include <QClipboard>
#include <variantproperty.h>

namespace QmlDesigner {

ItemCreatorTool::ItemCreatorTool(FormEditorView *editorView)
    : AbstractFormEditorTool(editorView),
    m_rubberbandSelectionManipulator(editorView->scene()->manipulatorLayerItem(), editorView)

{

}


ItemCreatorTool::~ItemCreatorTool()
{
}

void ItemCreatorTool::mousePressEvent(const QList<QGraphicsItem*> &/*itemList*/,
                                    QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_rubberbandSelectionManipulator.begin(event->scenePos());
    else
        view()->deActivateItemCreator();
}

void ItemCreatorTool::mouseMoveEvent(const QList<QGraphicsItem*> &/*itemList*/,
                                   QGraphicsSceneMouseEvent *event)
{
    if (m_rubberbandSelectionManipulator.isActive()) {
        m_rubberbandSelectionManipulator.update(event->scenePos());
    } else {

    }
}

void ItemCreatorTool::hoverMoveEvent(const QList<QGraphicsItem*> &/*itemList*/,
                        QGraphicsSceneMouseEvent * /*event*/)
{
}

void ItemCreatorTool::mouseReleaseEvent(const QList<QGraphicsItem*> &/*itemList*/,
                                      QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_rubberbandSelectionManipulator.isActive()) {
            QPointF mouseMovementVector = m_rubberbandSelectionManipulator.beginPoint() - event->scenePos();
            if (mouseMovementVector.toPoint().manhattanLength() < QApplication::startDragDistance()) {
                m_rubberbandSelectionManipulator.update(event->scenePos());
            } else {
                m_rubberbandSelectionManipulator.update(event->scenePos());
                QRectF rect(m_rubberbandSelectionManipulator.beginPoint(), event->scenePos());
                createAtItem(rect);
                m_rubberbandSelectionManipulator.end();
                view()->deActivateItemCreator();
            }
        }
    } else {
        view()->deActivateItemCreator();
    }
}

void ItemCreatorTool::mouseDoubleClickEvent(const QList<QGraphicsItem*> &/*itemList*/,
                                          QGraphicsSceneMouseEvent * /*event*/)
{

}

void ItemCreatorTool::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
        case Qt::Key_Escape:
            view()->deActivateItemCreator();
            break;
    }
}

void ItemCreatorTool::keyReleaseEvent(QKeyEvent * /*keyEvent*/)
{

}

void ItemCreatorTool::itemsAboutToRemoved(const QList<FormEditorItem*> &/*itemList*/)
{

}

void ItemCreatorTool::clear()
{
    m_rubberbandSelectionManipulator.clear();
}

void ItemCreatorTool::selectedItemsChanged(const QList<FormEditorItem*> &/*itemList*/)
{
}

void ItemCreatorTool::formEditorItemsChanged(const QList<FormEditorItem*> &/*itemList*/)
{
}

void ItemCreatorTool::instancesCompleted(const QList<FormEditorItem*> &/*itemList*/)
{
}

void ItemCreatorTool::instancesParentChanged(const QList<FormEditorItem *> &/*itemList*/)
{
}

void ItemCreatorTool::setItemString(const QString &itemString)
{
    m_itemString = itemString;
}

FormEditorItem* ItemCreatorTool::calculateContainer(const QPointF &point)
{
    QList<QGraphicsItem *> list = scene()->items(point);
    foreach (QGraphicsItem *item, list) {
         FormEditorItem *formEditorItem = FormEditorItem::fromQGraphicsItem(item);
         if (formEditorItem)
             return formEditorItem;
    }
    return 0;
}

void ItemCreatorTool::createAtItem(const QRectF &rect)
{
    QPointF pos = rect.topLeft();

    QmlItemNode parentNode = view()->rootQmlItemNode();
    if (!parentNode.isValid())
        return;

    FormEditorItem *parentItem = calculateContainer(pos);
    if (parentItem) {
        parentNode = parentItem->qmlItemNode();
        pos = parentItem->mapFromScene(pos);
    }

    QStringList list = m_itemString.split(QLatin1Char('^'));
    if (list.count() != 2)
        return;
    if (list.first() == "item") {
        RewriterTransaction transaction = view()->beginRewriterTransaction();
        Q_ASSERT(false); //this code is out of date
        ItemLibraryEntry itemLibraryEntry; // = view()->model()->metaInfo().itemLibraryInfo()->entry(list.at(1));
        QmlItemNode newNode = view()->createQmlItemNode(itemLibraryEntry, pos, parentNode);
        newNode.modelNode().variantProperty("width") = rect.width();
        newNode.modelNode().variantProperty("height") = rect.height();
        QList<QmlItemNode> nodeList;
        nodeList.append(newNode);
        view()->setSelectedQmlItemNodes(nodeList);
    } else if (list.first() == "image") {
        RewriterTransaction transaction = view()->beginRewriterTransaction();
        QmlItemNode newNode = view()->createQmlItemNodeFromImage(list.at(1), pos, parentNode);
        newNode.modelNode().variantProperty("width") = rect.width();
        newNode.modelNode().variantProperty("height") = rect.height();
        QList<QmlItemNode> nodeList;
        nodeList.append(newNode);
        view()->setSelectedQmlItemNodes(nodeList);
    }
}

} //QmlDesigner
