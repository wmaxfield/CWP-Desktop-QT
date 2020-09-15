#ifndef WEBENGINEVIEWWEBPAGE_H
#define WEBENGINEVIEWWEBPAGE_H

#ifdef WEBENGINE_USED

#include <QtWebEngineWidgets/QtWebEngineWidgets>

class MyWebPage : public QWebEnginePage
{
    Q_OBJECT
public:
    MyWebPage(QObject* parent = 0) : QWebEnginePage(parent){}

    bool acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool)
    {
        if (type == QWebEnginePage::NavigationTypeLinkClicked)
        {
            //QDesktopServices::openUrl(url);
            emit linkClicked(url);
            return false;
        }
        return true;
    }

signals:
    void linkClicked(const QUrl&);

};

#endif

#endif // WEBENGINEVIEWWEBPAGE_H
