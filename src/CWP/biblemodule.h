#ifndef BIBLEMODULE_H
#define BIBLEMODULE_H

//
//  BibleModule.h
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/23/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//
//#include "misclibrary.h"
#include <QString>
#include "../shared/structs.h"


#define ATTRIBUTE_TYPE "Type"
#define BIBLES_DIRECTORY "Bibles"


#define BIBLE_TABLE "Bible"
#define BIBLE_TYPE_FIELD "TypeOfBibleDB"
#define BIBLE_TYPE_TABLE "BibleType"
#define BOOK_COLUMN "Book"
#define BOOK_SNAME_COLUMN "BookShortName"
#define CHAPTER_COLUMN "Chapter"
#define COMMENTARY_DIRECTORY "Commentaries"


#define COPYRIGHT_INFO_COLUMN "CopyrightInfo"
#define COPYRIGHT_TABLE "copyright"
#define DefaultNotesFile "MyNotes"
#define DEFAULT_COL_WIDTH 40

#define DICTIONARY_ENTRY_COLUMN "DictEntryNumber"

#define DICTIONARIES_DIRECTORY "Dictionaries"
#define BIBLE_DIRECTORY "Bibles"

#define TITLE_COLUMN "TitleColumn"
#define TITLE_TABLE  "Title"

#define STITLE_TABLE "stitle"
#define STITLE_COLUMN "Title"

#define GET_STRING "GET"
#define INI_LEFT "Left"
#define INI_TOP "Top"



#define NUM_TABLE_TYPES "NUM_TABLE_TYPES"
#define ORIG_WORD_TYPE_COLUMN "OrigWordLanguage"
#define REFERENCE_TABLE "Reference"
#define REF_BOOK_COLUMN "BookRef"
#define REF_CHAPTER_COLUMN "ChapterRef"
#define REF_VERSE_COLUMN "VerseRef"
#define SET 4
#define STRONGS_DATABASE_NAME "Strongs"

#define PERCENTAGE_SPACE_BETWEEN_COLUMNS 5
#define PERCENTAGE_SPACE_BETWEEN_COLUMNS_STR "5"
//----------------------------------------------------------
#define BIBLE_DATABASE_SUFFIX ".bbl"
#define BIBLE_DATABASE_SUFFIX_TYPE 1

#define COMMENTARY_DATABASE_SUFFIX ".cmt"
#define COMMENTARY_DATABASE_SUFFIX_TYPE 5

#define DICTIONARY_SUFFIX ".dic"
#define DICTIONARY_DATABASE_SUFFIX_TYPE 3

#define NOTE_DATABASE_SUFFIX ".note"
#define NOTES_DATABASE_SUFFIX_TYPE 4

#define BOOK_DATABASE_SUFFIX ".book"
#define BOOK_DATABASE_SUFFIX_TYPE 2

#define STRONGS_DICTIONARY_SUFFIX ".dic"
#define STRONGS_DICTIONARY_DATABASE_SUFFIX_TYPE 6


#define MEDIA_INFO_DATABASE_SUFFIX_TYPE 8
#define MEDIA_INFO_DATABASE_SUFFIX ".minf"

#define PICTURE_INFO_SUFFIX_TYPE 9
#define PICTURE_INFO_DATABASE_SUFFIX ".pinf"

//----------------------------------------------------------



#define TESTAMENT_COLUMN "TSTMT"
#define TESTAMENT_TYPE_TABLE "TESTTYPE"
#define ZEROFILL 1

QString BuildPageForBookChapterVerse(BookInfoStructType *BooksToUse, int Book, int Chapter, int Verse,BibleBookInfoStructType *bis) ;

void BuildBiblePage(void);

QString BuildVersePage(BookInfoStructType *BooksToUse,// array of indexes
                         BookChapterVerseStructType *bcv, /* number of chapters in this book, and number of verses in the chapter*/
                      //   int NumBooks, /* number of books to show*/
                         SHOW_THIS_INFO_TYPE *ShowThisInfo,
                         SEARCH_INFO_TYPE *sit // also includes book number, verse, chapter
                         ) /* HiliteSearch, DoMetaTag, etc*/
;

void BuildPageForBookMark(QString Topic,BookInfoStructType *BooksToUse);

QString BuildBookMarkPage(BookInfoStructType *BooksToUse,// array of indexes
                            QString topic,// the topic in the bookmark array
                            QStringList VerseListArray, /* number of chapters in this book, and number of verses in the chapter*/
                            // int NumBooks, /* number of books to show*/
                            SHOW_THIS_INFO_TYPE *ShowThisInfo,
                            SEARCH_INFO_TYPE *sit // also includes book, chapter, verse number
                            ) /* HiliteSearch, DoMetaTag, etc*/
;

extern QString CurrentWebPage;           // global for the web page

#endif // BIBLEMODULE_H
