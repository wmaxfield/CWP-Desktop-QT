#ifndef DIALOGSEARCH_H
#define DIALOGSEARCH_H

#include <QDialog>
#include <QTimer>
#include "../shared/TimedSearch/timedsearchroutineclass.h"
#include "cwpdefines.h"
#include "searchlibrary.h"

#include "indexpath.h"

#define UIActionSheet void
#define UITableView  void

extern QString RecentSearchesKey;

enum searchStates {
    RestartSearchState=1,
    CountDownToRestartSearchState,
    SetupSearchState,
    CheckSearchTextState,
    waitOnSearchCompleteState,
    SearchCompleteState,
    StopSearchState

};




namespace Ui {
class DialogSearch;
}

class DialogSearch : public QDialog
{
    Q_OBJECT

public:
    explicit    DialogSearch(QWidget *parent = 0);
                ~DialogSearch();
    void        showEvent(QShowEvent *e);
    void        resizeEvent ( QResizeEvent * event );



    TimedSearchRoutineClass          *timedSearch;

    SearchInfoStructType        searchInfo;
    QString                     searchText;
    BOOL                        RestartSearch;
    int                         countDownToRestartSearch; // when non-zero, count to 0, then restart search at that point
    BOOL                        setupSearch;                //when yes, then init the search parms
    int                         searchState;
    int                         resultsArrayNumItems;

    int                         intStartBookNumber;
    int                         intFinishedBookNumber;
    int                         iNumBooksSearched;
    int                         newData;
    QTimer                     searchTimer;

    QString                     RecentSearchesKey;// = "RecentSearchesKey";

    // this delegate works by putting the function pointer into it
    // only one delegate function
    // use by doing  rscp->delegate = function;
    void (*didSelectRow)(DialogSearch *, /* didSelectRow*/int);
    //void (*reloadData)(); // call back to force data reload

    void actionSheet(UIActionSheet *actionSheet, /* clickedButtonAtIndex:*/int buttonIndex) ;
    int tableViewNumberOfRowsInSection(UITableView *tableView, /*numberOfRowsInSection:*/int section);
    float  tableViewHeightForRowAtIndexPath(UITableView *tableView,/* heightForRowAtIndexPath:*/NSIndexPath *indexPath);
    VerseCellStruct tableViewCellForRowAtIndexPath(UITableView *tableView,/* cellForRowAtIndexPath:*/NSIndexPath *indexPath);
    void  tableViewDidSelectRowAtIndexPath(UITableView *tableView,/* didSelectRowAtIndexPath:*/NSIndexPath *indexPath) ;

    void doMoreSearches(void *sender);
    void showClearRecentsAlert(void *sender);
    void reloadData();


private slots:
    void on_pbClose_2_clicked();

    void on_pbSearch_clicked();

    void on_pbSettings_clicked();

    void on_lineEdit_returnPressed();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_pbClear_clicked();

    void performSearch();

private:
    Ui::DialogSearch *ui;

    void   startAnimation();
    void   stopAnimation();
    void   adjustPopOverSize();

    int mainwindowWidth, mainwindowHeight;

};

// small search icon http://openclipart.org/image/300px/svg_to_png/58717/1273014462.png
// small gear icon http://openclipart.org/detail/169903/gear-by-hank0071
// broom icon http://openclipart.org/detail/182731/sweep-by-michelinux-182731

extern DialogSearch *dswp;

#endif // DIALOGSEARCH_H
