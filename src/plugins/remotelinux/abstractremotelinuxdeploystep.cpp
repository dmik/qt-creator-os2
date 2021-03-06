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
#include "abstractremotelinuxdeploystep.h"

#include "abstractremotelinuxdeployservice.h"
#include "remotelinuxdeployconfiguration.h"
#include "remotelinuxdeploystepwidget.h"

#include <projectexplorer/projectexplorerconstants.h>
#include <qt4projectmanager/qt4buildconfiguration.h>

using namespace ProjectExplorer;

namespace RemoteLinux {
namespace Internal {

class AbstractRemoteLinuxDeployStepPrivate
{
public:
    bool hasError;
    QFutureInterface<bool> future;
};

} // namespace Internal

AbstractRemoteLinuxDeployStep::AbstractRemoteLinuxDeployStep(BuildStepList *bsl, const QString &id)
    : BuildStep(bsl, id), m_d(new Internal::AbstractRemoteLinuxDeployStepPrivate)
{
}

AbstractRemoteLinuxDeployStep::AbstractRemoteLinuxDeployStep(BuildStepList *bsl,
        AbstractRemoteLinuxDeployStep *other)
    : BuildStep(bsl, other), m_d(new Internal::AbstractRemoteLinuxDeployStepPrivate)
{
}

bool AbstractRemoteLinuxDeployStep::fromMap(const QVariantMap &map)
{
    if (!BuildStep::fromMap(map))
        return false;
    deployService()->importDeployTimes(map);
    return true;
}

QVariantMap AbstractRemoteLinuxDeployStep::toMap() const
{
    return BuildStep::toMap().unite(deployService()->exportDeployTimes());
}

bool AbstractRemoteLinuxDeployStep::init()
{
    return isDeploymentPossible();
}

bool AbstractRemoteLinuxDeployStep::isDeploymentPossible(QString *whyNot) const
{
    deployService()->setDeviceConfiguration(deployConfiguration()->deviceConfiguration());
    deployService()->setBuildConfiguration(qobject_cast<Qt4ProjectManager::Qt4BuildConfiguration *>(buildConfiguration()));
    return deployService()->isDeploymentPossible(whyNot);
}

void AbstractRemoteLinuxDeployStep::run(QFutureInterface<bool> &fi)
{
    connect(deployService(), SIGNAL(errorMessage(QString)), SLOT(handleErrorMessage(QString)));
    connect(deployService(), SIGNAL(progressMessage(QString)), SLOT(handleProgressMessage(QString)));
    connect(deployService(), SIGNAL(stdOutData(QString)), SLOT(handleStdOutData(QString)));
    connect(deployService(), SIGNAL(stdErrData(QString)), SLOT(handleStdErrData(QString)));
    connect(deployService(), SIGNAL(finished()), SLOT(handleFinished()));

    m_d->hasError = false;
    m_d->future = fi;
    deployService()->start();
}

void AbstractRemoteLinuxDeployStep::cancel()
{
    if (m_d->hasError)
        return;

    emit addOutput(tr("User requests deployment to stop; cleaning up."), MessageOutput);
    m_d->hasError = true;
    deployService()->stop();
}

BuildStepConfigWidget *AbstractRemoteLinuxDeployStep::createConfigWidget()
{
    return new RemoteLinuxDeployStepWidget(this);
}

RemoteLinuxDeployConfiguration *AbstractRemoteLinuxDeployStep::deployConfiguration() const
{
    return qobject_cast<RemoteLinuxDeployConfiguration *>(BuildStep::deployConfiguration());
}

void AbstractRemoteLinuxDeployStep::handleProgressMessage(const QString &message)
{
    emit addOutput(message, MessageOutput);
}

void AbstractRemoteLinuxDeployStep::handleErrorMessage(const QString &message)
{
    emit addTask(Task(Task::Error, message, QString(), -1,
        ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM));
    m_d->hasError = true;
}

void AbstractRemoteLinuxDeployStep::handleFinished()
{
    if (m_d->hasError)
        emit addOutput(tr("Deployment failed."), ErrorMessageOutput);
    else
        emit addOutput(tr("Deployment finished."), MessageOutput);
    disconnect(deployService(), 0, this, 0);
    m_d->future.reportResult(!m_d->hasError);
    emit finished();
}

void AbstractRemoteLinuxDeployStep::handleStdOutData(const QString &data)
{
    emit addOutput(data, NormalOutput, DontAppendNewline);
}

void AbstractRemoteLinuxDeployStep::handleStdErrData(const QString &data)
{
    emit addOutput(data, ErrorOutput, DontAppendNewline);
}

} // namespace RemoteLinux
