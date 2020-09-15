#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma GCC diagnostic ignored "-Wold-style-cast"
#ifdef Q_OS_WIN
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#pragma GCC diagnostic ignored "-Wunused-function"

#include "ui_mainwindow.h"
#include "ui_tagalogdialog.h"

#include "../shared/ActivityIndicator/activityindicatorclass.h"
//#include "../shared/PopOverHandler/popoverhandlerclass.h"
#include <QMainWindow>
#ifdef WEBKIT_USED
#include <QtWebKitWidgets/QWebView>
#include <QNetworkRequest>

#include <QtWebKit>
#include <QWebFrame>
#include <QWebPage>

#endif
#include <QUrl>
#include "cwpdefines.h"
#ifdef WEBENGINE_USED
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#endif

#define DONT_WRITE_LOCATION_TO_DISK NO
#define WRITE_LOCATION_TO_DISK YES

#include "biblemaxvalues.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QString   urlString;

#ifdef WEBKIT_USED
    QWebView *webView;
#endif
#ifdef WEBENGINE_USED
    QWebEngineView *webView;
#endif

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent ( QResizeEvent * event );
    bool event(QEvent *e);
   // bool eventFilter(QObject *obj, QEvent *event);
    //void focusInEvent(QFocusEvent *e);
    void showEnglishDictionary();
    void showMorphology(QString MorphologyText);

    void showStrongsWithNumber(QString StrongsNumber, /*Language:*/QString L,/* Word:*/QString word);
    void showBookMarkAdd(QString bookNumber,/* Chapter:*/ QString lchapter ,/*Verse:*/ QString lverse);
    void showVerseEditForIndex(int indexIntoSelectedBooksArray,/*  Book:*/ QString bookNumber ,/*Chapter:*/ QString lchapter,/* Verse:*/ QString lverse);
    void showFootNoteForBible( QString BibleNumber, /* Book:*/ QString Number , /*Chapter:*/ QString Chapter , /*Verse:*/ QString Verse , /*SeqNumber:*/ QString SeqNumber);
    void showCopyrightForBookNumber( QString BibleNumber, /* ofType:*/ QString Type);
    void showNotesWindowForNoteIndex(int noteIndex ,/*   Book*/ int lnumber ,/*Chapter*/ int lchapter,/* Verse*/ int lverse);
    void showCopyDialog(QString Number,QString Chapter,QString Verse,QString BibleNumber,QString BibleName);

    void saveCurrentLocation(BOOL WriteToPrefs);
    void startWaitIndicator();


    //--------------------------------------------
    // the following are asynchronous!!
    //--------------------------------------------
    QString elementPageOffset;
#ifdef WEBKIT_USED
   // QString getElementPageOffset(QString element);
    void updateVerseYOffsets();
#endif
    int getVerseYOffset(int16_t verseNumber);
#ifdef WEBENGINE_USED
    void updateVerseYOffsets();
    int getVerseXOffset(int16_t verseNumber);
  //  void getElementPageOffset(QString element);
    bool doSetCurrentVerseByScrollPosition;
    int16_t updateVerseNumber; // used for asynchronous update of verse locations
#endif
    int currentScrollPosition;

    bool elementPageOffsetDone;
    //--------------------------------------------
    //--------------------------------------------

    int mainwindowWidth; // last measured width
    int mainwindowHeight;// last measured height

    int getPageScrollPosition();// get the webkit/webengine offset

    void setCurrentVerseByScrollPosition();

#ifdef WEBENGINE_USED
    void scrollPositionArrived(int scrollPosition);
    void updateNextVerseYOffset();
#endif
    ActivityIndicatorClass *activityIndicator;

    int verseYOffsets[MAX_POSSIBLE_VERSE_NUM]; // actually, 150 should do it.  the extra is for padding


    QString imagePath; // path where images are located

    void createRecentSearchesPopOver();
    QTimer *miscTimer;
    QTimer initTimer;
    QTimer webTimer;

    void LoadWebPage();
    int refreshWebPage;
    int refreshVerses;
    int scrollToCurrentVerse();
    bool doScrollToVerseAfterWebPageLoad;
    int16_t verseToScrollToAfterWebPageLoad;

    int                     book;
    int                     chapter;
    int                     verse;

    QString             bookNameLabel;// may replace later with UILabel
    QString             bookMarkTopic;


    int calculatePositionOfVerse(int VerseNumber);

    int16_t getVerseFromScrollPosition();
    int scrollToVerse(int16_t verse);
public slots:
    void on_actionBible_Dictionary_triggered();

private slots:
    void Init();
    void timerCheck();

    void on_actionSelect_Bibles_triggered();

    void on_actionExit_triggered();

    //--------------------------------------
    //webview slots start
    void link_click(const QUrl &);

    void on_pushButton_clicked();

    void webPageFinishedLoading(bool b);
    void processLink();

    void webPageLoadStarted();
    void webPageURLChanged(const QUrl &url);
#ifdef WEBKIT_USED
    void myDownloadRequested(QNetworkRequest r) ;
#endif
    // webview slots finished
    //--------------------------------------

    void handleElementID(QString element);



    void on_actionBookMark_triggered();

    //void on_actionTopic_triggered();
    void on_actionLeft_triggered();

    void on_actionRight_triggered();

    void on_actionBibleBooks_triggered();

    void on_actionBooksInBible_triggered();

    void on_actionPreferences_triggered();

    void on_actionHistory_triggered();

    void on_actionSearch_2_triggered();


    void on_actionTagalog_triggered();

    void on_actionAbout_triggered();

    void on_actionCreate_translation_template_triggered();

    void on_actionShow_Help_triggered();

private:
    Ui::MainWindow *ui;

    QString lastElementID;
    bool    doShowEditVerse;
    int16_t dismissCounter;
    bool    doShowHelp;
};

extern MainWindow *fvcp;
extern MainWindow *currentFVC;

#ifndef TRUE
#define TRUE 1
#endif
#endif // MAINWINDOW_H


