#ifndef DIALOGSHOWSTRONGS_H
#define DIALOGSHOWSTRONGS_H

#include <QDialog>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include "strongsdatabaseclass.h"
#ifdef WEBKIT_USED
#include <QtWebKitWidgets/QWebView>
#endif
#ifdef WEBENGINE_USED
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#endif

namespace Ui {
class DialogShowStrongs;
}

class DialogShowStrongs : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogShowStrongs(QWidget *parent = nullptr);
    ~DialogShowStrongs();
    void showEvent(QShowEvent *e);
    bool event(QEvent *);
    //void focusOutEvent(QFocusEvent *e);<-- does not fire
    void resizeEvent ( QResizeEvent * event );

    void BuildAndShowStrongsPage();
    void returnToLastLocation();
    QString getLastLocation();
    void goToLocation(QString strongsLocation);
    void moveButton(QPushButton *pb, int resizex) ;
    void moveLineEdit(QLineEdit *le,int resizex);
    void moveLine(QFrame *le,int resizex);
    int strongsNumber;
    QString Language;
    QString textInBible;

    RecordSet *rs;

    void saveCurrentLocation();
    QStringList LastItems;
    void handleRS();
#ifdef WEBKIT_USED
    QWebView *webView;
#endif
#ifdef WEBENGINE_USED
    QWebEngineView *webView;
#endif
private slots:
    void link_click(const QUrl &);
    void on_pbOK_clicked();

    void on_pbLeft_clicked();

    void on_pbRight_clicked();

    void on_pbLast_clicked();

    void on_pbLanguage_clicked();

    void on_lineEdit_editingFinished();

    void on_pbPictures_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_pbDictionary_clicked();

    void on_pbBibleDictionary_clicked();

private:
    bool internalClick;
    Ui::DialogShowStrongs *ui;
    int mainwindowWidth;
    int mainwindowHeight;
   // int webviewOffset;
};

#endif // DIALOGSHOWSTRONGS_H
