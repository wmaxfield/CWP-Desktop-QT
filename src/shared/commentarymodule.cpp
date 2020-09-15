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
//  CommentaryModule.m
//  CWP-Student
//
//  Created by WADE MAXFIELD on 12/2/10.
//  Copyright 2010 Wade Maxfield All rights reserved.
//converted

#include "commentarymodule.h"
#include "preferencesmodule.h"
#include "PreferencesLibrary.h"
#include "notesdbclass.h"
#include "../shared/bookdbclass.h"
int gNumberOfCommentaries;

#include <QList>

extern QList<BookHandlerClass *>      Books;
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
BOOL removeCommentaryFromSelectedArrayByName(QString Commentary)
{
    int index,index2;
    BOOL foundit=NO;

    for (index=0; index < Preferences->numberOfSelectedCommentaries; index++) {
        if (StringsAreEqual( Preferences->SelectedCommentaries[index].Name, Commentary)) {

            BookHandlerClass *bk ;
            bk = Books.at(Preferences->SelectedCommentaries[index].Index);
            bk->selected = NO;

            for (index2=index; index2 < Preferences->numberOfSelectedCommentaries-1; index2++) {
                Preferences->SelectedCommentaries[index2] = Preferences->SelectedCommentaries[index2+1];

            }
            Preferences->numberOfSelectedCommentaries--; //back out the number of books
            foundit = YES;
            break;
        }
    }
    saveSelectedCommentaries();

    return foundit;

}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
BOOL isCommentaryNameInSelectedArray(QString Commentary)
{
    int index;
    QString selectCommentary;

    for (index=0; index < Preferences->numberOfSelectedCommentaries; index++) {
        selectCommentary =  Preferences->SelectedCommentaries[index].Name;
        if (StringsAreEqual(selectCommentary, Commentary)) {
            return YES;
        }
    }

    return NO;
}




// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void saveSelectedCommentaries(void)
{
    int index;
    int NumSelectedBooks=Preferences->numberOfSelectedCommentaries;
    QString CommentaryName;

    for (index=0; index < NumSelectedBooks; index++) {
        CommentaryName = Preferences->SelectedCommentaries[index].Name;
        SavePrefVariable(CommentaryKey(index+1), CommentaryName);
        SavePrefVariable(CommentaryTypeKey(index+1), QString::number(Preferences->SelectedCommentaries[index].Type));
    }

    SavePrefVariable( INI_NUM_SELECTED_COMMENTARIES, QString::number(Preferences->numberOfSelectedCommentaries));

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void getSelectedCommentaries(void)
{
    extern int OpenBookAndPutInSelectedArray(QString Type, QString Name,int index);// preferenceslibrary.m

    Preferences->numberOfSelectedCommentaries=0; // set up for the book opening below, which increments this variable
    int NumSelectedCommentaries= GetPrefVariable( INI_NUM_SELECTED_COMMENTARIES).toInt();

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    for (int index = 0 ; index < NumSelectedCommentaries ; index++) {
        // get the bible from the database
        QString CommentaryName =GetPrefVariable(CommentaryKey(index+1)) ;
        //QString DocType = getSuffixStringFromTypeString( GetPrefVariable(CommentaryTypeKey(index+1)));
        QString fileName;
        QString DocType = COMMENTARY_DATABASE_SUFFIX;

 //       if (StringsAreEqual(DocType , COMMENTARY_DATABASE_SUFFIX)) { //then

        fileName = getFullPathOfUserDocumentsCWPDirectory()+"/"+CommentaryName+DocType;
        if (FileExists(fileName)) {
                OpenBookAndPutInSelectedArray(DocType,CommentaryName,index+Preferences->getNumberOfSelectedBooks());
            }else {
                fileName = getFullPathOfPreferencesCommentaryDirectory()+"/"+CommentaryName+DocType;
                if (FileExists(fileName)){
                    OpenBookAndPutInSelectedArray(DocType,CommentaryName,index+Preferences->getNumberOfSelectedBooks());
                }else{
                    fileName = getFullPathOfUserDocumentCWPCommentaryDirectory()+"/"+CommentaryName+DocType;
                    if (FileExists(fileName)){
                        OpenBookAndPutInSelectedArray(DocType,CommentaryName,index+Preferences->getNumberOfSelectedBooks());
                    }else{
                        fileName = getFullPathOfAppBundleCWPDirectory()+"/"+CommentaryName+DocType;
                        if (FileExists(fileName)){
                            OpenBookAndPutInSelectedArray(DocType,CommentaryName,index+Preferences->getNumberOfSelectedBooks());
                        }
                    }
                }
            }

       // } //end if
    }//next




}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL AddCommentaryToSelectedArrayByIndexNumber(int CommentaryNumber)
{
    int NumBooks;
    NumBooks = Preferences->numberOfSelectedCommentaries++;
    CommentaryDatabaseClass *bbl;
    BookHandlerClass *bk;

    bk = Books [CommentaryNumber];
    bbl =(CommentaryDatabaseClass*) bk->Database;
    if (bbl) {
        Preferences->SelectedCommentaries[NumBooks].Name=bbl->_BibleName;
        Preferences->SelectedCommentaries[NumBooks].Type=bbl->getDbType();
        Preferences->SelectedCommentaries[NumBooks].Index = CommentaryNumber;
        bbl->BibleIndexNumber = NumBooks; // the index number in the selectedBooks[] array
        bk->selected = YES;
        saveSelectedCommentaries();
        return YES;

    }else {
        Preferences->SelectedCommentaries[NumBooks].Index=-1;
        Preferences->numberOfSelectedCommentaries--;
    }



    return NO;

}

int getIndexOfCommentaryInCommentaries(QString bible)
{
    int index;

    for (index =0; index < Books.count(); index++) {
        if (StringsAreEqual(((BibleDatabaseClass*)Books [index])->_BibleName, bible))
            return index;
    }
    return -1;
}



//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void addCommentaryToSelectedArrayByName(QString bible)
{
    int index;


    index = getIndexOfCommentaryInCommentaries(bible);
    if (index>= 0) {
        AddCommentaryToSelectedArrayByIndexNumber(index);
    }

}
