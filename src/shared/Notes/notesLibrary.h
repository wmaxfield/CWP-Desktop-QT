#ifndef NOTESLIBRARY_H
#define NOTESLIBRARY_H
//
//  notesLibrary.h
//  Crossword-Try3
//
//  Created by WADE MAXFIELD on 11/14/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//

//#include <Foundation/Foundation.h>
//#include "NotesDB.h"
#include <QString>
#include "cwpdefines.h"


QString  SuffixFromType(int SuffixType);
void     initializeNotes(void);


extern int             gNumberOfNotes;
BOOL    isNoteNameInSelectedArray(QString noteName);

void    saveSelectedNotes(void);
void    addNoteToSelectedArrayByName(QString noteName);
BOOL    AddNoteToSelectedArrayByIndexNumber(int noteNumber,int selectedIndex);

int             getIndexOfNoteInNotes(QString noteName);
void            getSelectedNotes(void);     // get the notes from the notes directory

BOOL    isNoteNameInSelectedArray(QString noteName);

int     OpenPersonalNotesByNameAndPutInSelectedArray(QString Name);

BOOL    removeNoteFromSelectedArrayByName(QString bible);



#endif // NOTESLIBRARY_H
