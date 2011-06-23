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

#ifndef FUNCTIONHINTPROPOSALWIDGET_H
#define FUNCTIONHINTPROPOSALWIDGET_H

#include "iassistproposalwidget.h"

#include <QtCore/QScopedPointer>

namespace TextEditor {

struct FunctionHintProposalWidgetPrivate;

class TEXTEDITOR_EXPORT FunctionHintProposalWidget : public IAssistProposalWidget
{
    Q_OBJECT

public:
    FunctionHintProposalWidget();
    virtual ~FunctionHintProposalWidget();

    virtual void setAssistant(CodeAssistant *assistant);
    virtual void setReason(AssistReason reason);
    virtual void setKind(AssistKind kind);
    virtual void setUnderlyingWidget(const QWidget *underlyingWidget);
    virtual void setModel(IAssistProposalModel *model);
    virtual void setDisplayRect(const QRect &rect);
    virtual void setIsSynchronized(bool isSync);

    virtual void showProposal(const QString &prefix);
    virtual void updateProposal(const QString &prefix);
    virtual void closeProposal();

protected:
    virtual bool eventFilter(QObject *o, QEvent *e);

private slots:
    void nextPage();
    void previousPage();

private:
    bool updateAndCheck(const QString &prefix);
    void updateContent();
    void updatePosition();
    void abort();

private:
    QScopedPointer<FunctionHintProposalWidgetPrivate> m_d;
};

} // TextEditor

#endif // FUNCTIONHINTPROPOSALWIDGET_H
