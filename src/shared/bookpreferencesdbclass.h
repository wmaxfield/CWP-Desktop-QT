#ifndef BOOKPREFERENCESDBCLASS_H
#define BOOKPREFERENCESDBCLASS_H
//
//  BookPreferences.h
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 6/23/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//


#include "../shared/Database/database.h"

#define BOOK_PREFERENCES_TABLE "BookPrefs"

#define BOOK_CURRENT_BOOK_NUMBER "BookNumber"
#define BOOK_CURRENT_VERSE_NUMBER   "VerseNumber"
#define  BOOK_CURRENT_CHAPTER_NUMBER "ChapterNumber"

#ifndef CATEGORY_PREFERENCE
#define CATEGORY_PREFERENCE "Pref"
#endif

#define INI_BOOK_PREFERRED_TABLE_STYLE "BookPrefTableStyle"








class BookPreferencesDBClass :public SQliteDatabase
{
public:
    BookPreferencesDBClass();

    QString getPrefVariableByName( QString  varName, /*Category*/ QString category , /*Result:*/ BOOL *result);

    BOOL savePrefVariableWithCategory( QString Category , /*VarName*/ QString VariableName  , /*Value*/ QString myVar);


    BOOL  CreateTables(); // create the database tables (assuming they don't yet exist)
    BOOL  DropTables(); // drop the table in preparation for recreating it.

    void LoadPreferences();

    int currentBook();
    int currentVerse();
    int currentChapter();
    void setCurrentBook(int b);
    void setCurrentVerse(int v);
    void setCurrentChapter(int c);

    QString BookPreferredTableStyle();
    void    setBookPreferredTableStyle(QString lPreferredTableStyle);
    BOOL  loadingPrefs;

private:
    QString _BookPreferredTableStyle;
    int _currentBook;
    int _currentVerse;
    int _currentChapter;

};

extern BookPreferencesDBClass *gTextBookPreferences;
extern int gNumberOfBooks; // in initialization.m


#endif // BOOKPREFERENCESDBCLASS_H
