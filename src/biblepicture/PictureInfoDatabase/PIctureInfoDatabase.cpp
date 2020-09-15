//
//  PIctureInfoDatabase.cpp
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 5/29/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//
#include "cwpdefines.h"
#include "PIctureInfoDatabase.h"
#include "RecordSet/recordset.h"



QString PictureInfoDatabase::getImageName() {
    RecordSet   *rs=nil;//dim rs As RecordSet
    QString Query;// dim Query As String
    
    
    Query = "Select * from  " PICTURE_DB_INFO_TABLE ;
    
    rs  = SQLSelect(Query);
    
    if (rs && rs->NumberOfRows ) {// Then
        Query = rs->Field( IMAGE_NAME_FIELD_NAME);
    } else {
        Query = "";
    }//end if
    
    
    return Query;
    

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString PictureInfoDatabase::getDescAndCopyrightString() {
    RecordSet   *rs=nil;//dim rs As RecordSet
    QString Query;// dim Query As String
    
     
    Query = "Select * from " PICTURE_DB_INFO_TABLE ;
    
    rs  =SQLSelect(Query);
    
    if (rs && rs->NumberOfRows ) {// Then
        Query = "<B>"+rs->Field(DESCRIPTION_FIELD_NAME) +"</B><br/><br/> "+rs->Field( COPYRIGHT_FIELD_NAME);
    } else {
        Query = "No information available";
    }//end if
    
    
    return Query;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int PictureInfoDatabase::openWithType(int lType)
{
    BOOL openResult;
    
    if (isOpen) {
        Close();
    }
    
    
    if (databaseName.length()==0) {
        // if the name does not have ".bbl" appended, then add it

        if (Instr(databaseName, getSuffixStringFromTypeInt(lType))== 0) {
           databaseName = databaseName +getSuffixStringFromTypeInt(lType);
        }
    }
    
    // if we have an "/" in the database name, assume that the path component is
    // already there
    if (!Instr(databaseName, "/")) {
        
        if (pathComponent.length()) {
            databaseName = pathComponent+databaseName;
        } else {
            // the type of file changes where the database will be stored.
            //since this is a bible database type (which includes notes),
            // then we only have to look at a few changes.
            // The user notes are stored in the documents directory proper.
            switch (lType) {
                case TYPE_MEDIA_INFO_TYPE:
                    databaseName = pathComponent +databaseName;
                break; // wsm added break 5/20/20

                default: // point to pictures directory by default
                case TYPE_PICTURE_INFO_TYPE:
                    databaseName =pathComponent+"Pictures/" +databaseName;
                break;
            }
        }
    }    
    
    
    openResult =SQliteDatabase::Open();
    
    
    if (openResult==YES) {

        isOpen = YES;
        return SUCCESS;
    } else {
        isOpen = NO;
    }
    
    return FAIL;
    
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int PictureInfoDatabase::Open() {
    if (getDbType()==0) {
        return   openWithType(TYPE_PICTURE_INFO_TYPE);
    }
    
    return openWithType(getDbType());
}

