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
//  History.m
//  Crossword-Student
//
//  Created by WADE MAXFIELD on 11/30/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
// converted more or less

#include <QObject>
#include <QString>
#include "userhistoryclass.h"
#include "PreferencesLibrary.h"
#include "misclibrary.h"

#include <QStringList>

UserHistoryClass            *userHistory;

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void initHistory(){
    //--------------------------------------------------------------------
    // History must follow Preferences init
    //--------------------------------------------------------------------
    userHistory = new UserHistoryClass();
    if (userHistory) {
        userHistory->loadHistory();
    }
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
UserHistoryClass::UserHistoryClass()
{
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void UserHistoryClass::saveHistory() {
    int index;
    //int maxNum = historyArray.count();
    QString cvsString;

     for (index=0; index < MAX_HISTORY_ITEMS; index++) {
         if (index < historyArray.count()) {
           // cvsString = QString("%d:%d:%d:%d:%d").arg( cvs.BookNumber).arg( cvs.Chapter).arg(  cvs.Verse).arg( cvs.Posn).arg( cvs.orientation );
             SavePrefVariable("History"+QString::number(index),historyArray[index]);
         } else {
             SavePrefVariable("History"+QString::number(index), "");// save empty string to wipe out prev historyz
         }


     }
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void UserHistoryClass:: loadHistory(){
    int index;
    QString s;
    QStringList a;

    historyArray.clear();

    for (index=0 ; index < MAX_HISTORY_ITEMS; index++) {
        s = GetPrefVariable(QString("History")+QString::number(index));

        if (s.length() > 0) {
            historyArray.append(s);
            //[pcvs release];
        }

    }
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// don't add duplicates.  ie: duplicates will overwrite with last one
void UserHistoryClass::pushHistoryItemWithBook(int bookNumber, /*Chapter:*/int chapter, /*Verse:*/int verse, /*ScrollPosn:*/int sPosn)
{
    QString s1,s2;

    s1 = QString::number(bookNumber)+":"+QString::number(chapter)+":"+QString::number(verse)+":"+QString::number(sPosn);

    if (!historyArray.isEmpty()) {
        s2=  historyArray.last();// lastObject];

        if (s1 == s2){
            goto skipAppend;
        }
    }

    historyArray.append(s1);

skipAppend:
    if (historyArray.count()>= MAX_HISTORY_ITEMS) {
       historyArray.takeFirst();//removeObjectAtIndex:0]; // clear the top
    }

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// get the history index, use as LIFO if Reverse is true
ChapterVerseStructType UserHistoryClass::getHistoryItemByIndex(int index,/*Reverse:*/BOOL reverse)
{
    QStringList a;

    if (index<0) {
        index=0;
    }
    if (index>historyArray.count()-1) {
        index=historyArray.count()-1;
    }

    if (reverse) {
        index = historyArray.count()-index-1;  
    }

    a = mySplit(historyArray[index],":");

    cvs.BookNumber = a[0].toInt();
    cvs.Chapter = a[1].toInt();
    cvs.Verse = a[2].toInt();
    cvs.Posn = a[3].toInt();
    return cvs;

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int UserHistoryClass::getNumberOfHistoryItems() {
    return historyArray.count();
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void UserHistoryClass::setNumberOfHistoryItems(int i)
{
    while (historyArray.count()>i) {
         historyArray.takeLast();
    }

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void UserHistoryClass::deleteHistoryItemByIndex(int itemIndex) // first index is 0
{
    if (itemIndex<0) {
        itemIndex=0;
    }
    if (itemIndex>historyArray.count()-1) {
        itemIndex=historyArray.count()-1;
    }
    if (itemIndex >= 0) {
      historyArray.removeAt(itemIndex); //  historyArray.takeLast();// removeObjectAtIndex:itemIndex];
    }
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
ChapterVerseStructType UserHistoryClass::popHistoryItem() // last history added is removed
{


    QStringList a;
    QString s;

    cvs.BookNumber = -1;
    cvs.Chapter =1;
    cvs.Verse=1;

    if (historyArray.count()>0) {
       s= historyArray.takeLast();//lastObject];
    } else {
        return cvs;
    }

   a  = mySplit(s,":");

    cvs.BookNumber = a[0].toInt();
    cvs.Chapter = a[1].toInt();
    cvs.Verse = a[2].toInt();
    cvs.Posn = a[3].toInt();
    return cvs;
}

// ------------------------------------------------------------------------
// clear the posn variable part of the history array
// ------------------------------------------------------------------------
void UserHistoryClass::clearHistoryArrayPosn() {
    int index;
   QString s;
    QStringList a;

    for (index =0; index < historyArray.count(); index++) {
        s= historyArray[/*objectAtIndex:*/index];
        a = s.split(":");
        a[a.count()-1]="0";
        historyArray[index]=a.join(":");
    }
}
