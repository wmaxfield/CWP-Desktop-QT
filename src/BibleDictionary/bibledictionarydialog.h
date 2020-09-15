#ifndef BIBLEDICTIONARYDIALOG_H
#define BIBLEDICTIONARYDIALOG_H

#include <QDialog>
#include <QNetworkRequest>
#include <QTimer>
#include "cwpdefines.h"
#ifdef WEBKIT_USED
#include <QWebFrame>
#include <QtWebKitWidgets/QWebView>
#endif
#ifdef WEBENGINE_USED
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#endif

namespace Ui {
class BibleDictionaryDialog;
}

class BibleDictionaryDialog : public QDialog
{
    Q_OBJECT


public:
    explicit BibleDictionaryDialog(QWidget *parent = 0);
             ~BibleDictionaryDialog();

    void resizeEvent ( QResizeEvent * event ) ;
    void showEvent(QShowEvent *e);
    void doDataIndexBasedLoadWebPage(BOOL previous) ;

    void openBibleDictionary();
    void showDictionary() ;
    void ReloadWebPage();
    //----------------------
    void lookUpWordAndShowOnWebPage(QString word);
    QTimer                  webTimer;
    QTimer                  searchTimer;
    //----------------------
    int                     book;
    int                     chapter;
    int                     verse;
    QString                 CurrentWebPage;

    int                     numberOfDefinitions;
    int                     iCurrentDataIndex;
    bool                    firstTime;
    QString                 imagePath;
    QString                 dictionarySearchWord;
    QString                 bibleDictionarySearchWord;
    void                    reject();
#ifdef WEBKIT_USED
    QWebView *webView;
#endif
#ifdef WEBENGINE_USED
    QWebEngineView *webView;
#endif
private slots:
    void on_pbClose_clicked();

    void on_pbSearch_clicked();
    //--------------------------------------
    //--------------------------------------
    void link_click(const QUrl &);
    void myDownloadRequested(QNetworkRequest r) ;
    void processLink();
    void webPageFinishedLoading(bool b);
    void webPageLoadStarted();
    void webPageURLChanged(const QUrl &url);
    //--------------------------------------
    //--------------------------------------
    void on_toolButtonLeft_clicked();

    void on_toolButtonRight_clicked();
    void updateDisplayWithSearchWordDefinition() ;

private:
    Ui::BibleDictionaryDialog *ui;
    int mainwindowWidth; // last measured width
    int mainwindowHeight;// last measured height


    QString   urlString;
    QString   savedURL;     // in windows, have to trigger link twice (esp webkit)
    void  LoadWebPage(int startDataIndex,int numDefinitions);
    void loadWebPageWithString(QString vp);

    void cleanUp();
    void handleCurrentWebEngine();
};
extern BibleDictionaryDialog *bibleDictionaryWindowPtr;


#endif // BIBLEDICTIONARYDIALOG_H
