#ifndef DICTIONARYDB_H
#define DICTIONARYDB_H


//
//  dictionarydb.h
//  CWP-Student
//
//  Created by WADE MAXFIELD on 1/21/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//

//#include <Foundation/Foundation.h>
#include "../shared/Database/database.h"
#include "../shared/BibleDatabaseClass/bibledatabaseclass.h"

#define DICTIONARY_TYPE_ENGLISH_DICTIONARY 0
#define DICTIONARY_TYPE_BIBLE_DICTIONARY   1


#define  DICTIONARY_TABLE "Dictionary"

#define  DICTIONARY_ID_COLUMN "ID"
#define  DICTIONARY_WORD "Word"
#define  DICTIONARY_WORD_INFO "info"

#define  DICTIONARY_DEFINITION "Def"

#define ENGLISH_DICTIONARY_DB_NAME "Webster.dic"
#define BIBLE_DICTIONARY_DB_NAME "Smiths.dic"
#define STRONGS_DICTIONARY_DB_NAME "Strongs.dic"
//----




class dictionaryDB : public BibleDatabaseClass
{
public:
    dictionaryDB();
    dictionaryDB(/*initWithDBName:*/ QString DictionaryDBName,/* andType:*/int t, /*andDictionaryType:*/int ed);
    ~dictionaryDB();
    int             dictionaryType;

    int             iCurrentDataIndex;
    int             iStartDataIndex;


    QString     getDefinitionsForWord(QString word     /*limitedToNumberOfDefinitions*/, int NumberOfDefinitions);
    void        initWithDBName(QString dbName  /*andType*/, int Type  /*andDictionaryType*/, int DicType);

    QString     getEnglishDictionaryDefinitionsForWord(QString word  /*limitedToNumberOfDefinitions*/, int NumberOfDefinitions);
    QString     getBibleDictionaryDefinitionsForWord(QString word  /*limitedToNumberOfDefinitions*/, int NumberOfDefinitions);

    QString     getBibleDictionaryDefinitionsForIndex( int dataIndex  /*andNumberOfDefinitions*/, int NumberOfDefinitions  /*thenScrollToFieldID*/, int scrollToFieldID);

    BOOL        isWordInBibleDictionary(QString word);
    QString      preProcessWord(QString word);
};


extern dictionaryDB *gDictionaryDB; // in the misclibrary.m file


#endif // DICTIONARYDB_H
