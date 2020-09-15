//
//  MediaModule.h
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 5/28/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//
#include <QString>
//#import <Foundation/Foundation.h>
#include "BibleDatabaseClass/bibledatabaseclass.h"
#include "structs.h"
#include  "bibleinfo.h"

#define MEDIA_DB_MASTER_TABLE "MediaTable"

#define MENU_TEXT_FIELD_NAME "MenuText"
#define FILENAME_FIELD_NAME "FileName"
#define DB_FILENAME_FIELD_NAME "DBFileName"
#define CHAPTER_FIELD_NAME "Chapter"
#define BOOKNUMBER_FIELD_NAME "BookNumber"
#define SELTYPE_FIELD_NAME "SelType"
#define STRONGS_NUMBER_FIELD_NAME "StrNu"


class MediaModuleDatabase : public SQliteDatabase {
    
public:
    MediaModuleDatabase();
    // destructor not used, terminates when program does ~MediaModuleDatabase();

    int openWithType(int lType);
    int Open();
    RecordSet *getMediaListUsingLanguage(QString StrongsLanguage,/*andStrongsNumber*/QString StrongsNumber, /*andSaveResultTo*/int *result);

    int isOpen;
    QString pathComponent;
};



extern MediaModuleDatabase *gMediaModuleDatabase;

