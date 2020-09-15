#ifndef DIALOGENGLISHDICTIONARY_H
#define DIALOGENGLISHDICTIONARY_H

#include <QDialog>
#include "dialogshowstrongs.h"
#include "mainwindow.h"
#ifdef WEBKIT_USED
#include <QtWebKitWidgets/QWebView>
#endif

namespace Ui {
class DialogEnglishDictionary;
}

class DialogEnglishDictionary : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEnglishDictionary(QWidget *parent = 0);
    ~DialogEnglishDictionary();
    void showEvent(QShowEvent *e);
    void resizeEvent(QResizeEvent *e);
    void updateDisplayWithSearchWordDefinition();
    int calledFromStrongs;
    DialogShowStrongs *dstp;
    MainWindow *mainwindow;
    int numberOfDefinitions;
    QString dictionarySearchWord;
#ifdef WEBKIT_USED
    QWebView *webView;
#endif
#ifdef WEBENGINE_USED
    QWebEngineView *webView;
#endif


private slots:
    void on_pbOK_clicked();
    void link_click(const QUrl &url);

    void on_lineEdit_editingFinished();

    void on_pbSearch_clicked();

    void on_pbBibleDictionary_clicked();
    void processLink();


private:
    Ui::DialogEnglishDictionary *ui;
    int mainwindowWidth, mainwindowHeight;

    QString urlString; // for processLink
    QTimer webTimer; // is required to allow links to load a web page
};

#endif // DIALOGENGLISHDICTIONARY_H
