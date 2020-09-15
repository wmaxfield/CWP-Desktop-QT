#ifndef DIALOGEDITVERSE_H
#define DIALOGEDITVERSE_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include "cwpdefines.h"
#include "structs.h"
#ifdef WEBKIT_USED
#include <QtWebKitWidgets/QWebView>
#endif

#ifdef WEBENGINE_USED
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#endif

namespace Ui {
class DialogEditVerse;
}

class DialogEditVerse : public QDialog
{
    Q_OBJECT

public:
    QString A_me;
    explicit DialogEditVerse(QWidget *parent = 0);
    DialogEditVerse(int Book, int Chapter, int Verse,int BibleNumber,QString BibleName,QWidget *parent =0) ;

    ~DialogEditVerse();

    void timerEvent(QTimerEvent *e);
    void showEvent(QShowEvent *e);
 //   void focusOutEvent(QFocusEvent *event);
  //  bool event(QEvent *e);

    void setParams(int Book, int Chapter, int Verse, int BibleNumber, QString BibleName);
    void update();
    void getVerseRecord();

    void insertSpaceInWordSeqNumbersAtCurrentIndex();
    void handleRowClick(int indexPath);
    void BuildAndShowStrongsPage(int strongsNumber, QString language);
    void ReloadData();
    void resizeEvent1(QResizeEvent *event);
    void updateRecordFromListArray(int16_t itemNumber);

 //   void hideEvent(QHideEvent *e);
#ifdef WEBKIT_USED
    QWebView *webView;
#endif
#ifdef WEBENGINE_USED
    QWebEngineView *webView;
#endif

private slots:

    void on_pbOK_clicked();

    void on_pbUpdate_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_pbClear_clicked();

    void on_pbUC_clicked();

    void on_pbLC_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_pbCPP_clicked();

    void on_teWord_textChanged();

    void on_leLang_textChanged(const QString &arg1);

    void on_leDict_textChanged(const QString &arg1);

    void on_leFormat_textChanged(const QString &arg1);

    void on_lePost_textEdited(const QString &arg1);

    void on_lePre_textChanged(const QString &arg1);

    void on_pbReSeq_clicked();

    void on_pbMoveToPost_clicked();

    void on_pbMoveToPre_clicked();

    void on_pbUpdateNClose_clicked();

    void on_pbQuoteRight_clicked();

    void on_pbQuoteLeft_clicked();

    void on_pbUpdate_2_clicked();

private:

    int windowWidth; // last measured width
    int windowHeight;// last measured height



    Ui::DialogEditVerse *ui;
    int timerID;
    bool paramsSet;

    int _Book;
    int _StartChapter,_EndChapter;
    int _StartVerse,_EndVerse;
    QString _BibleName;
    int  _BibleNumber;
    int wordSequenceNumber;

    QStringList *stringList;
    QStringList *word,*post,*pre,*lang,*dict,*wformat,*wseq;
    bool verseChanged;
    bool doInitialRowClick;
    void moveVerticalButton(QPushButton *b, int resizey);
    void moveVerticalLE(QLineEdit *le, int resizey);
    void moveVerticalLBL(QLabel *le, int resizey);
    bool canResize;
     STRONGS_STRUCTURE_TYPE dummy;

};

extern DialogEditVerse *devPtr;

#endif // DIALOGEDITVERSE_H
