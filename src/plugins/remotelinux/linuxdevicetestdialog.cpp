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
#include "linuxdevicetestdialog.h"
#include "ui_linuxdevicetestdialog.h"

#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtGui/QPushButton>
#include <QtGui/QTextCharFormat>

namespace RemoteLinux {
namespace Internal {
class LinuxDeviceTestDialogPrivate {
public:
    LinuxDeviceTestDialogPrivate(AbstractLinuxDeviceTester *tester)
        : deviceTester(tester), finished(false)
    {
    }

    Ui::LinuxDeviceTestDialog ui;
    AbstractLinuxDeviceTester * const deviceTester;
    bool finished;
};

} // namespace Internal

using namespace Internal;

LinuxDeviceTestDialog::LinuxDeviceTestDialog(const QSharedPointer<const LinuxDeviceConfiguration> &deviceConfiguration,
        AbstractLinuxDeviceTester *deviceTester, QWidget *parent)
    : QDialog(parent), m_d(new LinuxDeviceTestDialogPrivate(deviceTester))
{
    m_d->ui.setupUi(this);

    m_d->deviceTester->setParent(this);
    connect(m_d->deviceTester, SIGNAL(progressMessage(QString)), SLOT(handleProgressMessage(QString)));
    connect(m_d->deviceTester, SIGNAL(errorMessage(QString)), SLOT(handleErrorMessage(QString)));
    connect(m_d->deviceTester, SIGNAL(finished(RemoteLinux::AbstractLinuxDeviceTester::TestResult)),
        SLOT(handleTestFinished(RemoteLinux::AbstractLinuxDeviceTester::TestResult)));
    m_d->deviceTester->testDevice(deviceConfiguration);
}

LinuxDeviceTestDialog::~LinuxDeviceTestDialog()
{
    delete m_d;
}

void LinuxDeviceTestDialog::reject()
{
    if (!m_d->finished)
        m_d->deviceTester->stopTest();
    QDialog::reject();
}

void LinuxDeviceTestDialog::handleProgressMessage(const QString &message)
{
    addText(message, QLatin1String("black"), false);
}

void LinuxDeviceTestDialog::handleErrorMessage(const QString &message)
{
    addText(message, QLatin1String("red"), false);
}

void LinuxDeviceTestDialog::handleTestFinished(AbstractLinuxDeviceTester::TestResult result)
{
    m_d->finished = true;
    m_d->ui.buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Close"));

    if (result == AbstractLinuxDeviceTester::TestSuccess)
        addText(tr("Device test finished successfully."), QLatin1String("blue"), true);
    else
        addText(tr("Device test failed."), QLatin1String("red"), true);
}

void LinuxDeviceTestDialog::addText(const QString &text, const QString &color, bool bold)
{
    QTextCharFormat format = m_d->ui.textEdit->currentCharFormat();
    format.setForeground(QBrush(QColor(color)));
    QFont font = format.font();
    font.setBold(bold);
    format.setFont(font);
    m_d->ui.textEdit->setCurrentCharFormat(format);
    m_d->ui.textEdit->appendPlainText(text);
}

} // namespace RemoteLinux
