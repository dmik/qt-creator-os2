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
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "os2utils.h"
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtGui/QWidget>

#include <qt_os2.h>

using namespace Utils;


void Os2Utils::openFolder(QWidget *parent, const QString &path)
{
    QByteArray name = QFile::encodeName( QDir::toNativeSeparators(path) );
    int        n = name.lastIndexOf('\\');
    HOBJECT    hFldr;

    if (n < 2) {
        name = "<WP_DRIVES>";
    } else {
        if (n == 2)
            n++;
        name.truncate( n );
    }

    hFldr = WinQueryObject(name.constData());
    if (hFldr) {
        // call this twice in order to bring the folder
        // to the top when it is not already open.
        WinOpenObject(hFldr, 0, TRUE);
        WinOpenObject(hFldr, 0, TRUE);
    }
}

