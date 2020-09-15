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
*/#include "helplibrary.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
//
//  helpLibrary.m
//  CWP-Bible
//
//  Created by WADE MAXFIELD on 12/10/10.
//  Copyright 2010 Wade Maxfield All rights reserved.
//

#include "helplibrary.h"
#include "../shared/misclibrary.h"
#include "HTMLModule.h"
#include "bibleinfo.h"

extern QString CurrentWebPage;
extern QString AboutWebPage;


QString BuildHelpPage(void)
{
    QString Directory= getFullPathOfAppBundleCWPDirectory()+"/cwp-help" ;

    QDir dir(Directory);
    if (!dir.exists()) {
       return EMPTY_PAGE_STRING;
    }


    QString filePath = Directory +"/"+HELP_FILE_NAME ;

        QString myText ;

        QFile f(filePath);

        if (!f.open(QFile::ReadOnly | QFile::Text))
            return EMPTY_PAGE_STRING;

        QTextStream in(&f);

        myText.append(in.readAll()); //[NSString stringWithContentsOfFile:filePath encoding:NSISOLatin1StringEncoding error:nil];
        if (myText.length()) {
          //  vp = [CurrentWebPage stringByReplacingOccurrencesOfString:@"&@&" withString:imagePath];

            myText = myReplace(myText, "help_files/",  "&@&//cwp-help//help_files//");
//            myText = myReplace(myText, "help_files/",  ".//cwp-help//help_files//");

            return myText;
        }

        return EMPTY_PAGE_STRING;

}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
QString ReadHelpFileString() {
    QString vp=BuildHelpPage();

  //vp =   HTML_STANDARD_HEADER HTML_NO_UNDERLINE_STYLE javascriptScrollOffset +vp+ HTML_STANDARD_END;

//    webView->setHtml(vp, QUrl::fromLocalFile("/"));
   return vp;
}


QString BuildAboutPage(void)
{
    QString Directory = getResourceDirectory()+"/"+"Help";
    QString version = "";//[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];

    QDir dir(Directory);
    if (!dir.exists()) {
        return "";
    }




    QString filePath = Directory +"/"+ ABOUT_FILE_NAME ;

    QString myText ;

    QFile f(filePath);

    if (!f.open(QFile::ReadOnly | QFile::Text))
        return "";

    QTextStream in(&f);

    myText.append(in.readAll()); //[NSString stringWithContentsOfFile:filePath encoding:NSISOLatin1StringEncoding error:nil];
        if (myText.length()) {

            version = "Version "+version+"<br/></b><center>"+__DATE__+" "+__TIME__+"</center>";
            myText = myReplace(myText, "about_files/",  "&@&//Help//about_files//");
            myText = myReplace(myText, "Version #.#", version);


            //AboutWebPage = myText ;
        }


   return myText;

}
