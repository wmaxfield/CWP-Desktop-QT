// RecentSearchesController.cpp
#include "../shared/chapterversestructclass.h"
#include "mainwindow.h"
//#include "TimedSearchRoutine.h"
//#include "speechDefines.h"
#include "cwpdefines.h"
#include "recentsearchescontroller.h"
#include "ui_recentsearchescontroller.h"
#include "indexpath.h"
#include "mainwindow.h"

QString RecentSearchesKey = "RecentSearchesKey";


static int iNumBooksSearched;

void RecentSearchesController::on_pbClear_clicked()
{
    // clear the list box
}
void RecentSearchesController::on_pbFindMore_clicked()
{
    // do more the list box
}

RecentSearchesController::RecentSearchesController(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecentSearchesController)
{
    ui->setupUi(this);

    // clear the delegate pointers
    didSelectRow=0;
    reloadData=0;

   setWindowTitle("Verses Found");

    int size = (resultsArrayNumItems)*76+140;
    if (size>fvcp->height()-50.0) {
        size=fvcp->height()-50.0;
    }

    // hopefully this resizes the dialog box
    this->setFixedHeight(size);// setFixedHeight
    //contentSizeForViewInPopover = CGSizeMake(300.0, size);

    // Create a button item to clear the recents list.
    connect(ui->pbClear,SIGNAL(clicked()),this,SLOT(on_pbClear_clicked()));
    connect(ui->pbFindMore,SIGNAL(clicked()),this,SLOT(on_pbFindMore_clicked()));


   // speakHelp("Enter your search text, and search will start automatically.  Touch a verse reference to navigate to that verse.");

    viewWillAppear(YES);
}

RecentSearchesController::~RecentSearchesController()
{
    delete ui;
    if (searchTimer) {
        searchTimer->stop();
        delete searchTimer;
    }
    if (timedSearch) {
       delete timedSearch;
    }


}




/*
 ------------------------------------------------------------------------
 Private methods used only in this file
 ------------------------------------------------------------------------
 */



void  RecentSearchesController:: adjustPopOverSize()
{
    int size = (resultsArrayNumItems)*76+140;
    if (size>fvcp->height()-50.0) {
        size=fvcp->height()-50.0;
    }
    //fvcp->recentSearchesPopoverController.popoverContentSize= CGSizeMake(300, size);

}
/* show the user that loading activity has started */

//------------------------------------------------------------------
//------------------------------------------------------------------
void  RecentSearchesController:: startAnimation()
{
    fvcp->activityIndicator->animate();
    //[activityIndicator startAnimating];
    // let task bar know also
   // UIApplication *application = [UIApplication sharedApplication];
   // application.networkActivityIndicatorVisible = YES;
}


/* show the user that loading activity has stopped */

//------------------------------------------------------------------
//------------------------------------------------------------------
void  RecentSearchesController:: stopAnimation()
{
    //[activityIndicator stopAnimating];
    //UIApplication *application = [UIApplication sharedApplication];
   // application.networkActivityIndicatorVisible = NO;
    fvcp->activityIndicator->stopAnimation();
}

// global results array

//------------------------------------------------------------------
// stop search, and reset the indicators
//------------------------------------------------------------------
void  RecentSearchesController::stopSearch()
{
    searchState =     StopSearchState;
    RestartSearch = NO;
    if (timedSearch) {
        timedSearch->SearchIsComplete = YES;
    }
    if (Preferences->getSearchThisBookOnly()) {
        searchInfo.BookNumber = Preferences->getCurrentBook();
    }else {
        if (Preferences->getSearchOT()) {
            searchInfo.BookNumber = 1;
        }
        else {
            if (Preferences->getSearchNT()) {
                searchInfo.BookNumber = LAST_BOOK_IN_OT+1;
            }
        }
    }

    if (reloadData!=0)
        reloadData();

}
//------------------------------------------------------------------
// timer routine
//------------------------------------------------------------------

void  RecentSearchesController:: performSearch()
{
    // int index;


    if (fvcp->recentSearchesController->isVisible()==NO) {
        RestartSearch = NO;
        searchState = SearchCompleteState;
        return;
    }


    if (RestartSearch) {
        RestartSearch = NO;
        searchState = RestartSearchState;
    }


    switch (searchState) {
        case RestartSearchState:
            countDownToRestartSearch = 6; // 600 milliseconds
            searchState = CountDownToRestartSearchState;
            break;

        case CountDownToRestartSearchState:
            if (--countDownToRestartSearch) {
                break;
            }
            searchState = SetupSearchState;
            resultsArrayNumItems=0;
            if (reloadData!=0)
               reloadData();

            //countDownToRestartSearch=3;
            break;


        case SetupSearchState:
            if (timedSearch) {
                timedSearch->Initialize();
            }

            searchInfo.OT=Preferences->getSearchOT();
            searchInfo.NT=Preferences->getSearchNT();
            searchInfo.SearchThisBookOnly =Preferences->getSearchThisBookOnly();
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

        case CheckSearchTextState:
            if (searchText.length()) {
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
                    timedSearch->timedSearchRoutine(&searchInfo, /* andResultArray:*/resultsArray, /*andNumItems:*/&resultsArrayNumItems );
                }
                if (reloadData)
                    reloadData();
                iNumBooksSearched++;


                if (fvcp) {
                    adjustPopOverSize();
                }

                if (timedSearch && timedSearch->SearchIsComplete) {
                    searchState = SearchCompleteState;
                }
            }
            break;

        case SearchCompleteState:

            break;


        default:
            break;
    }


    if (timedSearch) {
        timedSearch->timerExpired = NO;
    }
    if (!searchTimer) {
        searchTimer = new QTimer();
        searchTimer->setInterval(100);
        connect(searchTimer,SIGNAL(timeout()),SLOT(performSearch()));
        searchTimer->start();

                //[NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(performSearch) userInfo:nil repeats:YES];
    }


}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void  RecentSearchesController::viewWillAppear(BOOL animated) {
    Q_UNUSED(animated);
    // Ensure the complete list of recents is shown on first display.


    if (!timedSearch) {
        timedSearch =new TimedSearchRoutineClass();
        timedSearch->timerExpired = NO;
        if (!searchTimer) {
            searchTimer = new QTimer();
            searchTimer->setInterval(100);
            connect(searchTimer,SIGNAL(timeout()),SLOT(performSearch()));
            searchTimer->start();

                    //[NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(performSearch) userInfo:nil repeats:YES];
        }
    }

    if (Preferences->searchPrefChanged) {
        resultsArrayNumItems=0;

        if (reloadData)
            reloadData();
    }


}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void  RecentSearchesController::doMoreSearches(void* sender) {
    Q_UNUSED(sender);
    if (resultsArrayNumItems>= Preferences->MaximumSearches) {
        resultsArrayNumItems=0;
        if (timedSearch) {
            timedSearch->SearchIsComplete=NO;
        }
        searchState= CheckSearchTextState ;
        intStartBookNumber = searchInfo.BookNumber;
    }
    else {
        RestartSearch = YES;
    }

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void  RecentSearchesController::showClearRecentsAlert(void *sender) {
    Q_UNUSED(sender);
    // If the user taps the Clear Recents button, present an action sheet to confirm.
    //UIActionSheet *actionSheet = [[UIActionSheet alloc] initWithTitle:"Clear Search?" delegate:self cancelButtonTitle:"Cancel" destructiveButtonTitle:"YES" otherButtonTitles:nil];//"This Book","OT Only","NT Only",nil];
   // [actionSheet showInView:view];
   // [actionSheet release];
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void  RecentSearchesController::actionSheet(UIActionSheet *actionSheet, /* clickedButtonAtIndex:*/int buttonIndex) {
    Q_UNUSED(actionSheet);

    if (buttonIndex == 0) {
        /*
         If the user chose to clear recents, remove the recents entry from user defaults, set the list to an empty array, and redisplay the table view.
         */
        //[[NSUserDefaults standardUserDefaults] setObject:nil forKey:RecentSearchesKey];

        //[tableView reloadData];
        //clearButtonItem.enabled = NO;
        resultsArrayNumItems=0;
        stopSearch();

        //[tableView reloadData]; // reload the cleared data

    }
}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int  RecentSearchesController::tableViewNumberOfRowsInSection(UITableView *tableView, /*numberOfRowsInSection:*/int section)
{
    Q_UNUSED(tableView);
    Q_UNUSED(section);
    return resultsArrayNumItems+1;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

float  RecentSearchesController::tableViewHeightForRowAtIndexPath(UITableView *tableView,/* heightForRowAtIndexPath:*/NSIndexPath *indexPath)
{
    Q_UNUSED(tableView);

    if (indexPath->row==0) {
        return 100.0;
    }
    return 76.0;
}

// ------------------------------------------------------------------------
// Display the strings in displayedRecentSearches.
// ------------------------------------------------------------------------
VerseCellStruct RecentSearchesController::tableViewCellForRowAtIndexPath(UITableView *tableView ,/*cellForRowAtIndexPath:*/NSIndexPath *indexPath) {
    Q_UNUSED(tableView);
    VerseCellStruct cell ;


    if (indexPath->row==0) {
        // ------------------------------------------------------------------------
        // this is the cell for the search options
        // ------------------------------------------------------------------------

        //if (cell) {
            if (searchState == StopSearchState) {
                searchState = SearchCompleteState;
                cell.searchResultLabel = "";
                return cell;
            }

            //int intStartBookNumber;

            if (Preferences->getSearchThisBookOnly()) {
                searchInfo.SearchThisBookOnly = YES;
                 intStartBookNumber = Preferences->getCurrentBook();
            }

            if (Preferences->searchPrefChanged) {
                Preferences->searchPrefChanged = NO;
                 intStartBookNumber = searchInfo.BookNumber =0;
            }

            if (intStartBookNumber ==0) {
                if (Preferences->getSearchThisBookOnly()) {
                   searchInfo.BookNumber= intStartBookNumber = Preferences->getCurrentBook();
                }else {
                    if (Preferences->getSearchOT()) {
                       searchInfo.BookNumber= intStartBookNumber = 1;
                    }else {
                        // must be search NT
                       searchInfo.BookNumber= intStartBookNumber = LAST_BOOK_IN_OT+1;
                    }

                }
            }



            BibleBookInfoStructType bbis=getBibleBookInfo(searchInfo.BookNumber );
            BibleBookInfoStructType bbis2=getBibleBookInfo(intStartBookNumber );
          //  BibleBookInfoStructType bbis3 = bbis;

            // this search from mess needs fixing.


            if (searchInfo.BookNumber <= intStartBookNumber) {
                if ( iNumBooksSearched>1) {
                    if (!Preferences->getSearchNT()) {
                        bbis =getBibleBookInfo(LAST_BOOK_IN_OT );
                    }else {
                        bbis =getBibleBookInfo(LAST_BOOK_IN_BIBLE );
                    }

                }
            }


            if (timedSearch && timedSearch->SearchIsComplete) {
                if (fvcp->activityIndicator->isAnimating()) {
                   // [cell.activityIndicator stopAnimating];
                    fvcp->activityIndicator->stopAnimation();
                }



                if (bbis.BookNumber == 1 && iNumBooksSearched>1) {
                    if (!Preferences->getSearchNT()) {
                        bbis =getBibleBookInfo(LAST_BOOK_IN_OT );
                    }else {
                        bbis =getBibleBookInfo(LAST_BOOK_IN_BIBLE );
                    }
                }


                if (Preferences->getSearchThisBookOnly()) {
                    bbis = getBibleBookInfo(Preferences->getCurrentBook());
                }


//                cell.searchingStatusLabel.text = [NSString stringWithFormat:"Will search from %s %d.", bbis3.BookName, searchInfo.Chapter];
                cell.searchResultLabel=  QString::number(resultsArrayNumItems)+" verses found from\n"+bbis2.BookName +" to "+bbis.BookName;//,resultsArrayNumItems,bbis2.BookName,bbis.BookName ];

            }else {
                if (searchInfo.Chapter==0) {
                    searchInfo.Chapter=1;
                }

                if (searchState == CheckSearchTextState) {

                    if (!fvcp->activityIndicator->isAnimating()) {
                        fvcp->activityIndicator->animate();
                    }

                    cell.searchingStatusLabel = "Searching from "+ QString(bbis.BookName)+" "+QString::number( searchInfo.Chapter);

                }else {
                //    cell.searchingStatusLabel.text = [NSString stringWithFormat:"Will search from %s %d.", bbis.BookName, searchInfo.Chapter];
                }


                if (resultsArrayNumItems) {
                    cell.searchResultLabel = QString::number(resultsArrayNumItems)+" verses found";
                } else {
                    if (cell.searchResultLabel.length())
                        cell.searchResultLabel= "";
                }

                // cell.searchingStatusLabel.text = "";
            }// if(timedsearch.searchiscomplete)..else{


            if (timedSearch->currentlySearching  && searchState != SearchCompleteState) {
                if (cell.searchingStatusLabel.length()==0) {
                    cell.searchingStatusLabel="Searching from "+ QString(bbis.BookName)+" "+QString::number( searchInfo.Chapter);
                }
            }

            return cell;
    }// if (indexPath.row==0)


    // ------------------------------------------------------------------------
    // this is all others
    // ------------------------------------------------------------------------



    if (cell.searchResultLabel.length()) {
        // the row is decremented by 1, because #0 is taken up with the search status
        fillLabelsWithData(cell.VerseNumberLabel, cell.VerseTextLabel, indexPath->row-1, 0/*0 is first bible*/);// for now only do the first bible
        //   [cell autorelease];
    }


    return cell;

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void  RecentSearchesController::tableViewDidSelectRowAtIndexPath(UITableView *tableView,/* didSelectRowAtIndexPath:*/NSIndexPath *indexPath) {
    Q_UNUSED(tableView);
    // Notify the delegate if a row is selected.
    if (indexPath->row==0) {
        return;
    }
    if (didSelectRow)
        (*didSelectRow)(this,indexPath->row);
}


