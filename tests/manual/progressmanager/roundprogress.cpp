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

#include "roundprogress.h"
#include "multitask.h"
#include "runextensions.h"

roundprogress::roundprogress(QWidget *parent)
    : QWidget(parent), task(MyInternalTask(true)), task2(MyInternalTask(false))
{
    ui.setupUi(this);
    ui.progressButton->setIcon(QIcon("find.png"));
    connect(ui.startButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui.startButton2, SIGNAL(clicked()), this, SLOT(start2()));
    connect(ui.startBothButton, SIGNAL(clicked()), this, SLOT(start3()));
    ui.startButton->setFocus();
}

void roundprogress::start()
{
    if (future.isRunning())
        return;
    future = QtConcurrent::run(&MyInternalTask::run, &task);
    ui.progressButton->setFuture(future);
}

void roundprogress::start2()
{
    if (future.isRunning())
        return;
    future = QtConcurrent::run(&MyInternalTask::run, &task2);
    ui.progressButton->setFuture(future);
}

void roundprogress::start3()
{
    if (future.isRunning())
        return;
    future = QtConcurrent::run(&MyInternalTask::run, QList<MyInternalTask*>() << &task2 << &task);
    ui.progressButton->setFuture(future);
}
