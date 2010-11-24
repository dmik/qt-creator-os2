/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/
#include "maemopublishingbuildsettingspagefremantlefree.h"
#include "ui_maemopublishingbuildsettingspagefremantlefree.h"

#include "maemopublisherfremantlefree.h"
#include "maemotoolchain.h"

#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <qt4projectmanager/qt4buildconfiguration.h>
#include <qt4projectmanager/qt4projectmanagerconstants.h>
#include <utils/qtcassert.h>

using namespace ProjectExplorer;

namespace Qt4ProjectManager {
namespace Internal {

MaemoPublishingBuildSettingsPageFremantleFree::MaemoPublishingBuildSettingsPageFremantleFree(const Project *project,
    MaemoPublisherFremantleFree *publisher, QWidget *parent) :
    QWizardPage(parent),
    m_publisher(publisher),
    ui(new Ui::MaemoPublishingWizardPageFremantleFree)
{
    ui->setupUi(this);
    collectBuildConfigurations(project);
    QTC_ASSERT(!m_buildConfigs.isEmpty(), return);
    foreach (const Qt4BuildConfiguration * const bc, m_buildConfigs) {
        ui->buildConfigComboBox->addItem(bc->displayName());
    }
    ui->buildConfigComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
    ui->buildConfigComboBox->setCurrentIndex(0);
    connect(ui->skipUploadCheckBox, SIGNAL(toggled(bool)),
        SLOT(handleNoUploadSettingChanged()));
}

MaemoPublishingBuildSettingsPageFremantleFree::~MaemoPublishingBuildSettingsPageFremantleFree()
{
    delete ui;
}

void MaemoPublishingBuildSettingsPageFremantleFree::collectBuildConfigurations(const Project *project)
{
    foreach (const Target *const target, project->targets()) {
        if (target->id() != QLatin1String(Constants::MAEMO_DEVICE_TARGET_ID))
            continue;
        foreach (BuildConfiguration * const bc, target->buildConfigurations()) {
            Qt4BuildConfiguration * const qt4Bc
                = qobject_cast<Qt4BuildConfiguration *>(bc);
            if (!qt4Bc)
                continue;
            const MaemoToolChain * const tc
                = dynamic_cast<MaemoToolChain *>(qt4Bc->toolChain());
            if (!tc)
                continue;
            if (tc->version() == MaemoToolChain::Maemo5)
                m_buildConfigs << qt4Bc;
        }
        break;
    }
}

bool MaemoPublishingBuildSettingsPageFremantleFree::validatePage()
{
    m_publisher->setBuildConfiguration(m_buildConfigs.at(ui->buildConfigComboBox->currentIndex()));
    m_publisher->setDoUpload(!skipUpload());
    return true;
}

void MaemoPublishingBuildSettingsPageFremantleFree::handleNoUploadSettingChanged()
{
    setCommitPage(skipUpload());
}

bool MaemoPublishingBuildSettingsPageFremantleFree::skipUpload() const
{
    return ui->skipUploadCheckBox->isChecked();
}

} // namespace Internal
} // namespace Qt4ProjectManager