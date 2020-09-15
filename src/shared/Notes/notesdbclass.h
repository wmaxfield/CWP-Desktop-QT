#ifndef NOTESDBCLASS_H
#define NOTESDBCLASS_H
//
//  NotesDB.h
//  Crossword-Student
//
//  Created by WADE MAXFIELD on 11/14/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//
#include <QString>
#include "../shared/BibleDatabaseClass/bibledatabaseclass.h"
#include "BibleDatabaseClass/bibledatabaseclass.h"
#include "notesLibrary.h"

#define DEFAULT_NOTES_FILE "MyNotes"





class NotesDBClass : public BibleDatabaseClass
{
public:
    NotesDBClass();
    NotesDBClass(QString NameOfNote) ;

    int Open(QString NameOfNote);
    void initAndOpenWithDBName(QString dbName);

    void checkForCompletenessAndCorrectIfNecessary(); // and correct.
    BOOL openWithType(int lType);
};

#endif // NOTESDBCLASS_H
