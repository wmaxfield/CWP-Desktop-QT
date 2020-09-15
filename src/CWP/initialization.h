#ifndef INITIALIZATION_H
#define INITIALIZATION_H
//
//  Initialization.h
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 4/14/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//

//#include "CommentaryModule.h"
//#include "FirstViewController.h"
#include "../shared/misclibrary.h"
#include "../shared/structs.h"
#include "../shared/strongsdatabaseclass.h"
#include "PreferencesLibrary.h"
#include "searchlibrary.h"
//#include "../shared/RecentSearchesController.h"
//#include "SelectBookInBible.h"
//#include "NotesViewController.h"
#include "notesLibrary.h"
#include "notesdbclass.h"
//#include "HistoryViewController.h"
#include "../shared/commentarymodule.h"
//#include "../shared/helpLibrary.h"
#include "dictionarydb.h"
//#include "dictionaryViewController.h"
#include "notesLibrary.h"
//#include "FliteTTS.h"
#include "../shared/bookmanipulationclass.h"


extern NotesDBClass *gNotes[MAX_NUM_BIBLES];
//extern UserHistoryClass *userHistory;


class initialization
{
public:
    initialization();

    int failed;

    void initBibles();
    void initCommentaries();
    void initLibraryStuff();
    void initNotes();
    void initTextBooks();
    void initMedia();

    void initStrongsDatabase();
    void initEnglishDictionaryDatabase();
    void getSelectedNotes();
};

extern initialization *initObject;

extern int numberOfBooksShownChanged;
extern int changeWasDueToUserInteraction;
extern int currentScrollPosition;
extern int verseBeforeNumberOfBooksChanged;
extern QString debugFlag;



#endif // INITIALIZATION_H
