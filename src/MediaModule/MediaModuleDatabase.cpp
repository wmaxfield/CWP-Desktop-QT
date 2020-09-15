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
//  MediaModule.cpp
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 5/28/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//
#include "Database/database.h"
#include "MediaModuleDatabase.h"
#include "bibleinfo.h"
#include "misclibrary.h"

MediaModuleDatabase *gMediaModuleDatabase;
MediaModuleDatabase::MediaModuleDatabase() : SQliteDatabase() {
    isOpen = NO;
    //openResult = 0;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int MediaModuleDatabase::openWithType(int lType)
{
    int openResult;
    
    if (isOpen) {
        Close();
    }
    
    
    if (databaseName.length()>0) {
        // if the name does not have ".bbl" appended, then add it

        if (Instr(databaseName, getSuffixStringFromTypeInt(lType))== 0) {
            databaseName = databaseName +getSuffixStringFromTypeInt(lType);
        }
    }
    
    // if we have an "/" in the database name, assume that the path component is
    // already there
    if (!Instr(databaseName, "/")) {
        
        if (pathComponent.length()) {
            databaseName = pathComponent +databaseName;
        } else {
            
            // the type of file changes where the database will be stored.  since this is a bible database type (which includes notes),
            // then we only have to look at a few changes.  The user notes are stored in the documents directory proper.
            switch (lType) {
                default:
                case TYPE_BBL_TYPE:
                case TYPE_ABP_TYPE:
                case TYPE_MORPHOLOGY_TYPE:
                    databaseName = getFullPathOfAppBundleCWPBibleDirectory() +databaseName;// /documents/bibles/ASV.bbl
                    break;
                    
                case TYPE_NOTES_TYPE:
                    databaseName = getNotesDirectory() +databaseName;// /documents/UserNotes.note
                    break;
                    
                case TYPE_COMMENTARY_TYPE:
                    databaseName = getFullPathOfAppBundleCommentaryDirectory()+ databaseName;// /documents/Commentaries/Gill.cmt
                    break;
                    
                case TYPE_MEDIA_INFO_TYPE:
                    databaseName = getFullPathOfAppBundleMediaDirectory() +databaseName;
            }
        }
    }    
    
    
    openResult = SQliteDatabase::Open();
    
    
    if (openResult==YES) {
        if (getDbType() != lType) {
            setDbType( lType);
        }
        isOpen = YES;
        return SUCCESS;
    }
    else {
        isOpen = NO;
    }
    
    return FAIL;
    
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int MediaModuleDatabase::Open() {

        return  openWithType(TYPE_MEDIA_INFO_TYPE);

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
RecordSet *MediaModuleDatabase::getMediaListUsingLanguage(QString StrongsLanguage,/*andStrongsNumber*/QString StrongsNumber,/*andSaveResultTo*/int *result)
{
// Return the list of picture names that match the strongs number
// Return True if success, False if failure

    //if DB = Nil then
    //Return Nil
    //    end if
    RecordSet   *rs=nil;//dim rs As RecordSet
    QString Query;// dim Query As String

    *result = NO;

    Query = "Select * from " MEDIA_DB_MASTER_TABLE " where " LANGUAGE_COLUMN " = '"+StrongsLanguage+"' and " STRONGS_NUMBER_COLUMN " = "+StrongsNumber+" and SelType = 'Picture' ";

    rs  =  SQLSelect(Query);

    if (rs ) {// Then
        *result = YES; // found a match
    }//end if


    return rs;
}

