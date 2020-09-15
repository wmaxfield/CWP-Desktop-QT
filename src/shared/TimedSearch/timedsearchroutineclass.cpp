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
*/#include "timedsearchroutineclass.h"
#include "searchlibrary.h"
#include "../shared/chapterversestructclass.h"
#include "../shared/bibleinfo.h"
#include <QList>
//
//  TimedSearchRoutine.m
//  Crossword-Try3
//
//  Created by WADE MAXFIELD on 10/15/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//

// magnifying glass http://openclipart.org/detail/140539/magnifying-glass-by-gsagri04

TimedSearchRoutineClass::TimedSearchRoutineClass()
{
}

void TimedSearchRoutineClass::Initialize()
{
     SearchIsComplete = NO;
    SearchTriggered =NO;
    currentlySearching = NO;

    SearchingWindow.SearchInitialized =YES;

}

//---------------------------------------
//---------------------------------------
void TimedSearchRoutineClass:: timedSearchRoutine(SearchInfoStructType *searchInfo,/*andResultArray:*/ QList<ChapterVerseStructType> *SearchResultsList, /*andNumItems*/int *numItems)
{
    if ((SearchIsComplete)) {//then
        currentlySearching = NO;
        goto ExitSearch;
    }
    currentlySearching = YES;

    // do a book search if (we should
    if (searchInfo->SearchThisBookOnly) {//then
        searchInfo->BookNumber = Preferences->getCurrentBook();
        searchInfo->isBibleSearch = YES;

        searchInfo->BookTableName = getBibleBookInfo(searchInfo->BookNumber).TableName;

        if (! SearchBook(searchInfo,SearchResultsList,numItems, &SearchingWindow)) {
            goto ExitSearch;
        }

        goto SearchFinished;
    }// searcinfo.book



    //----------------------------------------------------
    // if (the search allows the OT,) {//then search the OT
    //----------------------------------------------------
    if (searchInfo->OT) {//then
        if (searchInfo->BookNumber>LAST_BOOK_IN_OT) {
            searchInfo->BookNumber=LAST_BOOK_IN_OT;
        }
        searchInfo->BookTableName= getBibleBookInfo(searchInfo->BookNumber).TableName;

        SearchBook(searchInfo,SearchResultsList,numItems,&SearchingWindow);

        if (*numItems >= Preferences->MaximumSearches)
        {
            //si=5;
            goto SearchFinished;//then goto SearchFinished
        }


        if (++searchInfo->BookNumber > LAST_BOOK_IN_OT) {//then
            searchInfo->BookNumber = 1;
            //searchInfo->OT = NO;<--wsm this reset search menu
            searchInfo->OT = NO;
        } //end if
        goto ExitSearch;
    } //end if


    //----------------------------------------------------
    // if (the search allows the NT,) {//then search the NT
    //----------------------------------------------------
    if ((searchInfo->NT)) {//then

        if ((searchInfo->BookNumber < LAST_BOOK_IN_OT+1 )) {//then
            searchInfo->BookNumber = LAST_BOOK_IN_OT +1;
        }

        searchInfo->BookTableName = getBibleBookInfo(searchInfo->BookNumber).TableName;

        SearchBook(searchInfo,SearchResultsList,numItems,&SearchingWindow);//){//SearchResultWindow.SearchResultsList) // perform the search and fill list box

        if (*numItems >= Preferences->MaximumSearches) goto SearchFinished;//then goto SearchFinished


        if (++searchInfo->BookNumber > Preferences->NumberOfBooksInBible) {//then
            searchInfo->BookNumber = 1;
            searchInfo->NT = NO;
        } //end if

        goto ExitSearch;
    } //end if

// if we make it here, the search is finished
    //---------------------------------------
    //SEARCH FINISHED
    //---------------------------------------
SearchFinished:
    //---------------------------------------
    //---------------------------------------
    SearchIsComplete = YES;
ExitSearch://<--------------
    timerExpired=YES;
    return; // come back again

}

