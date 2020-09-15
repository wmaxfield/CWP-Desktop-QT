#ifndef DIALOGCOPYRIGHT_H
#define DIALOGCOPYRIGHT_H

#include <QDialog>
#ifdef WEBKIT_USED
#include <QtWebKitWidgets/QWebView>
#endif
#ifdef WEBENGINE_USED
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#endif

namespace Ui {
class DialogCopyright;
}

class DialogCopyright : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCopyright(QWidget *parent = 0);
    ~DialogCopyright();
    void showEvent(QShowEvent *);

#ifdef WEBKIT_USED
    QWebView *webView;
#endif
#ifdef WEBENGINE_USED
    QWebEngineView *webView;
#endif

    int bookNumber;
    int bookType;
    QString sCopyrightText;

private slots:
    void on_pbOK_clicked();

private:
    Ui::DialogCopyright *ui;
};

#endif // DIALOGCOPYRIGHT_H
