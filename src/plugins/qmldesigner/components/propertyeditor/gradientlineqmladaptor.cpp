#include "gradientlineqmladaptor.h"

#include <qdeclarative.h>
#include <QMessageBox>

#include <nodeproperty.h>
#include <nodelistproperty.h>
#include <nodemetainfo.h>
#include <abstractview.h>
#include <rewritingexception.h>
#include <variantproperty.h>

namespace QmlDesigner {

GradientLineQmlAdaptor::GradientLineQmlAdaptor(QWidget *parent) :
    QmlEditorWidgets::GradientLine(parent)
{
    setActive(false);
    connect(this, SIGNAL(gradientChanged()), this, SLOT(writeGradient()));
}

void GradientLineQmlAdaptor::setItemNode(const QVariant &itemNode)
{

    if (!itemNode.value<ModelNode>().isValid())
        return;
    m_itemNode = itemNode.value<ModelNode>();
    emit itemNodeChanged();
}

static inline QColor normalizeColor(const QColor &color)
{
    QColor newColor = QColor(color.name());
    newColor.setAlpha(color.alpha());
    return newColor;
}

static inline qreal roundReal(qreal real)
{
    int i = real * 100;
    return qreal(i) / 100;
}

void GradientLineQmlAdaptor::setupGradient()
{
    if (!active())
        return;

    ModelNode modelNode = m_itemNode.modelNode();
    QLinearGradient newGradient;
    QVector<QGradientStop> stops;

    if (!modelNode.isValid())
        return;

    if (modelNode.hasProperty(gradientName())) { //gradient exists

        ModelNode gradientNode = modelNode.nodeProperty(gradientName()).modelNode();
        QList<ModelNode> stopList = gradientNode.nodeListProperty("stops").toModelNodeList();

        foreach (const ModelNode &stopNode, stopList) {
            QmlObjectNode stopObjectNode = stopNode;
            if (stopObjectNode.isValid()) {
                qreal position = stopNode.variantProperty("position").value().toReal();
                QColor color = stopNode.variantProperty("color").value().value<QColor>();
                stops.append( QPair<qreal, QColor>(position, color));
            }
        }
    } else {
        stops.append( QPair<qreal, QColor>(0, activeColor()));
        stops.append( QPair<qreal, QColor>(1, QColor(Qt::black)));
    }

    newGradient.setStops(stops);
    setGradient(newGradient);
}

void GradientLineQmlAdaptor::writeGradient()
{
    if (!active())
        return;

    if (!m_itemNode.isValid())
        return;

    if (!m_itemNode.modelNode().metaInfo().hasProperty(gradientName()))
        return;
    try {
        RewriterTransaction transaction = m_itemNode.modelNode().view()->beginRewriterTransaction();

        ModelNode modelNode = m_itemNode.modelNode();

        QString oldId;
        QVector<QGradientStop> stops = gradient().stops();
        if (m_itemNode.isInBaseState()) {
            if (modelNode.hasProperty(gradientName())) {
                oldId = modelNode.nodeProperty(gradientName()).modelNode().id();
                modelNode.removeProperty(gradientName());
            }

            ModelNode gradientNode = modelNode.view()->createModelNode("Qt/Gradient", 4, 7);

            if (!oldId.isNull())
                gradientNode.setId(oldId);

            for (int i = 0;i < stops.size(); i++) {
                ModelNode gradientStopNode = modelNode.view()->createModelNode("Qt/GradientStop", 4, 7);
                gradientStopNode.variantProperty("position") = roundReal(stops.at(i).first);
                gradientStopNode.variantProperty("color") = normalizeColor(stops.at(i).second);
                gradientNode.nodeListProperty("stops").reparentHere(gradientStopNode);
            }
            modelNode.nodeProperty(gradientName()).reparentHere(gradientNode);
        } else { //state
            if  (!modelNode.hasProperty(gradientName())) {
                qWarning(" GradientLine::updateGradient: no gradient in state");
                return;
            }
            ModelNode gradientNode = modelNode.nodeProperty(gradientName()).modelNode();
            QList<ModelNode> stopList = gradientNode.nodeListProperty("stops").toModelNodeList();
            for (int i = 0;i < stops.size(); i++) {
                QmlObjectNode stopObjectNode = stopList.at(i);
                stopObjectNode.setVariantProperty("position", roundReal(stops.at(i).first));
                stopObjectNode.setVariantProperty("color", normalizeColor(stops.at(i).second));
            }
        }
    }
    catch (RewritingException &e) {
        QMessageBox::warning(0, "Error", e.description());
    }
}

void GradientLineQmlAdaptor::deleteGradient()
{
    if (!m_itemNode.isValid())
        return;

    if (!m_itemNode.modelNode().metaInfo().hasProperty(gradientName()))
        return;

    ModelNode modelNode = m_itemNode.modelNode();

    if (m_itemNode.isInBaseState()) {
        if (modelNode.hasProperty(gradientName())) {
            RewriterTransaction transaction = m_itemNode.modelNode().view()->beginRewriterTransaction();
            ModelNode gradientNode = modelNode.nodeProperty(gradientName()).modelNode();
            if (QmlObjectNode(gradientNode).isValid())
                QmlObjectNode(gradientNode).destroy();
        }
    }
}

void GradientLineQmlAdaptor::registerDeclarativeType() {
    qmlRegisterType<QmlDesigner::GradientLineQmlAdaptor>("Bauhaus",1,0,"GradientLine");
}

} //QmlDesigner