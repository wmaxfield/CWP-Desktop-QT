#ifndef CHAPTERVERSESTRUCTCLASS_H
#define CHAPTERVERSESTRUCTCLASS_H
// CHAPTERVERSESTRUCTCLASS_H
/*
 *  ChapterVerseStructClass.h
 *  Crossword-Try3
 *
 *  Created by WADE MAXFIELD on 10/15/10.
 *  Copyright 2010 CrossWordProject. All rights reserved.
 *
 */
#include "structs.h"

#ifndef UIDeviceOrientation
#define UIDeviceOrientation int
#endif

//#define MAX_ENTRIES_IN_RESULTS_ARRAY 100

typedef struct chapterVerseStructure
{
    int BookNumber;
    int Chapter;
    int Verse;
    int Posn;   // for scroll position or sequence number
    UIDeviceOrientation orientation;
} ChapterVerseStructType;



extern int resultsArrayNumItems;
//extern ChapterVerseStructType resultsArray[MAX_ENTRIES_IN_RESULTS_ARRAY+5];
extern QList<ChapterVerseStructType>resultsArray;

class ChapterVerseStructClass
{
public:
    ChapterVerseStructClass();
    ChapterVerseStructType cvs;

};

#endif

