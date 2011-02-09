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

#ifndef QMLJSCOMPONENTVERSION_H
#define QMLJSCOMPONENTVERSION_H

#include "qmljs_global.h"

namespace QmlJS {

class QMLJS_EXPORT ComponentVersion
{
    int _major;
    int _minor;

public:
    static const int NoVersion;

    ComponentVersion();
    ComponentVersion(int major, int minor);
    ~ComponentVersion();

    int major() const
    { return _major; }
    int minor() const
    { return _minor; }

    // something in the GNU headers introduces the major() and minor() defines,
    // use these two to disambiguate when necessary
    int majorVersion() const
    { return _major; }
    int minorVersion() const
    { return _minor; }

    bool isValid() const;
};

bool operator<(const ComponentVersion &lhs, const ComponentVersion &rhs);
bool operator<=(const ComponentVersion &lhs, const ComponentVersion &rhs);
bool operator==(const ComponentVersion &lhs, const ComponentVersion &rhs);
bool operator!=(const ComponentVersion &lhs, const ComponentVersion &rhs);

} // namespace QmlJS

#endif // QMLJSCOMPONENTVERSION_H