#ifndef STRUCTS_H
#define STRUCTS_H
/*
 *  structs.h
 *  CrosswordTry2
 *
 *  Created by WADE MAXFIELD on 9/25/10.
 *  Copyright 2010 Wade Maxfield. All rights reserved.
 *
 */

//#include "searchLibrary.h"
#include <QString>
#include "cwpdefines.h"
//#include "BibleInfo.h"

// wsm move these here so that they will resolve
#define MAX_NUM_SELECTED_BOOKS 20
#define MAX_NUM_SELECTED_NOTES 20
#define MAX_NUM_BIBLES 20
#define MAX_NUM_SELECTED_COMMENTARIES 20

#ifndef SIZE_OF_SEARCH_UNIT
#define SIZE_OF_SEARCH_UNIT 128 // this is in two places in the code. fix later
#endif

typedef struct show_this_info_struct
{
    BOOL ShowStrongs;           // if yes, show strongs
    BOOL HilightSearch;         // if yes, hilite the search
    BOOL enableEnglishDictionary;  // if yes, the dictionary window is showing
    BOOL BlendStrongsIntoWord; // set from preferences module
    BOOL DoScrollToVerse;             // if YES, scroll to a verse
    int  VerseToScrollTo;                  // the verse to scroll to
    BOOL DoHideNotes;           // if YES, hide the notes
    BOOL DoHideTagsForPrinting; // if YES, the "S" and "B" are hidden for printing on Book
    BOOL ShortRun;              // if YES, then do a short run (deprecated)
    BOOL CompletedRun;          // when passed as a pointer, the run is completed

    BOOL EnableHTMLHeader;      // when YES, the HTMLHearder is enabled
    BOOL EnableMultimediaTag;      // when YES, will speak to you SPEECH

    BOOL EnableVerseEditing;    // when YES, the verse can be edited by clicking on E

    int  HeaderSize;            // size of the header
    int  IndexIntoSelectedBooksArray; // used for cell formatting
    int  NumberOfBooksBeingShownOnPage;
    BOOL doStackVerses;         // when YES, verses are not side by side
    int  IDNumber;              // used to tag the strong's word for re-setting the popup when rotating

    BOOL showBookMarkTag;        // when YES, (default), the Bookmark is enabled for the verse
    BOOL showCopyTag;         // when YES, the "C" symbol is there for copy
} SHOW_THIS_INFO_TYPE;

extern SHOW_THIS_INFO_TYPE globalSTI;// preferencesLibrary.cpp


typedef struct search_info_struct
{
    BOOL        OT;
    BOOL        NT;
    BOOL        ThisBookOnly; // search this book only
    BOOL        SearchForStrongs;

    int         BookNumber;
    int         ChapterNumber;
    int         VerseNumber;

    QString        BookTableName;

    QString        Translation;
    int         indexIntoArray;

    QString        WordToSearchFor;

    QString     PhraseToSearchFor;
    BOOL        PhraseSearch;

    BOOL        SearchForAWord;
    BOOL        SearchForAWholeWord;

    int         WordSequenceNumber;


    QString        Language;
    int         SearchType;
    int         NumberOfBooksInBible; // set to 66 always

} SEARCH_INFO_TYPE;




typedef struct BibleBookInfoStruct
{
    int BookType;   // uses #defines in BibleInfo.h
    int BookNumber;
    int NumChapters;
    int NumVerses;
    QString TableName;
    QString BookName;
    QString BookSName;
    QString Testament;
    QString Language;
    QString BookVSName;
    QString Title;
} BibleBookInfoStructType;

BibleBookInfoStructType getBibleBookInfo(int BookNumber);

typedef struct BookInfoStruct
{
    QString Name;
    int  Type;
    int  Index;
} BookInfoStructType;


typedef struct book_chapter_verse_struct
{
    int BookNumber;
    int ChapterNumber;
    int VerseNumber;
} BIBLE_BCV_TYPE;


typedef struct BookChapterVerseStruct
{
    int BookNumber;
    int NumberOfVerses[160];// number of verses for each chapter, 1 - 160
    int NumberOfChapters;   // number of chapters filled in.
} BookChapterVerseStructType;

//BookChapterVerseStructType GetBookChapterVerse(int BookNumber);
void GetBookChapterVerse(int BookNumber, BookChapterVerseStructType& bcv);

// here to make it easier to use in the future
#define TYPE_BBL_TYPE 0
#define TYPE_ABP_TYPE 1
#define TYPE_BOOK_TYPE 2
#define TYPE_DICTIONARY_TYPE 3 /* for the bible dictionary if needed*/
#define TYPE_NOTES_TYPE 4
#define TYPE_COMMENTARY_TYPE 5
#define TYPE_PREFERENCES_TYPE 6
#define TYPE_MORPHOLOGY_TYPE  7  // morphology can't be spoken
#define TYPE_MEDIA_INFO_TYPE  8  // place holder
#define TYPE_PICTURE_INFO_TYPE 9


typedef struct StrongsStructureStruct
{
    int     Number; // the strong's number
    QString    OriginalWord; // the original lanugage in ascii chars or html chars utf8
    QString    Phonic;        // the phonics of sounding the word
    QString    Language;           // the original language
    QString    Definition;       // the english definition
    QString    Cross1;        // a url string to a previous or subsequent word (not used)
    QString    Cross2;        // a url string to a previous or subsequent word (not used)
    QString    Cross3;        // a url string to a previous or subsequent word (not used)
} STRONGS_STRUCTURE_TYPE;

#define StrongsStructure STRONGS_STRUCTURE_TYPE
#endif // STRUCTS_H
