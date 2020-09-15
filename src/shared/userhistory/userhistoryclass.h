#ifndef USERHISTORYCLASS_H
#define USERHISTORYCLASS_H
// USERHISTORYCLASS_H
//
//  History.h
//  Crossword-Student
//
//  Created by WADE MAXFIELD on 11/30/10.
//  Copyright 2010 Wade Maxfield All rights reserved.
//converted
//#include <QObject>
//#include <QStringList>
#include <QList>
#include <QStringList>


#include "chapterversestructclass.h"
#include "../shared/Database/database.h"
#include "cwpdefines.h"

#define MAX_HISTORY_ITEMS 40

#define  HISTORY_TABLE "HISTORY"
#define  BOOK_HISTORY_DB_NAME "BookHISTORY"
#define  BIBLE_HISTORY_DB_NAME "BibleHISTORY"

#define  CATEGORY_HISTORY "History"
#define  CATEGORY_TOPIC_NAME "TopicName"

#define  CATEGORY_NUM_HISTORY "NumHISTORY"
#define  NUMBER_OF_TOPICS "NumTopics"

class  ChapterVerseStructClass;

class UserHistoryClass : public SQliteDatabase
{
public:
    explicit UserHistoryClass();

    QStringList historyArray;
    ChapterVerseStructType cvs;
    BOOL        isOpen;         // when YES, is open

    void                     pushHistoryItemWithBook(int bookNumber, /*Chapter:*/ int chapter, /*Verse:*/ int verse,
                                                     /*ScrollPosn*/ int sPosn);//, /*Orientation:*/ UIDeviceOrientation orientation);
    ChapterVerseStructType   getHistoryItemByIndex(int index, /*Reverse:*/BOOL reverse);
    int                      getNumberOfHistoryItems();              // last item is #-1
    void                     deleteHistoryItemByIndex(int index); // first index is 0
    ChapterVerseStructType   popHistoryItem(); // last history added is removed
    void                     saveHistory();
    void                     loadHistory();
    void                     clearHistoryArrayPosn();
    void                     setNumberOfHistoryItems(int i);
};

extern UserHistoryClass *userHistory;
#endif
