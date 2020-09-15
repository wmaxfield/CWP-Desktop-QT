//
//  CommentaryDatabaseClass.m
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/25/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//converted

#include "commentarydatabaseclass.h"
#include "biblemodule.h"
#include "HTMLModule.h"
#include "misclibrary.h"
#include <QStringList>
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
CommentaryDatabaseClass::CommentaryDatabaseClass() :  BibleDatabaseClass()
{
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int  CommentaryDatabaseClass::Open(QString NameOfCommentary)
{
    int rslt;

    if (Instr(NameOfCommentary,  COMMENTARY_DATABASE_SUFFIX )) {
        QStringList n = NameOfCommentary.split(".");
        NameOfCommentary = n[0];
       // super.dbType = TYPE_COMMENTARY_TYPE ;

        _BibleName = NameOfCommentary;
        rslt= BibleDatabaseClass::Open();// pick up other code benefits
        if (rslt==YES && getDbType()==TYPE_BBL_TYPE) {
            setDbType(TYPE_COMMENTARY_TYPE);
        }
    }

    return NO;

}

#define ACTS_TABLE "Acts"

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void CommentaryDatabaseClass::initAndOpenWithDBName(QString dbName)
{

    QStringList n = dbName.split(".");
        setDbType(COMMENTARY_DATABASE_SUFFIX_TYPE);
        databaseName= dbName;

        if (n.count()) {
            _BibleName = n[0] ;
        }
        int Type = getDbType();

        openWithType(Type);

}

