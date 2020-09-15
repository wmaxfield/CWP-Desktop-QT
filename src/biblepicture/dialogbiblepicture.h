#ifndef DIALOGBIBLEPICTURE_H
#define DIALOGBIBLEPICTURE_H

#include <QDialog>
#include "RecordSet/recordset.h"
#include "PictureInfoDatabase/PIctureInfoDatabase.h"
#ifdef WEBKIT_USED
#include <QtWebKitWidgets/QWebView>
#endif
#ifdef WEBENGINE_USED
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#endif

#define MEDIA_PICTURES_SUBDIR "Pictures"
#define MEDIA_MAPS_SUBDIR "Maps"


namespace Ui {
class DialogBiblePicture;
}

class DialogBiblePicture : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBiblePicture(QWidget *parent = 0);
    ~DialogBiblePicture();

    RecordSet *rs;
    void setRS(RecordSet *rs_) { rs= rs_;}// wsm fix wrong set.
    void showEvent(QShowEvent *);
    void resizeEvent ( QResizeEvent * event ) ;
    void reloadData();
    int mainwindowWidth ;
    int mainwindowHeight ;

    PictureInfoDatabase *pidb;

#ifdef WEBKIT_USED
    QWebView *webView;
#endif
#ifdef WEBENGINE_USED
    QWebEngineView *webView;
#endif

private slots:
    void on_pbClose_clicked();
    void on_listWidget_clicked(const QModelIndex &index);

private:
    Ui::DialogBiblePicture *ui;
};

#endif // DIALOGBIBLEPICTURE_H
