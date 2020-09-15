#ifndef STRONGSDATABASECLASS_H
#define STRONGSDATABASECLASS_H

//
//  StrongsDatabaseClass.h
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/28/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//
#include <QStringList>

#include "../shared/Database/database.h"
#include "structs.h"


class StrongsDatabaseClass : public SQliteDatabase
{
public:
    StrongsDatabaseClass();

    BOOL Open();
    STRONGS_STRUCTURE_TYPE  *GetStrongsDefinition(int StrongsNumber, /*Language:*/ QString Language);
    long Close();


    STRONGS_STRUCTURE_TYPE si;//dim SI as StrongsStructure
    BOOL                   isOpen();
    void                setIsOpen(BOOL s);

private:
    BOOL _isOpen;

};
extern StrongsDatabaseClass        *gStrongsDatabase;

#endif // STRONGSDATABASECLASS_H
