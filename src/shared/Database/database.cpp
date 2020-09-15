/* ========================================
 *
 * Copyright Wade Maxfield, 2010-2020
 * Commercial license available
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

 * ========================================
*///
//  verse.m
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/20/10.
//  Copyright 2010,2014 Wade Maxfield. All rights reserved.
//converted
#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QVariant>
#include <QStringList>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include "cwpdefines.h"
#include "biblemodule.h"
#include "../shared/misclibrary.h"
//z#include <qsql_sqlite.h>
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void SQliteDatabase::Prepare() {



    initialized=NO;
    _dbType=0;
    showSQLSelectError=0;
    showSQLExecError=0;
    isOpen=NO;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
SQliteDatabase::SQliteDatabase()
{

   Prepare();
}

SQliteDatabase::~SQliteDatabase( )
{
    database.close();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL SQliteDatabase::deleteDB()
{
    // Close SQliteDatabase
    if (isOpen)
        database.close();

    #ifdef Q_OS_LINUX
    // NOTE: We have to store SQliteDatabase file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db.sqlite");
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #else

    // Remove created SQliteDatabase binary file
    return QFile::remove(databaseName);

    #endif
}

// ------------------------------------------------------------------------
// Type is preset to a default.  If Type is in the SQliteDatabase, then the
// SQliteDatabase version overrides the default version already set.
// ------------------------------------------------------------------------
int SQliteDatabase:: initDBType()                              // return TypeOfBible field from the SQliteDatabase, or TYPE_BBL_TYPE
{
    RecordSet *rs;//dim rs As RecordSet
    QString Query; //dim Query As String



    if (!database.isOpen()){// then
        if (Instr(databaseName,"/")) {
            if (Open()==NO)
                MsgBox ("Database Open Error: "+databaseError.databaseText());
        }
        _dbType = TYPE_1_TYPE; // default
        return _dbType;
    }
    int mye = showSQLSelectError;
    showSQLSelectError=0;

    // this must be a "modern" SQliteDatabase
    Query = "Select "  BIBLE_TYPE_FIELD;
    Query = Query +" from " BIBLE_TYPE_TABLE;

    rs = SQLSelect(Query);
    showSQLSelectError=mye;

    // ignore any error by defaulting return
    if (rs != Nil && rs->NumberOfRows >0){// then
        //rs->MoveNext();
        //int n = rs->Results->seek(0,false);
        Query=rs->IdxField(1);
        _dbType = Query.toInt();// intValue];
        delete rs;
    } else {
        _dbType = TYPE_BBL_TYPE;
        if (rs)
            delete rs;
    }
    /*else {
        _dbType = TYPE_1_TYPE;
        Query = "insert into " BIBLE_TYPE_TABLE  "("  BIBLE_TYPE_FIELD  ") values (";
        Query = Query + QString::number(_dbType)+")";
        SQLExecute(Query);
    }*/


    initialized = YES;
    return _dbType;

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void SQliteDatabase::setDbType(int t)
{
    QString Query;

    if (initDBType() != t) {
        _dbType = t;
        Query = "delete from " BIBLE_TYPE_TABLE;// delete all from the table
        SQLExecute(Query);
        Query = "insert into " BIBLE_TYPE_TABLE  "("  BIBLE_TYPE_FIELD  ") values (";
        Query = Query + QString::number(t)+")";
        SQLExecute(Query);
    }
    initialized = YES;
}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int SQliteDatabase::getDbType()
{
    if (!initialized) {
        return initDBType();
    }else {
        return _dbType;
    }

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void SQliteDatabase::setTypeBasedOnSuffixType(int SuffixType)   // set my Type based on the suffix type given
{
        switch (SuffixType) {
            case BIBLE_DATABASE_SUFFIX_TYPE:
                _dbType = TYPE_BBL_TYPE;
                break;

            case COMMENTARY_DATABASE_SUFFIX_TYPE:
                _dbType= TYPE_COMMENTARY_TYPE;
                break;

            case DICTIONARY_DATABASE_SUFFIX_TYPE:
                _dbType = TYPE_DICTIONARY_TYPE;
                break;

            case NOTES_DATABASE_SUFFIX_TYPE:
                _dbType = TYPE_NOTES_TYPE;
                break;

            case MEDIA_INFO_DATABASE_SUFFIX_TYPE:
                _dbType = TYPE_MEDIA_INFO_TYPE;
                break;

            default:
                break;
        }
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void SQliteDatabase::SQLExecute(QString SQLStatement) // execute the given statement
{


    if (!isOpen) {
       //Prepare();
        if (!Open()) {
           // if (showSQLExecError)
                MsgBox("Database Open error:"+databaseError.databaseText());

            return;
        }
    }

/*    lastReturnFromSqlite3 = sqlite3_exec(SQliteDatabase,
                                         SQLStatement,
                                         nil,
                                         nil,
                                         nil);
*/
    QSqlQuery *query = new QSqlQuery(database);

    query->exec(SQLStatement) ;

    queryError = query->lastError();

    if (queryError.type() != QSqlError::NoError) {
        if (showSQLExecError)
            MsgBox(queryError.text());
    }

    query->clear(); // clean up
    delete query;

    return ;//database.lastError();

}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
RecordSet * SQliteDatabase::SQLSelect(QString SQLStatement)  // the statement to run for the select.
{
    RecordSet *rs=nil;
    QString t;
   // char **rslt;
    int NumRows=0;
    int NumColumns=0;

    // fix bad SQliteDatabase for select
    if (databaseName.isNull()) {
        return nil;
    }

    if (!isOpen){
        if (!Open()) {
                MsgBox("Database open error:"+databaseError.databaseText());
            return nil;
        }
    }
    //if (databaseErrorString.contains("Driver not loaded")){
    //    return NULL;
    //}
    //t = database.connectionName();
    //NumRows = database.isOpen();
    //NumColumns = database.isValid();

    QSqlQuery *query = new QSqlQuery(database);                   // the last query run;
    t=query->executedQuery();
   // showSQLSelectError = YES;
    if (! query->prepare(SQLStatement)) {
        queryError = query->lastError();
       if (showSQLSelectError)
           MsgBox(queryError.text()+ " for database "+database.databaseName());
    }

    if (!query->exec()) {
        queryError= query->lastError();
        if (queryError.type() != QSqlError::NoError) {
            if (showSQLSelectError) {
                MsgBox(queryError.text());
            }

        query->clear();
        //delete query;
        //query = nil;
        delete query;
        return nil; // error
        }

    }
//    t=query->executedQuery();

    if (!query->first())
        NumRows =0;
    else
        NumRows = 1; // indicated *something*

    //QSqlQuery query("select count(*) as count from database);

   // NumRows = query->size();
    NumColumns = query->record().count();


    rs =new RecordSet( /*initWithResults:*/query, /* NumRows:*/NumRows, /*NumCols:*/NumColumns, /*SQliteDatabase:*/database );

   // will be deleted when rs is deleted--> delete query;
    //sqlite3_free_table(result);
    return rs;// receiver must release
}
// ------------------------------------------------------------------------
// if returned YES, closed ok. else NO.
// ------------------------------------------------------------------------
BOOL SQliteDatabase::Close()
{
    isOpen = NO;
    initialized = NO;

    if (database.isOpen())
        database.close();

    if (connectionName.length())
        QSqlDatabase::removeDatabase(connectionName);// remove from the list of database connections

    databaseError = database.lastError();

    if (databaseError.type() != QSqlError::NoError) {
        databaseErrorString = databaseError.text();
       return NO;
    }


    return YES;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL SQliteDatabase::Open()
{
    if (databaseName.isEmpty()) {
        return NO;
    }

    getFileName();


    // we "create" a new entry in the list by adding an SQLITE database concatenated with the
    // "filename" so it is an entry in the QSQL database list
    database = QSqlDatabase::addDatabase("QSQLITE", _fileName);

    database.setDatabaseName(databaseName);
      // Open databasee
      if (database.open()) {
          databaseError = database.lastError();
          connectionName = database.connectionName();
          isOpen = YES;
          return YES;
      } else {
          databaseError = database.lastError();
          if (databaseError.type()== QSqlError::ConnectionError) {
           // Prepare();
           // database.setDatabaseName(databaseName);
           // getFileName();
            if (!database.open()) {
                databaseError = database.lastError();
                MsgBox("Unable to open "+_fileName+", error "+databaseError.databaseText());
            } else {
                connectionName = database.connectionName();
                isOpen = YES;
                return YES;
            }
          }

           isOpen = NO;
          return NO;
      }
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int SQliteDatabase::CreateDatabaseFile()                 // create the file given in *DatabaseName
{
    if (!databaseName.isEmpty()) {
        Open();
        return Close();
    }

    return -1;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
long long SQliteDatabase::LastRowID()
{
    long long lastInsert=0;

    QSqlQuery *myQuery =new QSqlQuery(database);
    lastInsert =myQuery->lastInsertId().toInt();
    //query->         SELECT last_insert_rowid()
    delete myQuery;


    return lastInsert;


}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString SQliteDatabase::getFileName() {
    if (databaseName.length()==0) {
        return "";
    }

    QFileInfo f(databaseName);
    _fileName = f.fileName();
    return _fileName;
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString SQliteDatabase::ErrorMessage()                  // the error message for the error code
{
    return database.lastError().databaseText();//[NSString stringWithFormat:@"There was a problem reading or writing your data. The error was %s",sqlite3_errmsg(self.SQliteDatabase)];
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
long SQliteDatabase::errorCode()                          // error code generated
{


   return databaseError.type();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL SQliteDatabase::Error()                              // if returns true, is an error
{
    return  errorCode()!=0; //(lastReturnFromSqlite3>SQLITE_OK && lastReturnFromSqlite3<SQLITE_ROW);
}

