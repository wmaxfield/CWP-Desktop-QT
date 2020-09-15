#ifndef DATABASE_H_
#define DATABASE_H_

// DATABASE_H
//
//  verse.h
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/20/10.
//  Copyright 2010, 2014 Wade Maxfield. All rights reserved.
//
// Notes:
//      The way Qt does databases is fundamentally broken, but it does
//  allow you to bring up and tear down database objects without regard to
//  keeping an object "live"
//      There is only one database object in Qt.  It keeps a list of connections.
//  Your object is in this list, and you must manage it correctly.
//      If you try to recreate the master database object, you will find all other
//  connections to your database destroyed.


#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
//#include <QFile>
//#include "ivbDefines.h"
//#include <sqlite3.h>
#include "../shared/RecordSet/recordset.h"
#include "cwpdefines.h"

#if CPP11 != 1
#ifndef nullptr
#define nullptr 0
#endif
#endif

#ifndef nil
#define nil nullptr
#endif

#define SUCCESS YES
#define FAIL    NO


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------


// use this #define as your beginning type, or make sure
// none of your types are this value
#ifndef TYPE_1_TYPE
#define TYPE_1_TYPE 1  // .pgm
#endif

#ifndef TYPE_2_TYPE
#define TYPE_2_TYPE 2 // .db files
#endif

#define TYPE_3_TYPE 3 //.tmp file
#define TYPE_4_TYPE 4 //.tmp file



class RecordSet;

class SQliteDatabase
{

public:
    explicit    SQliteDatabase();
                ~SQliteDatabase();
    void        Prepare();                  // set up this object, or re-set
    //BOOL   AutoCommit();                    // return the autocomit value
    //void   setAutoCommit(BOOL ac);             // set the autocommit


    int         CreateDatabaseFile();                 // create the file given in *databaseName

    BOOL        Close();                    // close the current database


    BOOL        deleteDB();
    BOOL        Error();                              // if returns true, is an error
    QString     ErrorMessage();                 // the error message for the error code

    long        errorCode();
    BOOL        Open();                               // open the database in databaseName
    BOOL    isOpen;         // when YES, is open


    void        SQLExecute(QString SQLStatement); // execute the given statement
    RecordSet   *SQLSelect(QString SQLStatement);  // the statement to run for the select.


    long long   LastRowID();                    // return the id of the last row added to the database

    int         initDBType();                              // return Type field from the database, or TYPE_1_TYPE, set into Type field


    void        setDbType(int t);

    int         getDbType(void);
    void        setTypeBasedOnSuffixType(int SuffixType) ;

    QString    encryptionKey;         // encryption key for encrypted database (NOT AVAILABLE IN THIS VERSION FOR NOW)


    QSqlDatabase  database;
    QString     databaseName;      // the full path to the database file
    QString     getFileName();           // the file name without the path
    QString     connectionName;         // the name in the connection list for Qt Databases
    BOOL        initialized;

    BOOL        showSQLSelectError;      // when true, we will complain with a MsgBox on SQLSelect
    BOOL        showSQLExecError;       // when true, will complain with a MsgBox on SQLExec() errors

    QSqlError   queryError;
    QSqlError   databaseError;
    QString     databaseErrorString;


private:
    int         _dbType;                // type of the database
    QString     _fileName;
    
};

#endif

