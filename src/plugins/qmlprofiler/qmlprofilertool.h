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

#ifndef QMLPROFILERTOOL_H
#define QMLPROFILERTOOL_H

#include <analyzerbase/ianalyzertool.h>
#include <analyzerbase/ianalyzerengine.h>

namespace QmlProfiler {
namespace Internal {

class QmlProfilerTool : public Analyzer::IAnalyzerTool
{
    Q_OBJECT

public:
    explicit QmlProfilerTool(QObject *parent);
    ~QmlProfilerTool();

    QByteArray id() const;
    QString displayName() const;
    QString description() const;
    ToolMode toolMode() const;

    void extensionsInitialized() {}

    Analyzer::IAnalyzerEngine *createEngine(const Analyzer::AnalyzerStartParameters &sp,
        ProjectExplorer::RunConfiguration *runConfiguration = 0);

    QWidget *createWidgets();
    void startTool(Analyzer::StartMode mode);

public slots:
    void connectClient(int port);
    void disconnectClient();

    void startRecording();
    void stopRecording();
    void setRecording(bool recording);

    void gotoSourceLocation(const QString &fileUrl, int lineNumber);
    void updateTimer(qreal elapsedSeconds);
    void correctTimer();

    void clearDisplay();

signals:
    void setTimeLabel(const QString &);
    void fetchingData(bool);
    void connectionFailed();

private slots:
    void updateProjectFileList();
    void attach();
    void tryToConnect();
    void connectionStateChanged();

private:
    void connectToClient();
    void updateRecordingState();
    void ensureWidgets();
    void logStatus(const QString &msg);
    void logError(const QString &msg);

    class QmlProfilerToolPrivate;
    QmlProfilerToolPrivate *d;
};

} // namespace Internal
} // namespace QmlProfiler

#endif // QMLPROFILERTOOL_H
