#ifndef DIALOGFOOTNOTE_H
#define DIALOGFOOTNOTE_H

#include <QDialog>
#include <QString>

#ifdef WEBKIT_USED
#include <QtWebKitWidgets/QWebView>
#endif
#ifdef WEBENGINE_USED
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#endif


namespace Ui {
class DialogFootNote;
}

class DialogFootNote : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFootNote(QWidget *parent = 0);
    ~DialogFootNote();
    void showEvent(QShowEvent *);
    int bibleNumber;
    int bookNumber;
    int chapter;
    int verse;
    int sequenceNumber;
    int isBook;

    QString FootNoteText;
#ifdef WEBKIT_USED
    QWebView *webView;
#endif
#ifdef WEBENGINE_USED
    QWebEngineView *webView;
#endif
private slots:
    void on_pbOK_clicked();

private:
    Ui::DialogFootNote *ui;
};

#endif // DIALOGFOOTNOTE_H
