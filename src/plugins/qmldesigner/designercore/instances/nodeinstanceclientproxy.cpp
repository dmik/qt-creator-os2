#include "nodeinstanceclientproxy.h"

#include <QLocalSocket>
#include <QVariant>
#include <QCoreApplication>
#include <QStringList>

#include "nodeinstanceserver.h"
#include "previewnodeinstanceserver.h"

#include "propertyabstractcontainer.h"
#include "propertyvaluecontainer.h"
#include "propertybindingcontainer.h"
#include "instancecontainer.h"
#include "createinstancescommand.h"
#include "createscenecommand.h"
#include "changevaluescommand.h"
#include "changebindingscommand.h"
#include "changefileurlcommand.h"
#include "removeinstancescommand.h"
#include "clearscenecommand.h"
#include "removepropertiescommand.h"
#include "reparentinstancescommand.h"
#include "changeidscommand.h"
#include "changestatecommand.h"
#include "addimportcommand.h"
#include "completecomponentcommand.h"

#include "informationchangedcommand.h"
#include "pixmapchangedcommand.h"
#include "valueschangedcommand.h"
#include "childrenchangedcommand.h"
#include "imagecontainer.h"
#include "statepreviewimagechangedcommand.h"
#include "componentcompletedcommand.h"

namespace QmlDesigner {

NodeInstanceClientProxy::NodeInstanceClientProxy(QObject *parent)
    : QObject(parent),
      m_nodeInstanceServer(new NodeInstanceServer(this)),
      m_baseStateNodeInstancePreview(new PreviewNodeInstanceServer(this)),
      m_blockSize(0)
{
    m_slowSocket = new QLocalSocket(this);
    m_slowSocket->connectToServer(QCoreApplication::arguments().at(1), QIODevice::ReadWrite | QIODevice::Unbuffered);
    m_slowSocket->waitForConnected(-1);

    m_fastSocket = new QLocalSocket(this);
    connect(m_fastSocket, SIGNAL(readyRead()), this, SLOT(readDataStream()));
    connect(m_fastSocket, SIGNAL(error(QLocalSocket::LocalSocketError)), QCoreApplication::instance(), SLOT(quit()));
    connect(m_fastSocket, SIGNAL(disconnected()), QCoreApplication::instance(), SLOT(quit()));
    m_fastSocket->connectToServer(QCoreApplication::arguments().at(1), QIODevice::ReadWrite | QIODevice::Unbuffered);
    m_fastSocket->waitForConnected(-1);
}

void NodeInstanceClientProxy::writeSlowCommand(const QVariant &command)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << quint32(0);
    out << command;
    out.device()->seek(0);
    out << quint32(block.size() - sizeof(quint32));

    m_slowSocket->write(block);
}

void NodeInstanceClientProxy::writeFastCommand(const QVariant &command)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << quint32(0);
    out << command;
    out.device()->seek(0);
    out << quint32(block.size() - sizeof(quint32));

    m_fastSocket->write(block);
}

void NodeInstanceClientProxy::informationChanged(const InformationChangedCommand &command)
{
    writeFastCommand(QVariant::fromValue(command));
}

void NodeInstanceClientProxy::valuesChanged(const ValuesChangedCommand &command)
{
    writeFastCommand(QVariant::fromValue(command));
}

void NodeInstanceClientProxy::pixmapChanged(const PixmapChangedCommand &command)
{
    writeSlowCommand(QVariant::fromValue(command));
}

void NodeInstanceClientProxy::childrenChanged(const ChildrenChangedCommand &command)
{
    writeFastCommand(QVariant::fromValue(command));
}

void NodeInstanceClientProxy::statePreviewImagesChanged(const StatePreviewImageChangedCommand &command)
{
    writeSlowCommand(QVariant::fromValue(command));
}

void NodeInstanceClientProxy::componentCompleted(const ComponentCompletedCommand &command)
{
    writeFastCommand(QVariant::fromValue(command));
}

void NodeInstanceClientProxy::flush()
{
}

qint64 NodeInstanceClientProxy::bytesToWrite() const
{
    return m_slowSocket->bytesToWrite();
}

void NodeInstanceClientProxy::readDataStream()
{
    QList<QVariant> commandList;

    while (!m_fastSocket->atEnd()) {
        if (m_fastSocket->bytesAvailable() < int(sizeof(quint32)))
            break;

        QDataStream in(m_fastSocket);

        if (m_blockSize == 0) {
            in >> m_blockSize;
        }

        if (m_fastSocket->bytesAvailable() < m_blockSize)
            break;

        QVariant command;
        in >> command;
        m_blockSize = 0;

        Q_ASSERT(in.status() == QDataStream::Ok);

        commandList.append(command);
    }

    foreach (const QVariant &command, commandList) {
        dispatchCommand(command);
    }
}

NodeInstanceServerInterface *NodeInstanceClientProxy::nodeInstanceServer() const
{
    return m_nodeInstanceServer;
}

void NodeInstanceClientProxy::createInstances(const CreateInstancesCommand &command)
{
    nodeInstanceServer()->createInstances(command);
}

void NodeInstanceClientProxy::changeFileUrl(const ChangeFileUrlCommand &command)
{
    nodeInstanceServer()->changeFileUrl(command);
}

void NodeInstanceClientProxy::createScene(const CreateSceneCommand &command)
{
    nodeInstanceServer()->createScene(command);
}

void NodeInstanceClientProxy::clearScene(const ClearSceneCommand &command)
{
    nodeInstanceServer()->clearScene(command);
}

void NodeInstanceClientProxy::removeInstances(const RemoveInstancesCommand &command)
{
    nodeInstanceServer()->removeInstances(command);
}

void NodeInstanceClientProxy::removeProperties(const RemovePropertiesCommand &command)
{
    nodeInstanceServer()->removeProperties(command);
}

void NodeInstanceClientProxy::changePropertyBindings(const ChangeBindingsCommand &command)
{
    nodeInstanceServer()->changePropertyBindings(command);
}

void NodeInstanceClientProxy::changePropertyValues(const ChangeValuesCommand &command)
{
    nodeInstanceServer()->changePropertyValues(command);
}

void NodeInstanceClientProxy::reparentInstances(const ReparentInstancesCommand &command)
{
    nodeInstanceServer()->reparentInstances(command);
}

void NodeInstanceClientProxy::changeIds(const ChangeIdsCommand &command)
{
    nodeInstanceServer()->changeIds(command);
}

void NodeInstanceClientProxy::changeState(const ChangeStateCommand &command)
{
    nodeInstanceServer()->changeState(command);
}

void NodeInstanceClientProxy::addImport(const AddImportCommand &command)
{
    nodeInstanceServer()->addImport(command);
}

void NodeInstanceClientProxy::completeComponent(const CompleteComponentCommand &command)
{
    nodeInstanceServer()->completeComponent(command);
}

void NodeInstanceClientProxy::dispatchCommand(const QVariant &command)
{
    static const int createInstancesCommandType = QMetaType::type("CreateInstancesCommand");
    static const int changeFileUrlCommandType = QMetaType::type("ChangeFileUrlCommand");
    static const int createSceneCommandType = QMetaType::type("CreateSceneCommand");
    static const int clearSceneCommandType = QMetaType::type("ClearSceneCommand");
    static const int removeInstancesCommandType = QMetaType::type("RemoveInstancesCommand");
    static const int removePropertiesCommandType = QMetaType::type("RemovePropertiesCommand");
    static const int changeBindingsCommandType = QMetaType::type("ChangeBindingsCommand");
    static const int changeValuesCommandType = QMetaType::type("ChangeValuesCommand");
    static const int reparentInstancesCommandType = QMetaType::type("ReparentInstancesCommand");
    static const int changeIdsCommandType = QMetaType::type("ChangeIdsCommand");
    static const int changeStateCommandType = QMetaType::type("ChangeStateCommand");
    static const int addImportCommandType = QMetaType::type("AddImportCommand");
    static const int completeComponentCommandType = QMetaType::type("CompleteComponentCommand");

    if (command.userType() ==  createInstancesCommandType) {
        createInstances(command.value<CreateInstancesCommand>());
    } else if (command.userType() ==  changeFileUrlCommandType)
        changeFileUrl(command.value<ChangeFileUrlCommand>());
    else if (command.userType() ==  createSceneCommandType)
        createScene(command.value<CreateSceneCommand>());
    else if (command.userType() ==  clearSceneCommandType)
        clearScene(command.value<ClearSceneCommand>());
    else if (command.userType() ==  removeInstancesCommandType)
        removeInstances(command.value<RemoveInstancesCommand>());
    else if (command.userType() ==  removePropertiesCommandType)
        removeProperties(command.value<RemovePropertiesCommand>());
    else if (command.userType() ==  changeBindingsCommandType)
        changePropertyBindings(command.value<ChangeBindingsCommand>());
    else if (command.userType() ==  changeValuesCommandType)
        changePropertyValues(command.value<ChangeValuesCommand>());
    else if (command.userType() ==  reparentInstancesCommandType)
        reparentInstances(command.value<ReparentInstancesCommand>());
    else if (command.userType() ==  changeIdsCommandType)
        changeIds(command.value<ChangeIdsCommand>());
    else if (command.userType() ==  changeStateCommandType)
        changeState(command.value<ChangeStateCommand>());
    else if (command.userType() ==  addImportCommandType)
        addImport(command.value<AddImportCommand>());
    else if (command.userType() ==  completeComponentCommandType)
        completeComponent(command.value<CompleteComponentCommand>());
    else
        Q_ASSERT(false);
}
} // namespace QmlDesigner