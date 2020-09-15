#ifndef BOOKMANIPULATIONLIBRARY_H
#define BOOKMANIPULATIONLIBRARY_H
//
//  BookManipulationLibrary.h
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 6/22/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//

//#include <Foundation/Foundation.h>
#include "../shared/Database/database.h"


QString CheckSup(RecordSet *rs,int BookIndexNumber,int Chapter, int Verse);
QString CreateBookMarkTag(int Book,int Chapter,int VerseNumber ,QString  VerseTxt,BOOL DoHideNote ,int DisplayBible  ,int BooksToUseIndex, BOOL OneRow);
void BuildBookPageForBookChapterVerse(int book, int chapter, int verse, QString searchString );

void LoadDefaultBookPrefs(void);

QString TextBook(int BookNumber);

#endif // BOOKMANIPULATIONLIBRARY_H
