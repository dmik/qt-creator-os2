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

#ifndef CHANGEIDSCOMMAND_H
#define CHANGEIDSCOMMAND_H

#include <QMetaType>
#include <QVector>


#include "idcontainer.h"

namespace QmlDesigner {

class ChangeIdsCommand
{
    friend QDataStream &operator>>(QDataStream &in, ChangeIdsCommand &command);
public:
    ChangeIdsCommand();
    ChangeIdsCommand(const QVector<IdContainer> &idVector);

    QVector<IdContainer> ids() const;

private:
    QVector<IdContainer> m_idVector;
};

QDataStream &operator<<(QDataStream &out, const ChangeIdsCommand &command);
QDataStream &operator>>(QDataStream &in, ChangeIdsCommand &command);

} // namespace QmlDesigner

Q_DECLARE_METATYPE(QmlDesigner::ChangeIdsCommand)

#endif // CHANGEIDSCOMMAND_H