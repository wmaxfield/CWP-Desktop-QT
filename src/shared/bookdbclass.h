#ifndef BOOKDBCLASS_H
#define BOOKDBCLASS_H
//
//  BookDB.h
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 6/22/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//
#include <QString>

//#include <Foundation/Foundation.h>
#include "../shared/BibleDatabaseClass/bibledatabaseclass.h"
#include "structs.h"
#include "chapterversestructclass.h"


//#include <QStringList>

#define BOOK_TEXT_TABLE "BookText"



class BookDBClass : public BibleDatabaseClass
{
public:
    BookDBClass();


    BOOL Open(QString NameOfBook);

    void initAndOpenWithDBName(QString dbName);


    int  GetNumChapters();
    int  GetNumParagraphs(int Chapter );
    int GetFootnoteIndex();

    QString GetParagraph(SHOW_THIS_INFO_TYPE *STI, /*SearchParmsStructPointer:*/SEARCH_INFO_TYPE *sit );
    QString HandleWord(QString TheWord,/* withSearchWord*/QString s, /* andKeyHilite:*/BOOL Hilite, /* withSearchInfo:*/ SEARCH_INFO_TYPE *SearchInformation);

    QStringList searchForString(QString stringSearchWord, /* forSearchInstance:*/ int numberOfDefinitions);

};

#endif // BOOKDBCLASS_H
