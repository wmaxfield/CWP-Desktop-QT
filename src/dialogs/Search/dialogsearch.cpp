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
#include "dialogsearch.h"
#include "ui_dialogsearch.h"
#include <QLabel>

#include "chapterversestructclass.h"
#include "mainwindow.h"
//#include "TimedSearchRoutine.h"
//#include "speechDefines.h"
#include "cwpdefines.h"
#include "indexpath.h"
#include "mainwindow.h"
#include "preferencesmodule.h"
#include "dialoghistory.h"
#include "userhistoryclass.h"
#include "dialogpreferences.h"
#include "../shared/bibleinfo.h"


DialogSearch *dswp;


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogSearch::DialogSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSearch)
{
    ui->setupUi(this);
    mainwindowWidth = this->width();
    mainwindowHeight = this->height();

    ui->listWidget->setStyleSheet("QListWidget::item { border-bottom: 1px solid black; border-left: 5px solid white; border-right: 5px solid white; border-top: 5px solid white; } ");

    this->setWindowTitle("Search");
    dswp = this;

    QString RecentSearchesKey = "RecentSearchesKey";

    searchTimer.setInterval(50);// was 100
    connect(&searchTimer,SIGNAL(timeout()),SLOT(performSearch()));

    searchInfo.SearchForStrongs = NO;


    timedSearch =new TimedSearchRoutineClass();
    timedSearch->timerExpired = NO;

    //ui->labelSearching->setVisible(NO);
    ui->progressBar->setVisible(NO);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogSearch::~DialogSearch()
{
    if (timedSearch)
        delete timedSearch;

    delete ui;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSearch::showEvent(QShowEvent *e) {
    Q_UNUSED(e);

    reloadData();
    ui->lineEdit->setFocus();

    // show at same position relative mainwindow
    QRect r;
    QRect mwr=fvcp->geometry();

    r.setX(mwr.x()+mwr.width()/2-width()/2);
    r.setY(mwr.y()+mwr.height()/3);
    move(r.x(),r.y());
}
//------------------------------------------------------------
// overrides resizeEvent to allow button movement
//------------------------------------------------------------
void DialogSearch::resizeEvent ( QResizeEvent * event ) {

    Q_UNUSED(event);
    /* QMessageBox msgBox;
       msgBox.setText("Resized");
       msgBox.exec();
       */
    //(void)event;
  // return;
    // move the quit button
   QRect r = ui->listWidget->geometry();
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

    ui->listWidget->setGeometry(r);


    r = ui->pbClose_2->geometry();
    int width = r.width();
    int height = r.height();
    r.setX(r.x()+resizex/2);
    r.setY(r.y()+resizey);
    r.setHeight(height);
    r.setWidth(width);
    ui->pbClose_2->setGeometry(r);


    // newy - 50;
    mainwindowWidth = newx;
    mainwindowHeight = newy;
    //activityIndicator->stopAnimation();

}





// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSearch::reloadData() {
    ui->listWidget->clear();

    int index;
    int Count = resultsArray.count();
    QString t,t2;
    QStringList a;
    ChapterVerseStructType cvs;
    BibleBookInfoStructType info;
    int bblidx;
    BibleDatabaseClass *bbld;

    SEARCH_INFO_TYPE sit;

    bblidx= Preferences->SelectedBooks[0].Index;// always show first book's verses

    if (bblidx < 0) bblidx=0;

    bbld =(BibleDatabaseClass *) ((BookHandlerClass *)Books[bblidx])->Database;

    for (index =0; index < Count ;index++) {

        QListWidgetItem* pItem =new QListWidgetItem();
        pItem->setSizeHint(QSize(0,89));// ??
        ui->listWidget->addItem(pItem);

        QLabel *label = new QLabel(ui->listWidget);

        cvs = resultsArray[index];
        info = getBibleBookInfo(cvs.BookNumber);

        sit.BookNumber = cvs.BookNumber;
        sit.ChapterNumber = cvs.Chapter;
        sit.VerseNumber = cvs.Verse;
        sit.BookTableName= info.TableName;



        t2 = bbld->GetPlainVerse(&globalSTI,&sit,bbld->getDbType());


        t = "<font size=\"+1\" face=\"helvetica\" color=\"dark blue\"><b>&nbsp;&nbsp;&nbsp;&nbsp;";
        t = t +  info.BookName + " "+QString::number(cvs.Chapter)+":"+QString::number(cvs.Verse);
        t = t + "</b></font><br /><br /><font face=\"helvetica\"  color=\"purple\">&nbsp;&nbsp;";
        t = t + t2.left(300);//t2.left(100);

        //This is the text.  ";
        //t = t + "This is the second part of the text how long is it and how many lines of label does it occupy.  Let's make it a little longer....</font>";
        label->setText(t);
        label->setWordWrap(true);
        label->setEnabled(false);
        t = QString::number(cvs.BookNumber)+":"+QString::number(cvs.Chapter)+":"+QString::number(cvs.Verse)+":"+QString::number(cvs.Posn);
        pItem->setData(Qt::UserRole,t);

        ui->listWidget->setItemWidget(pItem,label);

    }
}

//------------------------------------------------------------------
// timer routine
//------------------------------------------------------------------

void  DialogSearch:: performSearch()
{

    if (RestartSearch) {
        RestartSearch = NO;
        searchState = RestartSearchState;
    }

   // ui->labelSearching->setText(ui->labelSearching->text()+">");
    ui->progressBar->setValue(searchInfo.BookNumber);

    switch (searchState) {
        case RestartSearchState:
            countDownToRestartSearch = 1; // 600 milliseconds
            searchState = CountDownToRestartSearchState;
            searchInfo.SearchForAWholeWord=NO;
            searchInfo.SearchForStrongs=NO;
            searchInfo.SearchThisBookOnly=NO;
            searchInfo.SearchForAWord=NO;
            searchInfo.SearchType=0;

            break;

        case CountDownToRestartSearchState:
            if (--countDownToRestartSearch) {
                break;
            }
            searchState = SetupSearchState;
            resultsArray.clear();
            resultsArrayNumItems = 0;
            break;


        case SetupSearchState:
            if (timedSearch) {
                timedSearch->Initialize();
            }

            searchInfo.OT=Preferences->getSearchOT();
            searchInfo.NT=Preferences->getSearchNT();
            searchInfo.SearchThisBookOnly =Preferences->getSearchThisBookOnly();

            if (!searchInfo.OT && !searchInfo.NT && !searchInfo.SearchThisBookOnly) {
                searchInfo.OT = YES;
                searchInfo.NT = YES;
                // set up default preferences
                Preferences->setSearchOT(YES);
                Preferences->setSearchNT(YES);
            }


            iNumBooksSearched=0;

            if (searchInfo.SearchThisBookOnly) {
                intStartBookNumber= searchInfo.BookNumber = Preferences->getCurrentBook();
            }
            else {
                if (searchInfo.OT) {
                    intStartBookNumber=searchInfo.BookNumber=1;
                }else {
                    intStartBookNumber=searchInfo.BookNumber=LAST_BOOK_IN_OT+1;
                }

            }


            ui->progressBar->setMaximum(66);// 66 books in the bible


            searchInfo.isBibleSearch = NO;
            searchInfo.isNoteSearch = NO;
            searchInfo.isCommentarySearch = NO;

            if (Preferences->getNumberOfSelectedBooks()) {
                searchInfo.isBibleSearch = YES;
                searchInfo.indexIntoArray = Preferences->SelectedBooks[0].Index;
            } else {
                if (Preferences->numberOfSelectedCommentaries) {
                    searchInfo.isCommentarySearch = YES;
                    searchInfo.indexIntoArray = Preferences->SelectedCommentaries[0].Index;
                } else {
                    if (Preferences->getNumberOfSelectedNotes()) {
                        searchInfo.isNoteSearch = YES;
                        searchInfo.indexIntoArray = Preferences->SelectedNotes[0].Index;
                    }
                }

            }

            searchState=CheckSearchTextState;
#if FALLTHROUGH_ALLOWED == 1
[[fallthrough]];
#endif
        case CheckSearchTextState:
            if (searchText.length()==0) {
                searchState =SearchCompleteState;
                if (timedSearch){
                    timedSearch->timerExpired=YES;
                    newData=YES;
                    reloadData();
                }
                break;
            }

                if (Instr(searchText, " ")>0) {
                    searchInfo.PhraseSearch = YES;
                    searchInfo.SearchForAWord=NO;
                    searchInfo.PhraseToSearchFor =searchText;
                }
                else {
                    searchInfo.PhraseSearch =NO;
                    searchInfo.SearchForAWord=YES;
                    searchInfo.WordToSearchFor=searchText ;
                }

                globalSIT =searchInfo;


            if (timedSearch) {
                timedSearch->timerExpired = NO;
            }


            searchState = waitOnSearchCompleteState;
            break;

        case waitOnSearchCompleteState:
                if (timedSearch) {
                    timedSearch->timedSearchRoutine(&searchInfo, /* andResultArray:*/&resultsArray, /*andNumItems:*/&resultsArrayNumItems );

                    iNumBooksSearched++;

                    if ( timedSearch->SearchIsComplete) {
                        searchState = SearchCompleteState;
                        intFinishedBookNumber = searchInfo.BookNumber;
                    }

                    if (timedSearch->newData){
                        timedSearch->newData=NO;
                        // wsm, moved below 2 lines here.  maybe not?
                        // maybe always reload?
                        newData=YES;
                        reloadData();
                    }

                }
        break;

        case SearchCompleteState:
            // clean up here if needed
            //ui->labelSearching->setVisible(NO);
            ui->progressBar->setVisible(NO);
            ui->pbClear->setVisible(YES);
            ui->pbSearch->setVisible(YES);
            ui->pbSettings->setVisible(YES);
            searchTimer.stop();
            ui->labelFound->setText("Found "+ui->lineEdit->text()+" in "+QString::number(resultsArrayNumItems)+" verses.");
            if (intFinishedBookNumber < intStartBookNumber)
                intFinishedBookNumber = 66;
            ui->labelinBooks->setText("From "+getBibleBookInfo(intStartBookNumber).BookName+" to "+getBibleBookInfo(intFinishedBookNumber).BookName);
            reloadData();
            break;

        default:
            break;
    }


}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSearch::on_pbClose_2_clicked()
{
        hide();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSearch::on_pbSearch_clicked()
{
    resultsArray.clear();
    RestartSearch = YES;
    searchText = ui->lineEdit->text();
    searchTimer.start();

    //ui->labelSearching->setText("Searching");
    //ui->labelSearching->setVisible(YES);
    ui->progressBar->setVisible(YES);
    ui->pbClear->setVisible(NO);
    ui->pbSearch->setVisible(NO);
    ui->pbSettings->setVisible(NO);

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSearch::on_pbSettings_clicked()
{
    DialogPreferences *dpp = new DialogPreferences(this);
    dpp->show();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSearch::on_lineEdit_returnPressed()
{

    on_pbSearch_clicked();
}

// ------------------------------------------------------------------------
// when the user clicks on a verse, take them to the verse
// ------------------------------------------------------------------------
void DialogSearch::on_listWidget_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    //int a = 4;
    ChapterVerseStructType cvs;

    QStringList a;
    QListWidgetItem *itemPtr = ui->listWidget->currentItem();
    QString t;

    t = itemPtr->data(Qt::UserRole).toString();
    a = t.split(":");
    if (a.count()<3) {
        return;// dont' do anything if an error
    }
    cvs.BookNumber = a[0].toInt();
    cvs.Chapter = a[1].toInt();
    cvs.Verse = a[2].toInt();
    cvs.Posn = a[3].toInt();

    userHistory->pushHistoryItemWithBook(Preferences->getCurrentBook(),Preferences->getCurrentChapter(),Preferences->getCurrentVerse(),fvcp->currentScrollPosition);

    Preferences->setCurrentBook(cvs.BookNumber);
    Preferences->setCurrentChapter(cvs.Chapter);
    Preferences->setCurrentVerse(cvs.Verse);
    Preferences->currentPosition = 0;
    fvcp->doScrollToVerseAfterWebPageLoad=TRUE;
    fvcp->verseToScrollToAfterWebPageLoad=cvs.Verse;
    fvcp->LoadWebPage();

}

// clear and search for more
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSearch::on_pbClear_clicked()
{
    resultsArray.clear();
    resultsArrayNumItems=0;
//    if (timedSearch->SearchIsComplete)
//        RestartSearch = YES;

    searchState=waitOnSearchCompleteState;
    timedSearch->SearchIsComplete=NO;
     searchTimer.start();

     intStartBookNumber = intFinishedBookNumber;
    ui->progressBar->setVisible(YES);
    ui->progressBar->setValue(0);
     //ui->labelSearching->setText("Searching");
     ui->labelFound->setText("");
     ui->labelinBooks->setText("");
    // ui->labelSearching->setVisible(YES);
     ui->pbClear->setVisible(NO);
     ui->pbSearch->setVisible(NO);
     ui->pbSettings->setVisible(NO);
     ui->labelFound->setText("");
     ui->labelinBooks->setText("");

}
