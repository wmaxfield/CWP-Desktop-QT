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
//  StrongsDatabaseClass.m
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/28/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//converted
#include <QString>
#include <QObject>
#include "../shared/RecordSet/recordset.h"
#include "bibleinfo.h"
#include "misclibrary.h"
#include "strongsdatabaseclass.h"

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
StrongsDatabaseClass::StrongsDatabaseClass()
{
    databaseName = "Strongs.dic";
    _isOpen=NO;
}
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
STRONGS_STRUCTURE_TYPE * StrongsDatabaseClass::GetStrongsDefinition(int StrongsNumber ,/*Language:*/QString Language)
{
    // return the strong//s verse formatted for html, with greek or hebrew being shown.
    if (!isOpen()) {
        Open();
    }

    //----------------------------------------------------
    // get an entire verse from the database and return it as a string
    //----------------------------------------------------
    RecordSet *rs;//dim rs As RecordSet
    QString Query; //dim Query As String
    QString TableName;//dim TableName As String

       si.Number = StrongsNumber;

    if (!isOpen()) {
        si.Definition= QObject::tr("Strong's Definitions not found.  Database may be missing.");
        return &si;
    }
     si.Definition = QObject::tr("Definition not found! (Perhaps Strongs file is damaged?)");




    TableName =  HEBREW_TABLE;
    si.Language = LANGUAGE_HEBREW_C;

    if (Language == LANGUAGE_GREEK)  { // then
        TableName =  GREEK_TABLE;
        si.Language = LANGUAGE_GREEK;
    }



    Query = "Select * from "+TableName+" where " STRONGS_NUMBER_COLUMN "= "+QString::number(StrongsNumber);
    rs = SQLSelect(Query);

    if (rs != Nil )  { // then
        si.OriginalWord =rs->Field(STRONGS_ORIG_LANGUAGE_COLUMN);

        si.Phonic = rs->Field(STRONGS_PHONIC);

        si.Definition =  rs->Field(STRONGS_DEFINITION);
        si.Cross1="";
        si.Cross2="";
        si.Cross3="";

        si.Number = StrongsNumber;
    }
    else {
        if (Error()) {
            MsgBox("Error: "+ErrorMessage());
        }
    }

    return &si;
}
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
long StrongsDatabaseClass::Close()
{
    long rslt=0;

    if (isOpen()) {
        rslt = SQliteDatabase::Close();
        setIsOpen( NO);
    }
    return rslt;
}
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
void StrongsDatabaseClass::setIsOpen(BOOL s) {
    _isOpen = s;
}
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
BOOL StrongsDatabaseClass::isOpen() {
    return _isOpen;
}


//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
BOOL StrongsDatabaseClass::Open()
{
    int openResult;

    if (isOpen()==YES) {
        return SUCCESS;
    }



#ifndef DEBUG_VERSION
    // this is broken at mac osx level

    if (!FileExists(databaseName)){
        setIsOpen( NO);
        return FAIL;
    }
#endif

    if (Instr(databaseName,"/")==0)
        MsgBox("No / in db name");

    openResult = SQliteDatabase::Open();


    if (openResult==YES) {
        setIsOpen ( YES);

        initDBType(); // set the bible type if the TypeOfBible database entry is there.

        // force this to be strongs if the database return says bible
       // if (getDbType()==TYPE_BBL_TYPE) {
       //     setDbType( TYPE_DICTIONARY_TYPE);// may have to change this later
       // }
        return SUCCESS;
    }
    else {
        setIsOpen( NO);
        return FAIL;
    }




    return SUCCESS;
}



#if 0
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
BOOL StrongsDatabaseClass::Open()
{
    int openResult;

    if (isOpen()==YES) {
        return SUCCESS;
    }

        QStringList dirArray =getDirectoryListing(getFullPathOfAppBundleCWPDictionaryDirectory());
        QString DictionaryDBName="";
        BOOL found=NO;
        int index;
        if (dirArray.count()>0) {
            // English Dictionary
            for (index=0; index < dirArray.count();index++) {
                if (dirArray[index]== databaseName) {
                    found = YES;
                    break;
                }
            }
        }
            if (found){
                 DictionaryDBName = getFullPathOfAppBundleCWPDictionaryDirectory() +"/"+ STRONGS_DICTIONARY_DB_NAME;
            } else {
                dirArray = getDirectoryListing(getFullPathOfUserDocumentsCWPDirectory()+"/"+DICTIONARIES_DIRECTORY);

                for (index = 0; index < dirArray.count(); index++)
                    if (dirArray[index]==databaseName) {
                        found=YES;
                        break;
                    }

                if (found)
                    DictionaryDBName =getFullPathOfUserDocumentsCWPDirectory()+"/"+DICTIONARIES_DIRECTORY+"/"+databaseName;
              //  else
              //      MsgBox("error");
            }
            if (found && DictionaryDBName.length()>0) {
               databaseName = DictionaryDBName;
            } else {
                QString StorageLoc;
                StorageLoc = getFullPathOfUserDocumentsCWPDirectory()+"/"+DICTIONARIES_DIRECTORY;
               MsgBox( QObject::tr("Could not find ")+ databaseName+ QObject::tr(", Please fix. Expected it in \n'")+StorageLoc+"'");

                setIsOpen(NO);
                return FAIL;
            }


#ifndef DEBUG_VERSION
    // this is broken at mac osx level

    if (!FileExists(databaseName)){
        setIsOpen( NO);
        return FAIL;
    }
#endif

    if (Instr(databaseName,"/")==0)
        MsgBox("No / in db name");

    openResult = SQliteDatabase::Open();


    if (openResult==YES) {
        setIsOpen ( YES);

        initDBType(); // set the bible type if the TypeOfBible database entry is there.

        // force this to be strongs if the database return says bible
       // if (getDbType()==TYPE_BBL_TYPE) {
       //     setDbType( TYPE_DICTIONARY_TYPE);// may have to change this later
       // }
        return SUCCESS;
    }
    else {
        setIsOpen( NO);
        return FAIL;
    }




    return SUCCESS;
}
#endif


