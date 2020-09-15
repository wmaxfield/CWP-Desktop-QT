#ifndef PICTURE_INFO_DATABASE_H_
#define PICTURE_INFO_DATABASE_H_
//
//  PIctureInfoDatabase.h
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 5/29/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//

//#import <Foundation/Foundation.h>
#include <QString>
#include "BibleDatabaseClass/bibledatabaseclass.h"
#include "structs.h"
#include "bibleinfo.h"
#include "Database/database.h"

#define PICTURE_DB_INFO_TABLE "Info"

//(ID integer PRIMARY KEY, ImgName varchar, FileName varchar, CopyrightInfo varchar, Desc varchar);


#define IMAGE_NAME_FIELD_NAME "ImgName"
#define FILENAME_FIELD_NAME "FileName"
#define COPYRIGHT_FIELD_NAME "CopyrightInfo"
#define DESCRIPTION_FIELD_NAME "Desc"


class PictureInfoDatabase : public SQliteDatabase {
public:
    int isOpen;
    QString pathComponent;
  


    int openWithType(int lType);
    int Open() ;
    QString getDescAndCopyrightString();
    QString getImageName() ;
};


#endif
