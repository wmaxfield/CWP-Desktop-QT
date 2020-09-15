#ifndef BOOKHANDLERCLASS_H
#define BOOKHANDLERCLASS_H
//
//  BookHandlerClass.h
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/24/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
// converted

#include <QString>
#include "../shared/misclibrary.h"
#include "commentarydatabaseclass.h"

#include "structs.h"

#define BOOK_TYPE_BIBLE 1
#define BOOK_TYPE_COMMENTARY 2
#define BOOK_TYPE_NOTES 3
#define BOOK_TYPE_BOOK  4


//extern BibleDatabaseClass *b;
class BookHandlerClass
{
public:
    BookHandlerClass();
    ~BookHandlerClass();
    void *Database;
    QString name;
    BOOL selected;
};

#endif // BOOKHANDLERCLASS_H
