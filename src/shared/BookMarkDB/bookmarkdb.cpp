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
//  BookMarkDB.m
//  Crossword-Try3
//
//  Created by WADE MAXFIELD on 10/28/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//converted

#include "BookMarkDB.h"
#include "misclibrary.h"
#include "../shared/RecordSet/recordset.h"

#include <QStringList>

extern BookMarkDB *gBookMarkDB; // in the misclibrary.m file
extern BookMarkDB *gBookBookMarkDB; // in the initalization.m file


BookMarkDB::BookMarkDB() : SQliteDatabase()
{

}

//NOTE: TOC==TopicOrCategory

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BookMarkDB::TopicOrCategoryKey(int number) {
     QString str;
    return str.sprintf(  "TOC%d",number);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BookMarkDB::initWithDBName(QString dbName, /*andType */int Type)
{
    Q_UNUSED(Type);
    RecordSet *rs;
  // if ((self=[super init])) {

        databaseName = dbName;
        Open();// open the database

        QString Query = "Select * from " BOOKMARKS_TABLE " limit 1" ;

      rs=  SQLSelect(Query);

        if (rs == nullptr) {
            CreateTables();// just in case
            if (queryError.type()!=QSqlError::NoError) {
                return ;
            }
            // add a "general" topic
            addTopic("General");
        } else {
           // if (getDbType() == TYPE_BBL_TYPE) {
           //     setDbType( Type);
           // }
        }

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BookMarkDB::CreateTables() // create the database tables (assuming they don't yet exist)
{
    QString Query;//Dim Query as String


    Query = "create table " BOOKMARKS_TABLE " (ID integer NOT NULL PRIMARY KEY,TOC varchar, Name varchar, Value varchar)";
    SQLExecute(Query);

    if (queryError.type() != QSqlError::NoError)
        goto ProjectError;



    return YES;

ProjectError:
    MsgBox ( "There was an error creating the BookMark Database.");
    return (NO);

}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BookMarkDB::BookMarkKey(int  TopicOrCategoryNumber ,int BookMarkNumber) { // Asinteger) // Asstring
    QString str;
    return str.sprintf( "BookMark:%d:%d",TopicOrCategoryNumber,BookMarkNumber);
}





// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BookMarkDB::deleteBookMark( QString BookMark,/* underTopic*/ QString Topic)
{
    return DeleteVariable(Topic, /*VariableName:*/BookMark);
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BookMarkDB::deleteBookMarkTopicOrCategory(QString TopicOrCategory) // AsString)
{

    TopicOrCategory = dbEscapeSingleQuoteString(TopicOrCategory);
    // the following deletes the possible bookmarks
    QString  Query ="delete from " BOOKMARKS_TABLE" where TOC = '"+TopicOrCategory+"'";


    SQLExecute(Query);
    if (Error()) {
        return NO;
    }

    // the following deletes the topic
    Query ="delete from " BOOKMARKS_TABLE" where TOC = 'TopicName' and Name='"+TopicOrCategory+"'";

    SQLExecute(Query);
    if (Error()) {
        return NO;
    }

    return YES;
}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BookMarkDB::DeleteVariable( QString TopicOrCategory ,QString VariableName)
{
    TopicOrCategory = dbEscapeSingleQuoteString(TopicOrCategory);
    VariableName = dbEscapeSingleQuoteString(VariableName);
    QString  Query ="delete from " BOOKMARKS_TABLE" where TOC = '"+TopicOrCategory+"' and Name = '"+VariableName+"'";

    SQLExecute(Query);
    if (Error()) {
        return NO;
    }

    return YES;
}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BookMarkDB::getVariableByName(QString VariableName, QString TopicOrCategory)
{
    QString Query;
    RecordSet *rs;
    QString myVar;

    TopicOrCategory = dbEscapeSingleQuoteString(TopicOrCategory);
    VariableName = dbEscapeSingleQuoteString(VariableName);

    Query ="select Value from " BOOKMARKS_TABLE " where TOC = '"+TopicOrCategory +"' and Name='"+VariableName+ "' limit 1";

    rs = SQLSelect(Query);

    if ((rs != nullptr )) {
        if ((rs->FieldCount()!= 0 )) {
            myVar = rs->Field("Value");
            if ((myVar != nullptr )) {
                return myVar;
            }
        }
    }

    return "";
}




// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BookMarkDB::saveVariable(QString TopicOrCategory, QString VariableName, QString myVar)
{

    VariableName = dbEscapeSingleQuoteString(VariableName);
    myVar = dbEscapeSingleQuoteString(myVar);
    TopicOrCategory = dbEscapeSingleQuoteString(TopicOrCategory);

    QString  Query = "delete from " BOOKMARKS_TABLE " where TOC = '" +TopicOrCategory +"' and Name = '" + VariableName +"'";


    SQLExecute(Query);

    // the variable does not exist, insert it
    Query = "insert into " BOOKMARKS_TABLE "(ID,TOC,Name,Value) values (NULL,'"+ TopicOrCategory+"','"+VariableName+"','"+myVar+"') ";

    SQLExecute(Query);

    if ((queryError.type()!=QSqlError::NoError)) {
        MsgBox( "An error occured saving a bookmark.  The error was "+ErrorMessage());
        return NO;
    }


    return YES;


}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int BookMarkDB::getNumBookMarksInTopicOrCategory(QString TopicOrCategory)
{
    QString Query;
    RecordSet *rs;
    QString myVar;

    TopicOrCategory = dbEscapeSingleQuoteString(TopicOrCategory);
    Query = "select count as NumBookMarks from " BOOKMARKS_TABLE " where TOC = '"+TopicOrCategory+"'";
    rs = SQLSelect(Query);

    if ((rs != nullptr )) {
        if ((rs->FieldCount() != 0 )) {
            myVar = rs->IdxField(1);// only one field
            if ((myVar != nullptr )) {
                return myVar.toInt();
            }
        }

    }

    return 0;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int BookMarkDB::getNumTopics()
{
    QString Query;
    RecordSet *rs;
    QString myVar;

    Query ="select count(*) as NumTopics from " BOOKMARKS_TABLE " where TOC = '"  CATEGORY_TOPIC_NAME "'";
    rs = SQLSelect(Query);

    if ((rs != nullptr )) {
        if ((rs->FieldCount() != 0 )) {
            myVar = rs->IdxField(1);// only one field
            if ((myVar != nullptr )) {
                return myVar.toInt();
            }
        }

    }

    return 0;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL  BookMarkDB::addTopic(QString TopicName)
{
    BOOL result;

    result =saveVariable( CATEGORY_TOPIC_NAME, /*VariableName:*/ TopicName,/* Value:*/ TopicName ); // variable name matches variable value
    return result;
}

// ------------------------------------------------------------------------
// get the topics array and return it
// ------------------------------------------------------------------------
QStringList BookMarkDB::getTopicArray()
{
    QStringList TopicArray;


    QString Query;
    RecordSet *rs;
    QString myVar;

    Query = "select Name from " BOOKMARKS_TABLE " where TOC = '" CATEGORY_TOPIC_NAME "' order by Name";
    rs = SQLSelect(Query);

    if ((rs != nullptr )) {
        if ((rs->FieldCount() != 0 )) {
            while (! rs->atEOF() ) {
                myVar = rs->IdxField(1);// only one field
                TopicArray.append(myVar);
                rs->MoveNext();
            }
        }
    }

    return TopicArray;
}



// ------------------------------------------------------------------------
// Return an array of strings of the form:
//  Hebrews 1:1
//  Hebrews 1:2
// or, if is a textbook bookmark db
//  1 1:1
//  3 1:34
// ------------------------------------------------------------------------
QStringList  BookMarkDB::getBookmarkNamesArrayForTopic(QString Topic)
{
    QStringList BookmarkArray;

    QString Query;
    RecordSet *rs;
    QString myVar;


    Topic = dbEscapeSingleQuoteString(Topic);
    Query = "select Name from " BOOKMARKS_TABLE " where TOC = '"+Topic+"' order by Name";


    rs = SQLSelect(Query);
    if ((rs != nullptr )) {


        if ((rs->FieldCount() != 0 )) {
            while (! rs->atEOF() ) {
                myVar = rs->IdxField(1);// only one field
                BookmarkArray.append(myVar);
                rs->MoveNext();
            }
        }
        rs->done();// release sqlite table NOW
        delete rs;
    }

    return BookmarkArray;

}
QList<ChapterVerseStructType> BookMarkDB::getBookmarkNamesCVSArrayForTopic(QString Topic) {

    QList<ChapterVerseStructType> t;

    QString Query;
    RecordSet *rs;
    QString myVar;
    ChapterVerseStructType cvs;
    QStringList a;


    Topic = dbEscapeSingleQuoteString(Topic);
    Query = "select Value from " BOOKMARKS_TABLE " where TOC = '"+Topic+"' order by Name";


    rs = SQLSelect(Query);
    if ((rs != nullptr )) {


        if ((rs->FieldCount() != 0 )) {
            while (! rs->atEOF() ) {
                myVar = rs->IdxField(1);// only one field
                a = myVar.split(":");
                if (a.count()>2){
                    cvs.BookNumber = a[0].toInt();
                    cvs.Chapter = a[1].toInt();
                    cvs.Verse = a[2].toInt();
                }
                t.append(cvs);
                //BookmarkArray.append(myVar);
                rs->MoveNext();
            }
        }
        rs->done();// release sqlite table NOW
        delete rs;
    }

    return t;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BookMarkDB::addBookmarkUnderTopic(QString Topic, QString book, QString chapter, QString verse, unsigned short isTextBook)
{
    extern QString BibleBook(int BookNumber);// BibleInfo.m
    //extern QString TextBook(int BookNumber);// BibleInfo.m

    if (isTextBook) {
        saveVariable(Topic, /*VariableName:*/book+" "+chapter+":"+verse, /*Value:*/book+":"+chapter+":"+verse);
    } else {

    saveVariable(Topic,/* VariableName*/ BibleBook(book.toInt())+" "+chapter+":"+verse, /*Value:*/book+":"+chapter+":"+verse);
    }

}


