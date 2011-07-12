/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Author: Andreas Hartmetz, KDAB (andreas.hartmetz@kdab.com)
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

#ifndef MEMCHECKERRORVIEW_H
#define MEMCHECKERRORVIEW_H

#include <QtGui/QListView>
#include <QtGui/QStyledItemDelegate>
#include <QtGui/QLabel>

namespace Analyzer {
class AnalyzerSettings;
}

namespace ProjectExplorer {
class Project;
}

namespace Valgrind {
namespace Internal {

class MemcheckErrorDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    /// This delegate can only work on one view at a time, parent. parent will also be the parent
    /// in the QObject parent-child system.
    explicit MemcheckErrorDelegate(QListView *parent);

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

public slots:
    void currentChanged(const QModelIndex &now, const QModelIndex &previous);
    void viewResized();
    void layoutChanged();
    void copy();

private slots:
    void verticalScrolled();
    void openLinkInEditor(const QString &link);

private:
    // the constness of this method is a necessary lie because it is called from paint() const.
    QWidget *createDetailsWidget(const QModelIndex &errorIndex, QWidget *parent) const;

    static const int s_itemMargin = 2;
    mutable QPersistentModelIndex m_detailsIndex;
    mutable QWidget *m_detailsWidget;
    mutable int m_detailsWidgetHeight;
};

class MemcheckErrorView : public QListView
{
    Q_OBJECT

public:
    MemcheckErrorView(QWidget *parent = 0);
    ~MemcheckErrorView();

    // reimplemented to connect delegate to connection model after it has been set by
    // superclass implementation
    void setModel(QAbstractItemModel *model);

    void setDefaultSuppressionFile(const QString &suppFile);
    QString defaultSuppressionFile() const;
    Analyzer::AnalyzerSettings *settings() const { return m_settings; }

public slots:
    void settingsChanged(Analyzer::AnalyzerSettings *settings);
    void goNext();
    void goBack();

signals:
    void resized();

private slots:
    void resizeEvent(QResizeEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);
    void suppressError();
    void setCurrentRow(int row);

private:
    int rowCount() const;
    int currentRow() const;

    QAction *m_copyAction;
    QAction *m_suppressAction;
    QString m_defaultSuppFile;
    Analyzer::AnalyzerSettings *m_settings;
};

} // namespace Internal
} // namespace Valgrind

#endif // MEMCHECKERRORVIEW_H