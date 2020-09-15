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
//  notesLibrary.m
//  Crossword-Try3
//
//  Created by WADE MAXFIELD on 11/14/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
// converted
#include "notesLibrary.h"
#include "misclibrary.h"
#include "notesLibrary.h"
#include "notesdbclass.h"
#include "PreferencesLibrary.h"
#include "preferencesmodule.h"

NotesDBClass *gNotes[MAX_NUM_BIBLES];
int gNumberOfNotes;


NotesDBClass *getNotesDB(int NoteDBIndex)
{
    return gNotes[NoteDBIndex];
}
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
BOOL removeNoteFromSelectedArrayByName(QString bible)
{
    int index,index2;
    BOOL foundit=NO;
    int numSelectedNotes= Preferences->getNumberOfSelectedNotes();

    for (index=0; index < numSelectedNotes; index++) {
        if (StringsAreEqual(  Preferences->SelectedNotes[index].Name, bible)) {
            for (index2=index; index2 < numSelectedNotes-1; index2++) {
                Preferences->SelectedNotes[index2] = Preferences->SelectedBooks[index2+1];

            }
            Preferences->setNumberOfSelectedNotes(numSelectedNotes-1); //back out the number of books
            foundit = YES;
            break;
        }
    }
    saveSelectedNotes();

    return foundit;

}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
BOOL isNoteNameInSelectedArray(QString note)
{
    int index;
    QString selectNote="";
    int numNotes =Preferences->getNumberOfSelectedNotes();

    for (index=0; index < numNotes; index++) {
        selectNote =  Preferences->SelectedNotes[index].Name;
        if (StringsAreEqual(selectNote, note)) {
            return YES;
        }
    }

    return NO;
}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
int OpenPersonalNotesByNameAndPutInSelectedArray(QString Name)
{
    //int result;


        int index;
        NotesDBClass *bbld;

        for (index=0; index < gNumberOfNotes; index++) {

            bbld = gNotes[index];
                if (StringsAreEqual(bbld->_BibleName, Name)) {
                    //result =bbld->openWithType(TYPE_NOTES_TYPE );
                    //if (result == FAIL) {
                   //     MsgBox(bbld->ErrorMessage());
                   // }
                    if (AddNoteToSelectedArrayByIndexNumber(index,Preferences->numberOfNotesInSelectedNotesArray)==NO)
                        return FAIL;
                    //Preferences->incrementNumberOfSelectedNotes();<-- done above
                    Preferences->numberOfNotesInSelectedNotesArray++;
                    return SUCCESS;
                }
            }
    return FAIL;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void saveSelectedNotes(void)
{
    int index;
    int NumSelectedNotes=Preferences->getNumberOfSelectedNotes();
    QString NoteName;

    for (index=0; index < NumSelectedNotes; index++) {
        NoteName = Preferences->SelectedNotes[index].Name;
        if (NoteName != "") {
            SavePrefVariable(NoteKey(index+1), NoteName);
            SavePrefVariable(NoteTypeKey(index+1), QString::number(Preferences->SelectedNotes[index].Type));

        }
    }

   // SavePrefVariable( INI_NUM_SELECTED_NOTES, QString::number(Preferences->getNumberOfSelectedNotes()));

}

// gNotes must already be filled in
void getSelectedNotes(void)
{
    extern int OpenBookAndPutInSelectedArray(QString Type, QString Name, int index);// preferenceslibrary.m
    int index;
    QString BibleName; //dim BibleName As String
    QString DocType; //dim DocType As String
    int FoundNote=0; // number of notes found
   // int r=YES; //dim r As Integer

    //Preferences->setNumberOfSelectedNotes(0); // set up for the book opening below, which increments this variable
    int NumSelectedNotes= Preferences->getNumberOfSelectedNotes();

    //--------------------------------------------------------------------------
    // open the notes
    //--------------------------------------------------------------------------

    for (index = 0 ; index < NumSelectedNotes ; index++) {
        // get the bible from the database
        BibleName =GetPrefVariable(NoteKey(index+1)) ;
        if (BibleName == "") {
            Preferences->decrementNumberOfSelectedNotes();
            if (Preferences->getNumberOfSelectedNotes()<=0) {
                break;
            }
        }
        DocType = getSuffixStringFromTypeString( GetPrefVariable(NoteTypeKey(index+1)));


        if (StringsAreEqual(DocType ,NOTE_DATABASE_SUFFIX)) { //then
            QString fileName;
            FoundNote++;

            fileName = getNotesDirectory()+"/"+BibleName;
            fileName = fileName +DocType;
            if (FileExists(fileName)) {
                OpenPersonalNotesByNameAndPutInSelectedArray(BibleName);
            }

        } //end if
    }//next


}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int getIndexOfNoteInNotes(QString noteName)
{
    int index;
    NotesDBClass *note;

    for (index=0; index < gNumberOfNotes; index++) {
                note = gNotes [index];
                    if (CompareStrings(noteName, note->_BibleName)==0) {
                        return index;
                    }
                }


    return -1;
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL AddNoteToSelectedArrayByIndexNumber(int noteNumber, int selectedIndex)
{
    //int NumBooks;
    //NumBooks = Preferences->getNumberOfSelectedNotes();
    NotesDBClass *bbl;

   // NumBooks++;
   // Preferences->setNumberOfSelectedNotes(NumBooks);

    bbl = gNotes [noteNumber];
    if (bbl) {
        Preferences->SelectedNotes[Preferences->numberOfNotesInSelectedNotesArray].Name=bbl->_BibleName;
        Preferences->SelectedNotes[Preferences->numberOfNotesInSelectedNotesArray].Type=bbl->getDbType();
        Preferences->SelectedNotes[Preferences->numberOfNotesInSelectedNotesArray].Index = noteNumber;
        bbl->BibleIndexNumber = selectedIndex; // the index number in the selectedBooks[] array
        //Preferences->incrementNumberOfSelectedNotes();
        //saveSelectedNotes();
        return YES;

    }else {
        Preferences->SelectedNotes[Preferences->numberOfNotesInSelectedNotesArray].Index=-1;
       // Preferences->setNumberOfSelectedNotes(Preferences->getNumberOfSelectedNotes()-1);
    }


   // saveSelectedNotes();

    return NO;

}




//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void addNoteToSelectedArrayByName(QString bible)
{
    int index;


    index = getIndexOfNoteInNotes(bible);
    if (index>= 0) {
        AddNoteToSelectedArrayByIndexNumber(index,Preferences->getNumberOfSelectedNotes());
    }

}
