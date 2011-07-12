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

#include "qtsupportplugin.h"

#include "qtoptionspage.h"
#include "qtversionmanager.h"

#include "profilereader.h"

#include "gettingstartedwelcomepage.h"

#include <extensionsystem/pluginmanager.h>

#include <QtCore/QtPlugin>
#include <QtGui/QMenu>

using namespace QtSupport;
using namespace QtSupport::Internal;

QtSupportPlugin::~QtSupportPlugin()
{
    removeObject(m_welcomePage);
    delete m_welcomePage;
}

bool QtSupportPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);
    ProFileParser::initialize();
    ProFileEvaluator::initialize();
    new ProFileCacheManager(this);

    QtVersionManager *mgr = new QtVersionManager;
    addAutoReleasedObject(mgr);
    addAutoReleasedObject(new QtOptionsPage);

    m_welcomePage = new GettingStartedWelcomePage;
    addObject(m_welcomePage);

    return true;
}

void QtSupportPlugin::extensionsInitialized()
{
    QtVersionManager::instance()->extensionsInitialized();

}

Q_EXPORT_PLUGIN(QtSupportPlugin)