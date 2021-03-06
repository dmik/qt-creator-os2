/*
  This file was generated by the Html5 Application wizard of Qt Creator.
  Html5ApplicationViewer is a convenience class containing mobile device specific
  code such as screen orientation handling.
  It is recommended not to modify this file, since newer versions of Qt Creator
  may offer an updated version of it.
*/

#ifndef NAVIGATIONCONTROLLER_H
#define NAVIGATIONCONTROLLER_H

#include <QObject>

class QGraphicsWebView;
class QWebPage;

class NavigationController : public QObject
{
    Q_OBJECT

public:
    NavigationController(QWidget *parent, QGraphicsWebView *webView);
    virtual ~NavigationController();

    QWidget *webWidget() const;
    QWebPage* webPage() const;
    QGraphicsWebView* graphicsWebView() const;

signals:
    void pauseNavigation();
    void resumeNavigation();

private:
    class NavigationControllerPrivate *m_d;
};

#endif // NAVIGATIONCONTROLLER_H
