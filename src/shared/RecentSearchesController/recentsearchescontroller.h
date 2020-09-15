#ifndef RECENTSEARCHESCONTROLLER_H
#define RECENTSEARCHESCONTROLLER_H
#if 1
#include <QDialog>
#include <QTimer>
#include "../shared/TimedSearch/timedsearchroutineclass.h"
#include "cwpdefines.h"
#include "searchLibrary.h"

#include "indexpath.h"


extern QString RecentSearchesKey;

enum searchStates {
    RestartSearchState=1,
    CountDownToRestartSearchState,
    SetupSearchState,
    CheckSearchTextState,
    SearchCompleteState,
    StopSearchState

};

namespace Ui {
class RecentSearchesController;
}

#define UIActionSheet void
#define UITableView  void


class RecentSearchesController : public QDialog
{
    Q_OBJECT


public:
    explicit RecentSearchesController(QWidget *parent = 0);
    ~RecentSearchesController();
    void viewWillAppear(BOOL animated);


    void stopSearch(); // kill the search in progress

    UIBarButtonItem             *clearButtonItem;
    UIBarButtonItem             *nextButtonItem;

    TimedSearchRoutineClass          *timedSearch;
    SearchInfoStructType        searchInfo;
    QString                     searchText;
    BOOL                        RestartSearch;
    int                         countDownToRestartSearch; // when non-zero, count to 0, then restart search at that point
    BOOL                        setupSearch;                //when yes, then init the search parms
    int                         searchState;
    int                         resultsArrayNumItems;

    int                         intStartBookNumber;

    QTimer                     *searchTimer;
    // this delegate works by putting the function pointer into it
    // only one delegate function
    // use by doing  rscp->delegate = function;
    void (*didSelectRow)(RecentSearchesController *, /* didSelectRow*/int);
    void (*reloadData)(); // call back to force data reload

    void actionSheet(UIActionSheet *actionSheet, /* clickedButtonAtIndex:*/int buttonIndex) ;
    int tableViewNumberOfRowsInSection(UITableView *tableView, /*numberOfRowsInSection:*/int section);
    float  tableViewHeightForRowAtIndexPath(UITableView *tableView,/* heightForRowAtIndexPath:*/NSIndexPath *indexPath);
    VerseCellStruct tableViewCellForRowAtIndexPath(UITableView *tableView,/* cellForRowAtIndexPath:*/NSIndexPath *indexPath);
    void  tableViewDidSelectRowAtIndexPath(UITableView *tableView,/* didSelectRowAtIndexPath:*/NSIndexPath *indexPath) ;

    void doMoreSearches(void *sender);
    void showClearRecentsAlert(void *sender);
private slots:
    void on_pbClear_clicked();
    void on_pbFindMore_clicked();
    void performSearch();

private:
    //void   performSearch();
    void   startAnimation();
    void   stopAnimation();
    void   adjustPopOverSize();


    Ui::RecentSearchesController *ui;



};

#endif

#endif // RECENTSEARCHESCONTROLLER_H
