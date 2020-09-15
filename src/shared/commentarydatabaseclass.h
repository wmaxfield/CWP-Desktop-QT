#ifndef COMMENTARYDATABASECLASS_H
#define COMMENTARYDATABASECLASS_H
//
//  CommentaryDatabaseClass.h
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/25/10.
//  Copyright 2010 CrossWordProject. All rights reserved.
//


#include <QString>
//#include <Foundation/Foundation.h>
#include "../shared/Database/database.h"
#include "BibleDatabaseClass/bibledatabaseclass.h"






class CommentaryDatabaseClass : public BibleDatabaseClass
{
public:
    CommentaryDatabaseClass();

    int Open(QString NameOfCommentary);
    void initAndOpenWithDBName(QString dbName);



};

#endif // COMMENTARYDATABASECLASS_H
