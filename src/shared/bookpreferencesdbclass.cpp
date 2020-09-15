//
//  BookPreferences.m
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 6/23/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//converted

#include "bookpreferencesdbclass.h"
#include "misclibrary.h"
#include "HTMLModule.h"
#include "../shared/settings/myqsettings.h"



BookPreferencesDBClass::BookPreferencesDBClass()
{

  #if 0
    extern QString getBookPrefDBName();//miscLibrary.m

    databaseName = getBookPrefDBName() ;
        Open();// open the database

        if (getDbType()==TYPE_BBL_TYPE) {
            setDbType( TYPE_PREFERENCES_TYPE);
        }

       QString Query = "Select * from " BOOK_PREFERENCES_TABLE " limit 1" ;
        SQLSelect(Query);
        if (Error()) {
            CreateTables();// just in case
         }
 #endif

        void initQSettings(void);
        initQSettings();// initialize the QSettings system
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BookPreferencesDBClass::BookPreferredTableStyle() {
    return _BookPreferredTableStyle;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BookPreferencesDBClass::setBookPreferredTableStyle(QString lPreferredTableStyle) {
    _BookPreferredTableStyle = lPreferredTableStyle;
    if (!loadingPrefs) {
        savePrefVariableWithCategory(CATEGORY_PREFERENCE , INI_BOOK_PREFERRED_TABLE_STYLE ,dbEscapeSingleQuoteString(BookPreferredTableStyle()));
    }
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int BookPreferencesDBClass::currentVerse() {
    return _currentVerse;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BookPreferencesDBClass::setCurrentVerse(int currentVersel) {
    _currentVerse = currentVersel;
    if (!loadingPrefs) {
        savePrefVariableWithCategory( CATEGORY_PREFERENCE , BOOK_CURRENT_VERSE_NUMBER ,QString::number(_currentVerse));
    }
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int BookPreferencesDBClass::currentBook() {
    return  _currentBook;
}
// ------------------------------------------------------------------------
void BookPreferencesDBClass::setCurrentBook(int currentBookl) {
    _currentBook = currentBookl;

    if (!loadingPrefs) {
        savePrefVariableWithCategory( CATEGORY_PREFERENCE , BOOK_CURRENT_BOOK_NUMBER ,QString::number(_currentBook));
    }

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int BookPreferencesDBClass::currentChapter( ){
    return _currentChapter;
}
// ------------------------------------------------------------------------
void BookPreferencesDBClass::setCurrentChapter(int CurrentChapter) {
    _currentChapter = CurrentChapter;
    if (!loadingPrefs) {                                                   
       savePrefVariableWithCategory( CATEGORY_PREFERENCE , BOOK_CURRENT_CHAPTER_NUMBER ,QString::number(_currentChapter));
    }

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BookPreferencesDBClass:: LoadPreferences()
{
    BOOL result;

    loadingPrefs = YES;
    _currentBook = getPrefVariableByName( BOOK_CURRENT_BOOK_NUMBER , CATEGORY_PREFERENCE ,&result).toInt();
    _BookPreferredTableStyle = getPrefVariableByName( INI_BOOK_PREFERRED_TABLE_STYLE , CATEGORY_PREFERENCE,&result);
    loadingPrefs = NO;
}




// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BookPreferencesDBClass::getPrefVariableByName(QString varName ,/*Category*/QString category, /* Result:*/BOOL *result)
{
   // extern QString getSetting(QString group,QString Key, QString Default);

    QString s =getSetting(category,varName,"");

    if (s=="")
        *result=NO;
    else
        *result = YES;

    return s;

#if 0
    // usage  Variable = GetVariable(TableName,DB,Result,Category,VariableName
    // if result = false then bail
    //dim Result as RecordSet
    RecordSet *rs;
    //dim k as integer
   QString Query;//Dim Query as String
   QString myVar;//Dim myVar as Variant
                    //dim myDB As REALSQLDatabase

    if (database == Nil ) {//then
        return "";
    }//end if

    Query =  "select Value from " BOOK_PREFERENCES_TABLE " where Category = '"+category+"' and Name = '"+varName+"' limit 1";

    rs = SQLSelect(Query);
    // NSLog("rs retain count=%d",rs->retainCount]);
    if (rs != Nil ) {//Then
        if (rs->FieldCount() > 0 ){// Then
            myVar = rs->Field("Value");//.StringValue // wsm .Value returns nil
            if (myVar != Nil ) {//Then
                *result = YES;
                return myVar;
            }//end if
        }//end if
    }// end if

    *result = NO;
    return "";
#endif



}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BookPreferencesDBClass::savePrefVariableWithCategory(QString Category, /* VarName*/QString VariableName, /* Value*/QString myVar)
{
     setSetting(Category,VariableName,myVar);
    return YES;
#if 0
    //return True if success, False if error
    QString Query;// dim Query as String

    if (database == Nil ){// then
        return NO;
    } // end if

    myVar = myReplace(myVar, QUOTE,"\\\"");
    myVar = myReplace(myVar,"'","\'");


    Query =  "delete from " BOOK_PREFERENCES_TABLE " where Category = '"+Category+"' and Name = '"+VariableName+"'";
    SQLExecute(Query);

    if (Error()) { // then
        MsgBox(  "An error occured saving a book preference.  The error was "+ErrorMessage());
        return NO;
    } // end if



    // the variable does not exist, insert it
    Query =  "insert into " BOOK_PREFERENCES_TABLE "(ID,Category,Name,Value) values (NULL,'"+Category+"','"+VariableName+"','"+myVar+"')";
    SQLExecute(Query);

    if (Error()) { // then
        MsgBox("An error occured saving a book preference.  The error was '"+ErrorMessage()+"'");
        return NO;
    } // end if



    return YES;

#endif
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BookPreferencesDBClass:: CreateTables() // create the database tables (assuming they don't yet exist)
{

  #if 0
   QString Query;//Dim Query as String


    Query = "create table " BOOK_PREFERENCES_TABLE " (ID integer NOT NULL PRIMARY KEY,Category varchar, Name varchar, Value varchar)";
    SQLExecute(Query);

    if (Error())
        goto ProjectError;

    return YES;

ProjectError:
    MsgBox ( "There was an error creating the Book Preferences.");
    return (NO);
#endif
   return YES;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BookPreferencesDBClass::DropTables()
{
#if 0
   QString Query;//Dim Query as String

    Query = "drop table if exists " BOOK_PREFERENCES_TABLE;

    SQLExecute(Query);

    return Error();
#endif
    return 0;
}

