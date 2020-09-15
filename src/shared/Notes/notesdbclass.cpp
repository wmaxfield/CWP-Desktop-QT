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
//  NotesDB.m
//  Crossword-Student
//
//  Created by WADE MAXFIELD on 11/14/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
// converted

#include "notesdbclass.h"
#include "notesLibrary.h"
#include "misclibrary.h"
#include "HTMLModule.h"

#include <QStringList>


#define ACTS_TABLE "Acts"

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
NotesDBClass::NotesDBClass() : BibleDatabaseClass()
{
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
NotesDBClass::NotesDBClass(QString dbName) : BibleDatabaseClass()
{
    initAndOpenWithDBName( dbName);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesDBClass:: checkForCompletenessAndCorrectIfNecessary()
{
    RecordSet *rs;

    QString Query = "Select * from " ACTS_TABLE " limit 1" ;
    rs= SQLSelect(Query);
    if (rs == nil) {
        createTables();// just in case
        setTitle(_BibleName ); // set the bible name, just in case
    }
}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL NotesDBClass::openWithType(int lType)
{
    BibleDatabaseClass::openWithType(lType);
    checkForCompletenessAndCorrectIfNecessary();
    if (Error()) {
        return FAIL;
    }else {
        isOpen = YES;
        return SUCCESS;
    }
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesDBClass::initAndOpenWithDBName(QString dbName)
{
    QStringList n = mySplit(dbName,".");
    //int lType;

         databaseName= dbName ;
         if (n.count()) {
            _BibleName = n[0] ;
        }

         openWithType(TYPE_NOTES_TYPE);

         // the following was done in openWithType
         checkForCompletenessAndCorrectIfNecessary ();

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int NotesDBClass::Open(QString NameOfNote)
{
    int rslt=0;

    if (Instr(NameOfNote,  NOTE_DATABASE_SUFFIX )) {
        QStringList n = mySplit(NameOfNote,".");
        NameOfNote = n[0];

        _BibleName = NameOfNote;
        // super.dbType = TYPE_NOTES_TYPE ;// this must be before the "open" call
        if (!isOpen)
           rslt= openWithType(TYPE_NOTES_TYPE);// pick up other code benefits
        checkForCompletenessAndCorrectIfNecessary();
        return rslt;
    }

    return NO;

}

