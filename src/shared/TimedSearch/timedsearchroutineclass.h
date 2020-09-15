#ifndef TIMEDSEARCHROUTINECLASS_H
#define TIMEDSEARCHROUTINECLASS_H
//
//  TimedSearchRoutine.h
//  Crossword-Try3
//
//  Created by WADE MAXFIELD on 10/15/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//

#include "searchlibrary.h"


// search magnifying glass from
//http://openclipart.org/detail/1750/magnifying-glass-by-thestructorr

class TimedSearchRoutineClass
{
public:
    TimedSearchRoutineClass();

    SearchingWindowStructType SearchingWindow;// misc data


    BOOL SearchIsComplete;
    BOOL notFirstTime;
    BOOL AlreadyHere;
    BOOL SearchTriggered;
    BOOL timerExpired;
    BOOL currentlySearching;
    BOOL newData;

    void timedSearchRoutine(SearchInfoStructType *searchInfo,/* andResultArray:*/ QList<ChapterVerseStructType> *resultsArray, /*andNumItems:*/int *numItems);
    void Initialize();

};

#endif // TIMEDSEARCHROUTINECLASS_H
