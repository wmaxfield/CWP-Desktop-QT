#ifndef HELPLIBRARY_H
#define HELPLIBRARY_H
#include <QString>
//
//  helpLibrary.h
//  CWP-Bible
//
//  Created by WADE MAXFIELD on 12/10/10.
//  Copyright 2010 Central Telecommunications, Inc. All rights reserved.
//


#define HELP_FILE_NAME "help.htm"
#define ABOUT_FILE_NAME "about.htm"

QString BuildHelpPage(void);
QString BuildAboutPage(void);

QString ReadHelpFileString();

#define EMPTY_PAGE_STRING "-----"

#endif // HELPLIBRARY_H
