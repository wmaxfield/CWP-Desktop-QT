/* ========================================
 *
 * Copyright Wade Maxfield, 2010-2020
 * Commercial license available
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

 * ========================================
*/
#ifdef WEBENGINE_USED
#undef WEBKIT_USED
#endif
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../dialogs/SelectBibles/selectbiblesdialog.h"
#include "bibleinfo.h"
#include "preferencesmodule.h"
#include <QApplication>

#include <QGridLayout>
#include <QTimer>
#include "dialogpreferences.h"

#include "helplibrary.h"
#include "PreferencesLibrary.h"
#include "dialogcopyright.h"
#include "noteswindow.h"
#include "bookmarkhandler.h"
#include "timerclass.h"
#include "dialogaddtopic.h"
#include "dialogshowstrongs.h"
#include "dialogenglishdictionary.h"
#include "HTMLModule.h"
#include "userhistoryclass.h"

#include "dialogfootnote.h"
#include "dialoghistory.h"
#include "dialogsearch.h"
#include "dialogbookmarkviewer.h"

#include "bibledictionarydialog.h"
#include "../tagalog/tagalogdialog.h"
#include "../translationtextfile/translationtextfiledialog.h"
#include "../dialogs/CopyDialog/copydialog.h"
#include "../CWP/dialogEditVerse/dialogeditverse.h"
#ifdef WEBENGINE_USED
#include "WebEngineViewWebPage.h"
#endif
#ifdef WEBKIT_USED
#include <QWebView>
#include <QUrl>
#include <QWebFrame>
#endif
#include <QToolTip>
/*
some icons from openclipart.org
*/

#include "helpLibrary/helplibrary.h"

extern TimerClass *bmhTimer;
extern int         loadStarted;
//--------------------------
// define below to a 1
// to get a popup window
// with the web page text
//-------------------------
#define DEBUG_WEB_PAGE 0



extern MainWindow *fvcp;
extern QString CurrentWebPage;
extern int clicked;
extern DialogCopyright *copyrightWindow;
extern NotesWindow *notesWindow;
extern BibleDictionaryDialog *bibleDictionaryWindowPtr;
extern QString dictionarySearchWord; // global so everyone can see

int clicked;
DialogCopyright *copyrightWindow;
NotesWindow *notesWindow;

QString dictionarySearchWord; // global so everyone can see


BookMarkHandler *bmh;
TimerClass *bmhTimer;
int         loadStarted;
static TimerClass *nwTimer;

SelectBiblesDialog *sbd;

static NotesWindow *dnw;
int urlChanged;

int emergencyExit;

static void _topFuncForIndex(){}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
static void handleDebugWebWindowShow(QString vp) {
    //Q_UNUSED(vp);


    if (!dnw)
        dnw = new NotesWindow();

    if (dnw) {
        dnw->dontReloadText=YES;
        dnw->show();
        dnw->setPlainText(vp);

    }

}

// ------------------------------------------------------------------------
// bookmark handler is always created (is delete on close)
// ------------------------------------------------------------------------
void handleBookMarkWindow(){
   // bmh was set up by calling function
    bmh->show();
}

//TimerClass *initTimer;
extern QString debugFlag;
int enable_debug;
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QRect r;

    fvcp = this;// first view pointer
    currentFVC = this;
    doShowHelp=false;

    setWindowIcon(QIcon(":/icons/icon/CWPIcon256x256.png"));

    ui->setupUi(this);

    int HideMenus =1;
    if (Instr(debugFlag,"debug=1")){
        HideMenus=0;
        enable_debug=1;
    }
    if(enable_debug)
        MsgBox(debugFlag);

    //QScreen *screen;

    QString g = Preferences->getMainWindowGeometry();
    QStringList a = g.split(",");
    if (a.length()>1){

        // x y w h
        r.setX(a[0].toInt());
        r.setY(a[1].toInt());
        r.setWidth(a[2].toInt());
        r.setHeight(a[3].toInt());

        if (r.x()<0){
            r.setX(10);
        }
        if (r.y()<0)
            r.setY(10);
        if (r.height()<100)
            r.setHeight(600);
        if (r.width()<50)
            r.setWidth(800);
    }else {
            r.setX(10);
            r.setY(10);
            r.setHeight(600);
            r.setWidth(800);
    }

    this->setGeometry(r);

    move(r.x(),r.y());

    mainwindowWidth = this->width();
    mainwindowHeight = this->height();

#ifdef WEBENGINE_USED
    webView = new QWebEngineView(ui->centralWidget);
    doSetCurrentVerseByScrollPosition=false;
    webView->setObjectName(QStringLiteral("webView"));
#endif

#ifdef WEBKIT_USED
    // wsm move webview creation here for migration to/from qwebengine
    webView = new QWebView(ui->centralWidget);
    webView->setObjectName(QStringLiteral("webView"));
    webView->setGeometry(QRect(0, 35, 891, 471));// default
#endif
    // original settings below
    webView->setUrl(QUrl(QStringLiteral("about:blank")));
    r.setX(0);
    r.setY(0);
    // reconfigure
    r.setHeight( mainwindowHeight);
    r.setWidth(mainwindowWidth);
    webView->setGeometry(r);


    activityIndicator = nullptr;

    ui->mainToolBar->setStyleSheet("QToolBar { background: white; border: 1px solid white;}");
#ifdef WEBKIT_USED
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);// or DelegateAllLinks
    connect( webView->page() , SIGNAL( linkClicked(const QUrl &)), this, SLOT(link_click(const QUrl &)));
    connect( webView, SIGNAL(loadFinished(bool)),this,SLOT(webPageFinishedLoading(bool)));
    connect(webView,SIGNAL(loadStarted()),this,SLOT(webPageLoadStarted()));
    connect(webView,SIGNAL(urlChanged(QUrl)),this,SLOT(webPageURLChanged(const QUrl &)));
    //connect(webView->page(),SIGNAL(downloadRequested(QNetworkRequest)),this,SLOT(myDownloadRequested(QNetworkRequest r)));

    connect(webView->page(),SIGNAL(downloadRequested(QNetworkRequest)),this,SLOT(myDownloadRequested(QNetworkRequest)));
#endif

#ifdef WEBENGINE_USED
    webView->setPage(new MyWebPage());
    connect(webView->page(),SIGNAL(linkClicked(QUrl)),this,SLOT(link_click(const QUrl &)));
    connect(webView,SIGNAL(loadStarted()),this,SLOT(webPageLoadStarted()));
    connect( webView, SIGNAL(loadFinished(bool)),this,SLOT(webPageFinishedLoading(bool)));
    connect(webView,SIGNAL(urlChanged(QUrl)),this,SLOT(webPageURLChanged(const QUrl &)));

#endif


    webView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding );
    ui->pushButton->setVisible(0);

    //---------------
    // race conditions forced delay of some initialization
    connect(&initTimer,SIGNAL(timeout()),this,SLOT(Init()));
    initTimer.start(50);
    //---------------

    //---------------------------------------------------------
    // web page things have to be delayed to function properly
    connect(&webTimer,SIGNAL(timeout()),this,SLOT(processLink()));


    refreshVerses=NO;
    refreshWebPage=YES;
    globalSTI.DoScrollToVerse=NO;//
    doScrollToVerseAfterWebPageLoad=YES;
    verseToScrollToAfterWebPageLoad=Preferences->getCurrentVerse();

    if( HideMenus ) {
        // works for hiding menu on action bar.
        ui->menuEditDBs->menuAction()->setVisible(false);
    }

    // always hide the "search" menu.  search will be enabled later
    ui->menuSearch->menuAction()->setVisible(false);

    doShowEditVerse=false;
    dismissCounter=0;
    for(int i=0; i < MAX_POSSIBLE_VERSE_NUM; i++)
        verseYOffsets[i]=0;

   // setAttribute(Qt::WA_QuitOnClose,true);

}


//------------------------------------------------------------
// overrides resizeEvent to allow button movement
//------------------------------------------------------------
void MainWindow::resizeEvent ( QResizeEvent * event ) {

    Q_UNUSED(event)


    QRect r = webView->geometry();
    int newx,newy,resizex,resizey;
    //r.setX(r.x()+20);
    newx = this->width();
    newy = this->height();
    resizex = newx - mainwindowWidth;
    resizey = newy - mainwindowHeight;
    //x = this->ui->pbQuit->pos().x();
    //y = this->ui->pbQuit->pos().y();
    //this->ui->pbQuit->move(x+resizex, y+resizey);
    //this->webView->width() = newx;
    r.setWidth(resizex +r.width());
    r.setHeight(resizey + r.height());

    webView->setGeometry(r);
    // newy - 50;
    mainwindowWidth = newx;
    mainwindowHeight = newy;
    //activityIndicator->stopAnimation();

}

bool MainWindow::event(QEvent *e)
{
    bool result = QMainWindow::event(e);
    enum QEvent::Type ev = e->type();

    switch(ev){
        case QEvent::Timer:
         break;
        //break;

        case QEvent::ActivationChange:
            // the qwidget is not seen by the main window, except through
        // the change in activation.  This is a "modal" widget, so it
        // will be removed when clicked away from.  So, if it is up,
        // the second activation change event will be the event to
        // close the window.
            if (devPtr){
                dismissCounter++;
                if (dismissCounter>1){
                    try {
                    devPtr->close();
                    }catch (int e){
                        //ignore
                        Q_UNUSED(e)
                    }

                    dismissCounter=0;
                    devPtr=nullptr;
                }
            }
        break;

        case QEvent::Close:{
            QRect r = geometry();
            QString s = QString::number(r.x()) + ","+QString::number(r.y()) + "," + QString::number(r.width()) +","+QString::number(r.height());
            Preferences->setMainWindowGeometry(s);
        }

        break;

        default:
          break;
    }
    return result;
}
/*
void MainWindow::focusInEvent(QFocusEvent *e)
{
    if (devPtr){
        devPtr->close();
        devPtr=nullptr;
    }
}*/

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int MainWindow::getPageScrollPosition(){

#ifdef WEBKIT_USED
    currentScrollPosition = webView->page()->currentFrame()->evaluateJavaScript("window.pageYOffset").toInt();
#endif
#ifdef WEBENGINE_USED
      webView->page()->runJavaScript("window.pageYOffset", [this](const QVariant &result){
          //qDebug() << "Value is: " << result.toString() << endl;
          int position= result.toInt();
          this->currentScrollPosition =position;
      });
#endif
    return currentScrollPosition;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::handleElementID(QString idString)
{

    lastElementID = idString;
    //startingOrientation = UIDeviceOrientationUnknown;// for calculations on where to put strongs when rotates.  if zero, then is not known
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void handleNotesWindow() {

    if (!notesWindow)
        notesWindow = new NotesWindow();

    if (notesWindow) {
        notesWindow->show();
    }
}

#define OBJNAME_IDX 0
#define TOP_OFFSET_IDX 1
#define LEFT_OFFSET_IDX 2
#if WEBKIT_USED
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::updateVerseYOffsets() {// update the verses from the page (run a javascript)

    int verseTest=1;
    QString gl;

    BookChapterVerseStructType bcv;

     GetBookChapterVerse(Preferences->getCurrentBook(),bcv);

    int maxVerse = bcv.NumberOfVerses[Preferences->getCurrentChapter()];


    do {
        gl = "ObjectPosition(\"v"+QString::number(verseTest)+"\");";  //,element];

        gl = webView->page()->currentFrame()->evaluateJavaScript(gl).toString();

        if (gl.length()){
            QStringList a=gl.split("+");
            this->verseYOffsets[verseTest]= a[TOP_OFFSET_IDX].toInt();

            if (this->doScrollToVerseAfterWebPageLoad){
                if (verseTest==fvcp->verseToScrollToAfterWebPageLoad){
                    this->doScrollToVerseAfterWebPageLoad=NO;
                    this->scrollToVerse(fvcp->verseToScrollToAfterWebPageLoad);
                }
            }

        }else{
            // failure, probably no bible shown.  ignore.
            return;
        }

    } while (++verseTest<=maxVerse);


}
#endif

int noBibleShown;
#ifdef WEBENGINE_USED
// ------------------------------------------------------------------------
// java script on page returns the following string:
// "objectName" "+" "topvalue" "+" "leftvalue"   (ie. "v1+59+35")
// ------------------------------------------------------------------------
void MainWindow::updateVerseYOffsets() {

    QString js;
    updateVerseNumber=1;
    for(int i=0; i < MAX_POSSIBLE_VERSE_NUM; i++)
        verseYOffsets[i]=0;

    js = "ObjectPosition(\"v"+QString::number(updateVerseNumber)+"\");";  //,element];
    webView->page()->runJavaScript(js,[this](const QVariant &result){
        QString s=result.toString();
        if (s.length()){
            QStringList a=s.split("+");
            this->verseYOffsets[1]= a[TOP_OFFSET_IDX].toInt();

            if (this->doScrollToVerseAfterWebPageLoad){
                if (1==fvcp->verseToScrollToAfterWebPageLoad){
                    this->doScrollToVerseAfterWebPageLoad=NO;
                    this->scrollToCurrentVerse();
                }
            }
            this->updateNextVerseYOffset();
        }else{
            // failure, probably no bible shown.  ignore.
            noBibleShown++;
        }
    });

}

// ------------------------------------------------------------------------
// java script on page returns the following string:
// "objectName" "+" "topvalue" "+" "leftvalue"   (ie. "v1+59+35")
// ------------------------------------------------------------------------
void MainWindow::updateNextVerseYOffset(){
    QString js;

    updateVerseNumber++;
    BookChapterVerseStructType bcv;

     GetBookChapterVerse(Preferences->getCurrentBook(),bcv);

    int maxVerse = bcv.NumberOfVerses[Preferences->getCurrentChapter()];
    if (updateVerseNumber > maxVerse){
        return; // finished
    }

    js = "ObjectPosition(\"v"+QString::number(updateVerseNumber)+"\");";  //,element];
    webView->page()->runJavaScript(js,[this](const QVariant &result){
#define OBJNAME_IDX 0
#define TOP_OFFSET_IDX 1
#define LEFT_OFFSET_IDX 2

        static int rerunNumber;

        QString s=result.toString();
        if (s.length()){
            rerunNumber=0;
            QStringList a=s.split("+");
            this->verseYOffsets[this->updateVerseNumber]= a[TOP_OFFSET_IDX].toInt();
            //this->doUpdateNextVerseYOffset=true;
            if (this->doScrollToVerseAfterWebPageLoad){
                if (this->updateVerseNumber==fvcp->verseToScrollToAfterWebPageLoad){
                    this->doScrollToVerseAfterWebPageLoad=NO;
                    this->scrollToVerse(fvcp->verseToScrollToAfterWebPageLoad);
                }
            }

        } else {
            if (++rerunNumber > 10)
                return; // failure, don't publicize
            (this->updateVerseNumber)--;

        }

        this->updateNextVerseYOffset();

    });
}


#endif
#if 0

// ------------------------------------------------------------------------
// return a string containing
// "objectName" "+" "topvalue" "+" "leftvalue"   (ie. "v1+59+35")
// ------------------------------------------------------------------------
QString MainWindow::getElementPageOffset(QString element) {
    QString js;

    js = "ObjectPosition(\""+element+"\");";  //,element];
    elementPageOffset = webView->page()->runJavaScript(js).toString();
    return elementPageOffset;

}


// ------------------------------------------------------------------------
// return a string containing
// "objectName" "+" "topvalue" "+" "leftvalue"   (ie. "v1+59+35")
// ------------------------------------------------------------------------

void MainWindow::getElementPageOffset(QString element) {
    QString js;

    js = "ObjectPosition(\""+element+"\");";  //,element];
    webView->page()->runJavaScript(js,[](const QVariant &result){
        fvcp->elementPageOffset=result.toString();
        fvcp->elementPageOffsetDone=true;
    });

}

#endif
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int MainWindow::getVerseYOffset(int16_t verseNumber) {
    if (verseNumber <=0 || verseNumber > MAX_POSSIBLE_VERSE_NUM)
        return 0;
    return verseYOffsets[verseNumber];
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::Init() {

        initTimer.stop();

    if ( emergencyExit) {
        MsgBox("CWP is unable to run properly.  Maybe another copy is running?  Exiting program.");
        close();
    }

    globalSTI.enableEnglishDictionary =Preferences->getShowEnglishDictionaryWindow();
    globalSTI.BlendStrongsIntoWord = Preferences->getBlendStrongs();
    globalSTI.ShowStrongs = Preferences->getEnableDisplayStrongs();
    //globalSTI.enableEnglishDictionary = Preferences->getEnableEnglishDictionary();
    globalSTI.HilightSearch = NO;
    globalSTI.doStackVerses = NO;
    globalSTI.showCopyTag = Preferences->getShowCopyTag();
    globalSTI.showBookMarkTag = Preferences->getShowBookMarkTag();

    // force load of preference values for verses
    Preferences->getBlue();
    Preferences->getRed();
    Preferences->getGreen();
    // the image path is where the image .png and .gif files live
    imagePath = getImageDirectory();

    if (debugFlag.contains("1"))
        globalSTI.EnableVerseEditing=YES;


       /*
        *webview signals
       iconChanged();
       loadFinished(bool ok);
       loadProgress(int progress);
       loadStarted();
       linkClicked(QUrl &url);
       selectionChanged();
       statusBarMessage();
       titleChanged();
       urlChanged(const QUrl &url);
       */

         miscTimer = new QTimer(this);
       connect(miscTimer, SIGNAL(timeout()), this, SLOT(timerCheck()));
       refreshWebPage=1;
       miscTimer->start(60);



}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}


//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::link_click(const QUrl &url) {
    //Q_UNUSED(url);
    clicked++;

    urlString = url.toString();
    //------------------------
    // processLink() is called
    webTimer.start(50);// when fires, processLink() is called
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::LoadWebPage()
{
    //activityIndicator->animate();
    globalSTI.EnableMultimediaTag = Preferences->getSpeechEnabled();

    book =Preferences->getCurrentBook() ;

    chapter =Preferences->getCurrentChapter();

    verse = Preferences->getCurrentVerse();

    BibleBookInfoStructType bbis=getBibleBookInfo(book );

    if (Preferences->getCurrentChapter()> bbis.NumChapters ) {
        chapter =1;
    }

    bookNameLabel =" Chapter "+QString::number(chapter)+" ";

    int numSelectedBooks = Preferences->getNumberOfSelectedBooks() ;
    int numSelectedNotes = Preferences->getNumberOfSelectedNotes();
    if ((numSelectedBooks+ numSelectedNotes+
          Preferences->numberOfSelectedCommentaries) == 0  ) {
        CurrentWebPage = EMPTY_PAGE_STRING;
    } else {
        // add the notes to the end of the selected books array
        for (int index=0; index < numSelectedNotes; index++){
            // copy the notes into the array
            Preferences->SelectedBooks[index+numSelectedBooks] = Preferences->SelectedNotes[index];
        }

        for (int index=0; index < Preferences->numberOfSelectedCommentaries; index++){
            // copy the notes into the array
            Preferences->SelectedBooks[index+numSelectedBooks+numSelectedNotes] = Preferences->SelectedCommentaries[index];
        }

        if (Preferences->showBookMarkList) {
            Preferences->showBookMarkList = NO;
            BuildPageForBookMark(bookMarkTopic,Preferences->SelectedBooks);
        } else  {
            extern QString CurrentWebPage;
            //extern QString BuildPageForBookChapterVerse(BookInfoStructType *BooksToUse, int Book, int Chapter, int Verse, BibleBookInfoStructType *bis);

            CurrentWebPage = BuildPageForBookChapterVerse(Preferences->SelectedBooks,book, chapter, verse, &bbis) ;
            CurrentWebPage = myReplace(CurrentWebPage,"%3a",HTML_COLON_F);
           //wsm fix navigation CurrentWebPage = myReplace(CurrentWebPage,"+","_");
           // CurrentWebPage.replace(QRegExp("//*\+"))

            // the following are mainly for debug and can be removed later
            CurrentWebPage = myReplace(CurrentWebPage,"</tr>","</tr>\n\r");
            CurrentWebPage = myReplace(CurrentWebPage,"function","\n\rfunction");
        }
    }


    //int rslt = [CurrentWebPage retainCount];
    //[CurrentWebPage release];
    // [bibleWebView loadHTMLString:CurrentWebPage baseURL:[NSURL URLWithString:"/"]];
    //  CurrentWebPage = nil;
    QString  vp;

    if (doShowHelp || StringsAreEqual(EMPTY_PAGE_STRING, CurrentWebPage)) {
        vp= ReadHelpFileString();
        // fixup file references
        QString helpFilePath = "file:///"+ getFullPathOfAppBundleCWPDirectory() + "/";
        vp = myReplace(vp,"&@&",helpFilePath);

        webView->setHtml(vp,QUrl::fromLocalFile("/"));
        doShowHelp=false;
    }
    else {

        vp = myReplace(CurrentWebPage,"&@&",imagePath);

        if ((numSelectedBooks+ numSelectedNotes+
              Preferences->numberOfSelectedCommentaries) == 1  ) {
            if (!Instr(vp,"--ABP_TYPE--"))
                vp = myReplace(vp,"table.sample {", "table.sample { white-space: pre-wrap; ");
        }


        //vp=CurrentWebPage;
        webView->setHtml(vp, QUrl::fromLocalFile("/"));
#define PRINT_WEB_PAGE_TO_FILE 0
#if PRINT_WEB_PAGE_TO_FILE
        QFile file("/Users/maxfield/out.html");
        file.open(QIODevice::WriteOnly| QIODevice::Text);
        QTextStream out(&file);
        out <<vp;
        file.close();
#endif
        //webView->load(QUrl("http://www.google.com/"));
    }

    globalSTI.HilightSearch = NO;
    this->setWindowTitle(BibleBook(book )+QString(" ")+QString::number(chapter));//+QString(":")+QString::number(verse));

    //---------------------------------------------------------
    // if debug html is set, show the html string for every page
    if (Preferences->getDebug() & DEBUG_HTML){
        if (!nwTimer)
            nwTimer = new TimerClass(50,YES,(void*)handleDebugWebWindowShow);
        else
            nwTimer->start(50);
        nwTimer->userString = vp;
    }

#ifdef WEBKIT_USED
   refreshVerses=YES;
   //refreshWebPage=YES;
#endif
}
#ifdef WEBKIT_USED
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::myDownloadRequested(QNetworkRequest r) {
   // urlChanged++;
    Q_UNUSED(r);
}
#endif

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionBibleBooks_triggered()
{

    sbd = new SelectBiblesDialog(nullptr);

    sbd->hide();
    sbd->show();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::on_actionSelect_Bibles_triggered()
{
    sbd = new SelectBiblesDialog(nullptr);// delete after close
    sbd->hide();
    sbd->show();
}
// ------------------------------------------------------------------------
// before quitting, save the main window position on the screen
// ------------------------------------------------------------------------
void MainWindow::on_actionExit_triggered()
{
    // save the MainWindow geometry
    // see also MainWindow::event()
    {
        QRect r = geometry();
        QString s = QString::number(r.x()) + ","+QString::number(r.y()) + "," + QString::number(r.width()) +","+QString::number(r.height());
        Preferences->setMainWindowGeometry(s);
    }
    qApp->quit();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::on_actionBookMark_triggered()
{
    if (!bmvPtr)
        bmvPtr = new DialogBookMarkViewer();

    bmvPtr->hide();
    bmvPtr->show();


}
#ifndef exception_behavior_array_t
typedef int exception_behavior_t;
typedef exception_behavior_t *exception_behavior_array_t;
#endif
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionHistory_triggered()
{
    if (!historyWindowPointer)
            historyWindowPointer   =new DialogHistory();

    try {
        historyWindowPointer->hide();
        historyWindowPointer->show();
    }catch (exception_behavior_array_t e){
        //ignore
        Q_UNUSED(e)
    }

}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionLeft_triggered()
{
    //
    //int BooksToUse[1];
    //BooksToUse[0]=0;

    //= scrollOffset = wf->evaluateJavaScript("window.pageYOffset").toInt();
#ifdef WEBKIT_USED
    setCurrentVerseByScrollPosition();
#endif
#ifdef WEBENGINE_USED
    // we can't update scroll position, page is about to go away, and the javascript won't return
#endif
    userHistory->pushHistoryItemWithBook(Preferences->getCurrentBook(),Preferences->getCurrentChapter(),Preferences->getCurrentVerse(),currentScrollPosition);
    saveCurrentLocation(WRITE_LOCATION_TO_DISK);


    chapter =decrementChapter(Preferences->getCurrentBook(),chapter);
    Preferences->setCurrentChapter( chapter);
    Preferences->setCurrentVerse (1) ;// we go to the first verse on the page

    //this->setWindowTitle(BibleBook(book )+QString(" ")+QString::number(chapter));//+QString(":")+QString::number(verse));
    // ▼
   //
    LoadWebPage();
    doScrollToVerseAfterWebPageLoad=YES;
    verseToScrollToAfterWebPageLoad=1;

    if (historyWindowPointer)
        on_actionHistory_triggered();
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionRight_triggered()
{
#ifdef WEBKIT_USED
    setCurrentVerseByScrollPosition();
#endif
#ifdef WEBENGINE_USED
    // we can't ask for current scroll position, the web page will vanish before javascript executes
#endif
    saveCurrentLocation(WRITE_LOCATION_TO_DISK);
    userHistory->pushHistoryItemWithBook(Preferences->getCurrentBook(),Preferences->getCurrentChapter(),Preferences->getCurrentVerse(),currentScrollPosition);


    chapter = incrementChapter(Preferences->getCurrentBook() , chapter );
    Preferences->setCurrentChapter( chapter);
    Preferences->setCurrentVerse (1) ;// we go to the first verse on the page
    //Preferences->currentPosition =0; // don't pay attention to
    //this->setWindowTitle(BibleBook(book )+QString(" ")+QString::number(chapter));//+QString(":")+QString::number(verse));
    // ▼
   //
    LoadWebPage();
    doScrollToVerseAfterWebPageLoad=YES;
    verseToScrollToAfterWebPageLoad=1;
    if (historyWindowPointer)
        on_actionHistory_triggered();
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionSearch_2_triggered()
{
    if (!dswp) {
        dswp = new DialogSearch();
    }
    dswp->hide();
    dswp->show();
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//void MainWindow::on_actionTopic_triggered()
//{
    // show bookmark topics
//}

#include "dialogselectbookinbible.h"

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionBooksInBible_triggered()
{
    //
    DialogSelectBookInBible *dp = new DialogSelectBookInBible(this);
    dp->show();
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionPreferences_triggered()
{
    //
    DialogPreferences *dpp = new DialogPreferences(this);
    dpp->show();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::on_pushButton_clicked()
{
    if (activityIndicator->isAnimating())
        activityIndicator->stopAnimation();
    else
        activityIndicator->animate();
}


//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::processLink() {

    webTimer.stop();

    QString  lchapter="1";
    QString  lverse="1";
    QString  BibleNumber;
    QString  BibleType;
    QString BOOKNumber ;

checkUrlString:
        if (Instr(urlString,"_"))
        {
            if (Instr(urlString,"http://") || Instr(urlString, "file://")) {
                urlString = Right(urlString, 7);// remove http://
                while (Mid(urlString,0,1)=="/") {
                    urlString = Right(urlString,1);
                }

                while (Mid(urlString,urlString.length()-1,1)=="/")
                   urlString = urlString.left(urlString.length()-1);
            }

            QString Number;
            QStringList array;
            array = mySplit(urlString,"_");
            if (array.count()) {

                QChar c=0;

                Number = array[0];
                if (array[0].length())
                    c = Number.at(0).toUpper().toLatin1();

                if (c == 'X') {
                    if (Number.at(1) == 'n') {
                        Number = myReplace(Number,"xn--","");
                        Number = Number.toUpper();

                        array[0]=Number;
                        c = Number.at(0);
                    }
                }

                switch (c.toLatin1()) {

                    case 'A':// morphology
                    morphology:
                    {
                        extern QString handleMorphology(QString Number);
                        Number = array[1];
                        Number = myReplace(Number,"%5B","");
                        Number = myReplace(Number,"%5D","");
                        Number = myReplace(Number,"[","");
                        Number = myReplace(Number,"]","");
                        if (array.count()>2) {
                            handleElementID(array[2]);
                        }
                        //MsgBox(handleMorphology(Number));
                        Number = handleMorphology(Number);
                        showMorphology(Number);
                    }   break;

                    case 'D':
                    EnglishDictionary:
                        // english dictionary
                        if (array.count()>1) {
                            //[self startWaitIndicator];

                            dictionarySearchWord = array[1];
                            showEnglishDictionary();
                            handleElementID(array[2]);

                            return ;
                        }
                        break;

                    case 'H': //strong's
                    case 'G':
                        Number = Right(array[0] ,1);
                        if (array.count()>1) {
                            handleElementID(array[1]);
                        }
                        if (array.count()>2) {

                            dictionarySearchWord = myReplace(array[2],"zzz"," ");
                            if (array[2]=="asterisk") {
                                array[2]="*";
                            }
                        } else {
                            array.append("*");
                        }

                        showStrongsWithNumber(Number, /* Language:*/QString(c.toUpper().toLatin1()), /* Word:*/array[2]);

                        break;

                    case EDIT_INDICATOR_DIRECT_C:{
                        if (array.count()>3) {
                            handleElementID(array[3]);
                        }
                        int indexIntoSelectedBooksArray = Right(array[0], 1).toInt();
                        Number = array[1];
                        array = mySplit(array[2], HTML_COLON_F);
                        if (array.count() >1) {
                            lchapter = array[0];
                            lverse = array[1];
                            dismissCounter=0;
                            showVerseEditForIndex(indexIntoSelectedBooksArray,/* Book:*/Number, /*Chapter:*/lchapter, /*Verse:*/lverse);
                        }
                       } break;

                    case BOOKMARK_INDICATOR_DIRECT_C: // do a bookmark
                        if (array.count()>2) {
                            handleElementID(array[2]);
                        }
                        BOOKNumber= array[0];

                        array = mySplit(array[1], HTML_COLON_F);
                        if (array.count() >1) {
                            lchapter = array[0];
                            lverse = array[1];
                            BOOKNumber = BOOKNumber.right(BOOKNumber.length()-1);
                            showBookMarkAdd(BOOKNumber, /*Chapter:*/lchapter, /*Verse:*/lverse);
                        }
                        break;

                    case FOOTNOTE_INDICATOR_C : {// show footnote
                        if (array.count()>4) {
                            handleElementID(array[4]);
                        }

                        array = mySplit(urlString,"-");

                        // check array so we don't crash
                        if (array.count() < 3) {
                            break;
                        }

                        BibleNumber = array[1];
                        urlString  = array[2];

                        array = mySplit(urlString,"_");

                        if (array.count() == 0)// then
                            break;

                        //-------------------------------

                        Number = array[0];
                        lchapter = array[1];
                        lverse = array[2];
                        QString lSeqNumber=array[3];
                        showFootNoteForBible(BibleNumber, /*Book:*/Number, /*Chapter:*/lchapter,/* Verse:*/lverse,/* SeqNumber:*/lSeqNumber);
                      }  break;

                    case COPYRIGHT_INDICATOR_SHOW_WINDOW_C: // copyright info was clicked
                                                            // check array so we don't crash
                        if (array.count() < 3) {
                            break;
                        }

                        BibleNumber = array[1];
                        BibleType = array[2];

                        if (array.count()>3) {
                            handleElementID(array[3]);
                        }


                        showCopyrightForBookNumber(BibleNumber, /*ofType:*/BibleType);
                        break;

                    case NOTES_INDICATOR_DIRECT_C: //NOTES_INDICATOR_DIRECT:
                    {
                        QString noteIndex;

                        Number = Right(array[0] ,1);
                        if (array.count()>2) {
                            handleElementID(array[2]);
                        }

                        if (array.count()>3) {
                            noteIndex = array[3];
                        } else {
                            noteIndex = "0";
                        }

                        array = mySplit(array[1], HTML_COLON_F);
                        if (array.count()>1) {
                            lchapter = array[0];
                            lverse = array[1];
                            showNotesWindowForNoteIndex(noteIndex.toInt(),/*Book:*/Number.toInt(),/* Chapter:*/lchapter.toInt(),/* Verse:*/lverse.toInt());
                        }
                    }   break;


                    case SPEECH_INDICATOR_DIRECT_C:
                    {
                        QStringList   array2 = mySplit(array[0], "-");
                        if (array2.count()>3) {
                            Number = array2[3];
                            lverse = array[2];
                            lchapter = array[1];

                            //speakFromBible(array2[1].toInt(),/* Book:*/Number.toInt(), /*Chapter:*/lchapter.toInt(),/* Verse:*/lverse.toInt());
                            //     [self showNotesWindowForNoteIndex:[noteIndex intValue] Book:Number.toInt() Chapter:lchapter.toInt() Verse:lverse.toInt()];
                        }
                    }   break;

                  /*  case 'Z': // current Y location of the screen (scroll offset)
                    {
                        // check array so we don't crash
                        if (array.count() < 2) {
                            break;
                        }

                        Number=  array[1];
                        currentScrollPosition = Number.toInt();
                        setCurrentVerseByScrollPosition(currentScrollPosition);
                    }
                        break; */
                case CHAPTER_XREF_C:

                    // if there are '3' C's in a row (CCC) then
                    // is a copy command
                    if (Number.contains(COPY_INDICATOR_DIRECT_SWITCH_VAR_STR COPY_INDICATOR_DIRECT_SWITCH_VAR_STR COPY_INDICATOR_DIRECT_SWITCH_VAR_STR)){
                            //case COPY_INDICATOR_DIRECT_SWITCH_VAR:

                        Number = array[1];
                        lchapter= array[2];
                        lverse= array[3];
                        QString BibleName="";
                        // show a dialog here
                        QStringList   array2 = mySplit(array[0], "-");
                        int count = array2.count();
                        if (count<4) {
                            BibleNumber = array2[1];// the index into the bible
                            BibleName = array2[2];

                        }else {
                            break;// fail silently
                        }

                        showCopyDialog(Number,lchapter,lverse,BibleNumber,BibleName);
                        break;
                    }

                    // else -- is a chapter xref
                    // continue on with older code
                        int n;
                        Number = Right(array[0] ,1);
                        n = Number.toInt();
                        if (n== 0) {
processBookName:
                            extern BibleBookInfoStructType getBibleBookInfoByName(QString b);
                            BibleBookInfoStructType bis =  getBibleBookInfoByName(Number);
                            Number = QString::number(bis.BookNumber);
                            goto processChapterXRefC;
                        } else {
                            if ((n==1 || n==2 || n == 3)&& array.count()==3 ){
                                Number = Number + " "+array[1];
                                array.removeFirst(); // pull off the book #, i.e. 1 Corinthians
                                goto processBookName;
                            }
                        }
#if FALLTHROUGH_ALLOWED==1
[[clang::fallthrough]];
#endif
                    case USER_XREF_C://'U' // a reference in a user's notes
                        Number = Right(array[0] ,1);
processChapterXRefC:
                        array = mySplit(array[1], HTML_COLON_F);
                        if (array.count()>1) {
                            lchapter = array[0];
                            lverse = array[1];
                            // sometimes verses have continuations in them. remove the continuation and look for the first verse
                            if (Instr(lverse,"-")) {
                                array = mySplit(array[1],"-");
                                lverse = array[0];
                            }

            // we can't get current scroll position, page will vanish before script executed
                            int16_t currentVerse = getVerseFromScrollPosition();// uses memory lookup
                            userHistory->pushHistoryItemWithBook(Preferences->getCurrentBook(),Preferences->getCurrentChapter(),currentVerse,currentScrollPosition);
                            saveCurrentLocation( DONT_WRITE_LOCATION_TO_DISK );

                            Preferences->setCurrentBook( Number.toInt());
                            Preferences->setCurrentChapter(lchapter.toInt());
                            Preferences->setCurrentVerse( lverse.toInt());
                            // make sure we scroll to the exact verse
                            globalSTI.DoScrollToVerse = YES;
                            globalSTI.VerseToScrollTo = Preferences->getCurrentVerse();
                            doScrollToVerseAfterWebPageLoad=YES;
                            verseToScrollToAfterWebPageLoad=Preferences->getCurrentVerse();

                            LoadWebPage();
                        }
                        break;

                    case '/':
                    {
                        c = array[0].at(1);

                        if (c == 'A') {
                            goto morphology;
                        }

                        if (c == 'D') {
                            goto EnglishDictionary;
                        }


                        if (c == CHAPTER_XREF_C) {
                            extern BibleBookInfoStructType getBibleBookInfoByName(QString b);

                            QString bookName;
                            bookName = Right(array[0], 2);

                            if (bookName.toInt()>0) {
                                if (bookName.length()==1) {
                                    bookName = bookName+array[1];
                                    array.removeAt(1);// we don't need this one any more
                                }
                            }

                            // this is a vector using a short book name
                            BibleBookInfoStructType bis =  getBibleBookInfoByName(bookName);
                            // Preferences.currentBook = bis.BookNumber;

                            // if the booknumber is -1, then this might be a problem with the
                            // offset into the array.  let's try with the next item in the
                            // array, if the array count is > 1
                            if (bis.BookNumber == -1) {
                                if (array.count()>1) {
                                    bookName = array[1];
                                    bis = getBibleBookInfoByName(bookName);
                                    if (bis.BookNumber==-1) {
                                        break; // we could not figure it out, don't do anything
                                    }
                                }
                            }

                            if (array.count()>1) {

                                if (array.count()== 4) {
                                    array = mySplit(array[3],HTML_COLON_F);
                                } else {
                                    array = mySplit(array[1], HTML_COLON_F);
                                }

                                if (array.count()>1) {
                                    lchapter = array[0];
                                    lverse = array[1];
#ifdef WEBKIT_USED
                                    setCurrentVerseByScrollPosition();
#endif
#ifdef WEBENGINE_USED
        // we can't ask for scroll position, page will vanish before script executes
#endif
                                    userHistory->pushHistoryItemWithBook(Preferences->getCurrentBook(),Preferences->getCurrentChapter(),Preferences->getCurrentVerse(),currentScrollPosition);
                                    saveCurrentLocation(DONT_WRITE_LOCATION_TO_DISK );

                                    Preferences->setCurrentChapter( lchapter.toInt());
                                    Preferences->setCurrentBook(bis.BookNumber);
                                    Preferences->setCurrentVerse( lverse.toInt());
                                    // make sure we scroll to the exact verse
                                    globalSTI.DoScrollToVerse = YES;
                                    globalSTI.VerseToScrollTo = Preferences->getCurrentVerse();
                                }
                                LoadWebPage();
                            }
                        }
                    }   break;

                    default:
                        break;
                }

            }

          return;  //not ok
        } else {
            // see if this is a previous version's LINK to another book/chapter
            if (Instr(urlString,"+")) {
                urlString = myReplace(urlString,"+","_");
                goto checkUrlString;
            }
        }


        return ;//ok


}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::saveCurrentLocation(BOOL WriteToPrefs)
{
    Q_UNUSED(WriteToPrefs);

    static int lastBook,lastVerse,lastChapter,lastScrollResult;
    setCurrentVerseByScrollPosition();

    if (lastBook != Preferences->getCurrentBook()) {
        goto update;
    }

    if (lastChapter != Preferences->getCurrentChapter()) {
        goto update;
    }

    if (lastVerse != Preferences->getCurrentVerse()) {
        goto update;
    }

    if (lastScrollResult != currentScrollPosition) {
        goto update;
    }


    return;

update:

        lastBook = Preferences->getCurrentBook();

        lastChapter = Preferences->getCurrentChapter();

        lastVerse = Preferences->getCurrentVerse();

    if (lastScrollResult != currentScrollPosition) {
        lastScrollResult = currentScrollPosition;
    }


    if (!WriteToPrefs) {
        return;
    }
    //----------------------------------------------------------------------
    // The following code was added to allow you to "remember" the current
    // location of yourchapter,verse so when you navigate back to this book, it will
    // be at the same location it was
    //----------------------------------------------------------------------
    QString key = QString(INI_BOOK_LOC)+QString::number( Preferences->getCurrentBook());
    QString value =QString::number(Preferences->getCurrentChapter())+QString(BOOK_LOC_SEPARATOR)+QString::number(Preferences->getCurrentVerse())+ BOOK_LOC_SEPARATOR "0";// storage on disk ignores scroll position
    SavePrefVariable(key ,value);
}

//------------------------------------------------------------
//------------------------------------------------------------
int16_t MainWindow::getVerseFromScrollPosition(){
    int CurrentPageOffset=0;

    int16_t cb =Preferences->getCurrentBook();
    if (cb <1 || cb > 66)
        return 0;

    BookChapterVerseStructType bcv;
    GetBookChapterVerse(cb,bcv);

    int maxVerse = bcv.NumberOfVerses[Preferences->getCurrentChapter()];

    if (maxVerse > MAX_POSSIBLE_VERSE_NUM)
        return 0;

    CurrentPageOffset = currentScrollPosition; // use passed-in value
                                        // CurrentPageOffset=currentScrollPosition; //[self getPageScrollOffset]; // where the webview page has scrolled to

    if (CurrentPageOffset<=10) {
        return 1;
    }

    do {
        if (CurrentPageOffset >= verseYOffsets[maxVerse]) {
            break;
        }
    } while (--maxVerse);

    // find out which verse is closest, and point to that one.
    if (maxVerse>1){
        int16_t o1=-1, o2=-1, o3=-1,mv=0;
        o1=abs(CurrentPageOffset-verseYOffsets[maxVerse]) ;
        o2=abs(CurrentPageOffset-verseYOffsets[maxVerse-1]);
        o3=abs(CurrentPageOffset-verseYOffsets[maxVerse+1]);
        mv=maxVerse;
        if (o1 > o2)
            mv=maxVerse-1;
        if (o1 > o3)
            mv=maxVerse+1;

        maxVerse=mv;
    }

    if (maxVerse <=0) {
        maxVerse=1;
    }

   return maxVerse;
}


//------------------------------------------------------------
// update the current verse
//------------------------------------------------------------
void MainWindow::setCurrentVerseByScrollPosition()  {


    if (activityIndicator && activityIndicator->isAnimating()) {
        return;
    }

    Preferences->setCurrentVerse(getVerseFromScrollPosition());
}


#if 0
//------------------------------------------------------------
// update the current verse
//------------------------------------------------------------
void MainWindow::setCurrentVerseByScrollPosition(int scrollPosition) {

    int CurrentPageOffset=0;

    BookChapterVerseStructType bcv;

     GetBookChapterVerse(Preferences->getCurrentBook(),bcv);


    if (activityIndicator && activityIndicator->isAnimating()) {
        return;
    }

    int maxVerse = bcv.NumberOfVerses[Preferences->getCurrentChapter()];

    CurrentPageOffset = scrollPosition; // use passed-in value
                                        // CurrentPageOffset=currentScrollPosition; //[self getPageScrollOffset]; // where the webview page has scrolled to

    if (CurrentPageOffset<=10) {
        Preferences->setCurrentVerse(1);
        return;
    }

    do {
        if (CurrentPageOffset >= verseYOffsets[maxVerse]) {
            break;
        }
    } while (--maxVerse);

    //maxVerse++; // correct for Qt pecularities
    if (maxVerse <=0) {
        maxVerse=1;
    }

    Preferences->setCurrentVerse(maxVerse);

}
#endif
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::showNotesWindowForNoteIndex(int noteIndex ,/*   Book*/ int lnumber ,/*Chapter*/ int lchapter,/* Verse*/ int lverse)
{
    Q_UNUSED(noteIndex);
    Q_UNUSED(lnumber);
    Q_UNUSED(lchapter);
    Q_UNUSED(lverse);

    if (!notesWindow)
        notesWindow = new NotesWindow();

    if (notesWindow) {
        notesWindow->chapter = lchapter;
        notesWindow->bookNumber = lnumber;
        notesWindow->verse = lverse;
        notesWindow->noteIndex = noteIndex;
        notesWindow->show();
    }

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::showCopyrightForBookNumber( QString BibleNumber, /* ofType:*/ QString Type)
{
    Q_UNUSED(BibleNumber);
    Q_UNUSED(Type);

    if (!copyrightWindow) {
        copyrightWindow = new DialogCopyright();
    }

#if 1
    // handle book, chapter verse
    if (copyrightWindow) {
        copyrightWindow->bookNumber = BibleNumber.toInt();
        copyrightWindow->bookType = Type.toInt();

        copyrightWindow->show();
    }

#endif
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::showFootNoteForBible( QString BibleNumber, /* Book:*/ QString Number , /*Chapter:*/ QString Chapter , /*Verse:*/ QString Verse , /*SeqNumber:*/ QString SeqNumber)
{
    Q_UNUSED(BibleNumber)
    Q_UNUSED(Number)
    Q_UNUSED(Chapter)
    Q_UNUSED(Verse)
    Q_UNUSED(SeqNumber)


        {/*localize variables */
            // ------------------------------------------------------------------------
            // Create and configure the recent searches controller.
            // ------------------------------------------------------------------------
            DialogFootNote *dfnp = new DialogFootNote(this);


            // handle book, chapter verse
            if (dfnp) {
                dfnp->bibleNumber =BibleNumber.toInt();
                dfnp->bookNumber = Number.toInt();
                dfnp->chapter = Chapter.toInt();
                dfnp->verse = Verse.toInt();
                dfnp->sequenceNumber= SeqNumber.toInt();
            }
            dfnp->isBook=false;
            dfnp->show();
        }

}
#ifdef WEBKIT_USED
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int MainWindow::scrollToCurrentVerse() {
    int y;
    QString js;

    int16_t v = Preferences->getCurrentVerse();
        //y = getVerseYOffset("v"+QString::number(v));
        y = getVerseYOffset(v);
        y -=4;

        if (y < 0 && v>1){
            //js = "window.location.href=f"+QString::number(v);
           // webView->page()->currentFrame()->evaluateJavaScript(js);
          //  return 0;

        }

        // attempt to remember where we were on the page.
        if (Preferences->currentPosition > 0)
            if (Preferences->currentPosition < y) {
                y = Preferences->currentPosition;
                Preferences->currentPosition=0;
            }

        js= "scrollTo(0,"+QString::number(y)+");";
        webView->page()->currentFrame()->evaluateJavaScript(js);
        return 0;
}
#endif

int MainWindow::calculatePositionOfVerse(int VerseNumber){
    return getVerseYOffset(VerseNumber);
}
// ------------------------------------------------------------------------
// have the java script on the page scroll to the current verse
// ------------------------------------------------------------------------
int MainWindow::scrollToVerse(int16_t verse) {
    int y;
    QString js;



    y = getVerseYOffset(verse)-4;

    if (y<0)
        y=0;



    js= "scrollTo(0,"+QString::number(y)+");";
#ifdef WEBKIT_USED
    webView->page()->currentFrame()->evaluateJavaScript(js);
#endif
#ifdef WEBENGINE_USED
    webView->page()->runJavaScript(js);
#endif
        return 0;
}
#ifdef WEBENGINE_USED
// ------------------------------------------------------------------------
// have the java script on the page scroll to the current verse
// ------------------------------------------------------------------------
int MainWindow::scrollToCurrentVerse() {
    int y;
    QString js;

    int v = Preferences->getCurrentVerse();

    y = getVerseYOffset(v)-4;

    if (y<0)
        y=0;

    // attempt to remember where we were on the page.
    if (Preferences->currentPosition > 0)
        if (Preferences->currentPosition < y) {
            y = Preferences->currentPosition;
            Preferences->currentPosition=0;
        }

    js= "scrollTo(0,"+QString::number(y)+");";
#ifdef WEBKIT_USED
    webView->page()->currentFrame()->evaluateJavaScript(js);
#endif
#ifdef WEBENGINE_USED
    webView->page()->runJavaScript(js);
#endif
        return 0;
}

#endif


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::showBookMarkAdd(QString bookNumber,/* Chapter:*/ QString lchapter ,/*Verse:*/ QString lverse)
{
    Q_UNUSED(bookNumber);
    Q_UNUSED(lchapter);
    Q_UNUSED(lverse);
   // Q_UNUSED();

    bmh = new BookMarkHandler(0,bookNumber.toInt(),lchapter.toInt(),lverse.toInt(),NO);


    // using a timer fixes the bug where the macintosh menu stayed selected.
    if (!bmhTimer)
        bmhTimer = new TimerClass(50,YES,(void*)handleBookMarkWindow);
    else
        bmhTimer->start(50);


}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::showVerseEditForIndex(int indexIntoSelectedBooksArray,/*  Book:*/ QString bibleBookNumber ,/*Chapter:*/ QString lchapter,/* Verse:*/ QString lverse)
{
    Q_UNUSED(indexIntoSelectedBooksArray);
    Q_UNUSED(bibleBookNumber);
    Q_UNUSED(lchapter);
    Q_UNUSED(lverse);
    //int doHide=true;


    BookInfoStructType    bis= Preferences->SelectedBooks[indexIntoSelectedBooksArray];


    devPtr= new DialogEditVerse(bibleBookNumber.toInt(),lchapter.toInt(),lverse.toInt(),
                              bis.Index,bis.Name);
    devPtr->show();

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::showStrongsWithNumber(QString StrongsNumber, /*Language:*/QString L,/* Word:*/QString word)
{

   DialogShowStrongs *strongsView = new DialogShowStrongs(nullptr);

    strongsView->Language = L;

    if (Instr(StrongsNumber,".")) {
        QStringList a;
        a = mySplit(StrongsNumber,".");
        StrongsNumber = a[0];
    }

    strongsView->strongsNumber = StrongsNumber.toInt();
    strongsView->textInBible = word;
    strongsView->setModal(NO);
    strongsView->show();

}



// ------------------------------------------------------------------------
// show the english dictionary definition of the word
// ------------------------------------------------------------------------
void MainWindow::showEnglishDictionary(){
    DialogEnglishDictionary *dedp = new DialogEnglishDictionary(nullptr);
// set the dictionary word to look up here
    dedp->mainwindow = this;// so we can set its focus
    dedp->calledFromStrongs = YES;
    dedp->dictionarySearchWord = dictionarySearchWord;// pick up from global
    dedp->setModal(YES);
    dedp->show();

}
static CopyDialog *copyDialog;
// ------------------------------------------------------------------------
// let the user choose to copy 1 or more verses
// ------------------------------------------------------------------------
void MainWindow:: showCopyDialog(QString BookNumber,QString Chapter,QString Verse,QString BibleNumber,QString BibleName) {

    if(!copyDialog){
        copyDialog=new CopyDialog(this);
    }

    copyDialog->setParams(BookNumber.toInt(),Chapter.toInt(),Verse.toInt(),BibleNumber.toInt(),BibleName);
    copyDialog->show();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::showMorphology(QString MorphologyText)
{
    QPoint p = QCursor::pos();
    p.setY(p.y()-20);// ugh.  no good way for this according to slashdot
    p.setX(p.x()+20);

        QToolTip::showText( p , MorphologyText );
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::startWaitIndicator() {
    // start the wait indicator if needed
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void MainWindow::timerCheck()
{
    if (refreshVerses) {
        refreshVerses = NO;
        updateVerseYOffsets();
    }

    if (refreshWebPage) {
        refreshWebPage=NO;
        //activityIndicator->animate();
        //miscTimer->stop();
        LoadWebPage();
        //miscTimer->start(150);
    }
#if WEBKIT_USED
//    if (doScrollToCurrentVerse){
//        if(scrollToCurrentVerse()==0)
//            doScrollToCurrentVerse=NO;
//    }
#endif
#if WEBENGINE_USED


    static int16_t scrollPositionCounter;
    if (++scrollPositionCounter>3){
        scrollPositionCounter=0;
        getPageScrollPosition(); // run asynchronous code

    }
    static int16_t initial_set_delay=10;
    if (initial_set_delay){
        initial_set_delay--;
    }

    if (!initial_set_delay)
        setCurrentVerseByScrollPosition();
#endif

}



//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::webPageLoadStarted(){
    loadStarted++;
#ifdef WEBKIT_USED
   static QUrl newUrl= webView->url();
   QString s ;//= newUrl.toString();

   QWebPage *p = webView->page();
   QWebFrame *wf= p->currentFrame();

   s = wf->baseUrl().toString();
  // s = wf->url().toString();
   if (s== QString("about:blank")) {
       // this "stop" worked in this location (webPageLoadStarted)
      //webView->stop();
      return;
   }
#endif
}

//-------------------------------------------------------------------------------------------------------------
// web page complete
//-------------------------------------------------------------------------------------------------------------
void MainWindow::webPageFinishedLoading(bool b) {
    Q_UNUSED(b);

     urlChanged++;
#if WEBKIT_USED
     if (globalSTI.DoScrollToVerse){
         scrollToCurrentVerse();
         globalSTI.DoScrollToVerse=NO;
     }
     //updateVerseYOffsets();
#endif
#if WEBENGINE_USED
     if (globalSTI.DoScrollToVerse){
         doScrollToVerseAfterWebPageLoad=YES; // force after updateVerseYOffsets
         verseToScrollToAfterWebPageLoad=globalSTI.VerseToScrollTo;
         globalSTI.DoScrollToVerse=NO;
     }
     updateVerseYOffsets();// this is an asynchronous call now
#endif
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::webPageURLChanged(const QUrl &url) {

#ifdef WEBKIT_USED
    //
    QString s = url.toString();
    QWebPage *p = webView->page();
    QWebFrame *wf= p->currentFrame();

    s = wf->url().toString();

// we get the "correct" url here
    if (s== QString("about:blank")) {
       return;
    }
#endif
    Q_UNUSED(url)
        urlChanged++;
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionBible_Dictionary_triggered()
{
    // show the bible dictionary window
    if (!bibleDictionaryWindowPtr) {
        bibleDictionaryWindowPtr = new BibleDictionaryDialog(nullptr);
    }

    bibleDictionaryWindowPtr->hide();// make this pop back up to top
    bibleDictionaryWindowPtr->show();

}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionTagalog_triggered()
{
    if (!tagalogDialogPtr){
        tagalogDialogPtr=new TagalogDialog();
    }
    tagalogDialogPtr->show();
}

#include "../dialogs/about/dialogabout.h"
static DialogAbout *d;
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionAbout_triggered()
{
    if (!d)
        d = new DialogAbout();

    d->show();

}
static TranslationTextFileDialog *ttd;
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionCreate_translation_template_triggered()
{
    if(!ttd)
        ttd = new TranslationTextFileDialog();

    ttd->show();
}


void MainWindow::on_actionShow_Help_triggered()
{
    doShowHelp=true;
    refreshWebPage=true;
    MsgBox("To get back to Bibles, press either the Previous or Next Chapter button.");

}
