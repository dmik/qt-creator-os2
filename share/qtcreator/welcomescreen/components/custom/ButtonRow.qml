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

import Qt 4.7
import "ButtonGroup.js" as Behavior

/*
   Class: ButtonRow
   A ButtonRow allows you to group Buttons in a row. It provides a selection-behavior as well.

   Note: This component don't support the enabled property.
   If you need to disable it you should disable all the buttons inside it.

   <code>
       ButtonRow {
           Button { text: "Left" }
           Button { text: "Right" }
       }
   </code>
*/
Row {
    id: root

    /*
     * Property: exclusive
     * [bool=true] Specifies the grouping behavior. If enabled, the checked property on buttons contained
     * in the group will be exclusive.
     *
     * Note that a button in an exclusive group will allways be checkable
     */
    property bool exclusive: true

    /*
     * Property: checkedButton
     * [string] Contains the last checked Button.
     */
    property Item checkedButton;

    Component.onCompleted: {
        Behavior.create(root, {direction: Qt.Horizontal});
    }

    Component.onDestruction: {
        Behavior.destroy();
    }
}
