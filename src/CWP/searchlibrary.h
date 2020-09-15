#ifndef SEARCHLIBRARY_H
#define SEARCHLIBRARY_H
//
//  searchLibrary.h
//  Crossword-Try3
//
//  Created by WADE MAXFIELD on 10/14/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//
//#include <QLabel>
#include <QString>
//#include <UIKit/UIKit.h>
#include "../shared/misclibrary.h"
#include "../shared/RecordSet/recordset.h"
#include "../shared/chapterversestructclass.h"
#include "cwpdefines.h"
#define SIZE_OF_SEARCH_UNIT 128

typedef struct SearchInfoStruct
{
    BOOL isBibleSearch;           // clumsy, but for now good enough
    BOOL isCommentarySearch;
    BOOL isNoteSearch;
    int  indexIntoArray;
    BOOL OT;      // Boolean // when tru, search ot
    BOOL NT;      // Boolean
    BOOL SearchThisBookOnly;      // Boolean
    int16_t  BookNumber;      // integer // if non-zero has a book number in it
    QString BookTableName;      // String*40 // the name of the book to search
    QString WordToSearchFor;//[SIZE_OF_SEARCH_UNIT];      // String*128 // the word(s) to search for
    QString PhraseToSearchFor;//[SIZE_OF_SEARCH_UNIT];      // String*128 // the string to search for
    BOOL PhraseSearch;      // Boolean // when true, do the phrase search
    BOOL SearchForAWord;      // Boolean // when true, do the word search
    BOOL SearchForAWholeWord;      // Boolean // when true, look for a whole word, not a partial match
    int  WordSequenceNumber;      // integer // look for words at or past this sequence
    int  Chapter;      // integer // under certain circumstances, the chapter to search
    int  Verse;      // integer // under certain circumstances, the verse to search
    BOOL SearchForStrongs ;      // Boolean // when true, we are doing a strong's number search
    int  StrongsNumber;             // integer
    char Language[2];               // string*2
    int  SearchType;                // integer // see BookSearch Constants for choices
} SearchInfoStructType;
extern SearchInfoStructType globalSIT;


typedef struct SearchingWindowStruct {
    int     StaticTextChapter;//
    BOOL    SearchInitialized; // when true, the init is done
    BOOL    StaticTextChapterNumberVisible;
    BOOL    StaticTextChapterVisible;
    BOOL    StopSearch;
} SearchingWindowStructType;

extern SearchingWindowStructType SearchingWindow;


RecordSet               *runSelectQuery(SearchInfoStructType *s,QString Query);
ChapterVerseStructType  FindWord(SearchInfoStructType *s);
BOOL                    HandlePhraseSearch(SearchInfoStructType *s,QList<ChapterVerseStructType> *ResultList,int *numItems, SearchingWindowStructType *SearchingWindow );
RecordSet               *SearchBookChapter(SearchInfoStructType *s,BOOL *result);
BOOL                    SearchBook(SearchInfoStructType *s,QList<ChapterVerseStructType> *ResultList,int *numItems,SearchingWindowStructType *SearchingWindow ) ;
RecordSet               *SearchBookGetResult(SearchInfoStructType *s,BOOL *result,SearchingWindowStructType *SearchingWindow);// As recordSet

class QLabel;
void                    fillLabelsWithData(QString VerseNumberLabel, QString VerseTextLabel, int itemNumber,int BibleNumber);



#endif // SEARCHLIBRARY_H
