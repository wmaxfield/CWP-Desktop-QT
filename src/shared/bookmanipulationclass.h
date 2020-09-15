#ifndef BOOKMANIPULATIONCLASS_H
#define BOOKMANIPULATIONCLASS_H
//
//  BookManipulationClass.h
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 6/22/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//

#include <QString>
#include "structs.h"

#include "../shared/bookhandlerclass.h"



class BookManipulationClass
{
public:
    BookManipulationClass();
    QString  buildTextPageForBook( int BookIndex, /*andChapter*/ int ChapterNumber, /* shouldScrollToVerse*/ BOOL bScrollToVerse, /* withVerseToScrollTo*/ int iVerseToScrollTo, /*andHiliteSearchTerm*/ BOOL HilightSearch,/* forSearchString*/ QString searchString);

    QString  getFootNoteForBook( int bookIndex, /*andChapter*/ int chapterNumber, /*andVerse*/ int Verse, /*andFootNoteNumber*/ int footNote );
    BibleBookInfoStructType GetTextBookInfoForBook( int BookNumber, /*andChapter*/ int ChapterNumber);
    QString getTextBookNameForNumber(int BookNumber);

    int     NumberOfBooks; //defaults to 1
    BOOL    DontShowLines; // defaults to YES
    BOOL    DoMetaTag;     // defaults to NO
    int     MTag;
    BOOL    DoHideNotes;   // defaults to NO
    BOOL    OneRow;        //When true (set at <onerow>), we are processing one row in the page, and <tr> </tr> must not be inserted until </onerow> is found

};

extern QList<BookHandlerClass *> gTextBooks;// array of text books we've found
extern BookManipulationClass *bmc;


#endif // BOOKMANIPULATIONCLASS_H
