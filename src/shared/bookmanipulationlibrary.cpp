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
//  BookManipulationLibrary.m
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 6/22/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//converted

#include <QStringList>

#include "bookmanipulationlibrary.h"
#include "misclibrary.h"
#include "WebDefinitionModule.h"
#include "../shared/bookmanipulationclass.h"
#include "../shared/bookdbclass.h"
#include "BibleDatabaseClass/bibledatabaseclass.h"
#include "WebDefinitionModule.h"
#include "HTMLModule.h"
#include "../shared/bookpreferencesdbclass.h"
#include "bookhandlerclass.h"
#include "bibleinfo.h"

extern QString CurrentWebPage;

BookManipulationClass *bmc;
BookPreferencesDBClass *gTextBookPreferences;

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BuildBookPageForBookChapterVerse(int book, int chapter, int verse, QString searchString )
{
    if (!bmc)
        bmc = new BookManipulationClass();


    QString t;

    t = bmc->buildTextPageForBook(book,chapter,globalSTI.DoScrollToVerse,verse,globalSTI.HilightSearch,searchString);

    CurrentWebPage = myReplace(t, "</tr><tr>", "</tr>\r\n<tr>");
    QString ts = "style='font-size: "+QString::number(Preferences->getFontSize())+"pt;' href";
    CurrentWebPage = myReplace(CurrentWebPage, "href", ts);
    ts = "<p style='font-size: "+QString::number(Preferences->getFontSize())+"pt;'>";
    CurrentWebPage = myReplace(CurrentWebPage, "<p>", ts);


}


#define WORD_FIELD 2

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString CheckSup(RecordSet *rs,int BookIndexNumber,int Chapter, int Verse)
{
    // if there is a "<SUP>" in the text, with a number between them, and a </SUP>) { //then that is a footnote.
    // replace the footnote SUP with the link for the footnote handler
    if (rs==Nil) {//then
        return "";
    }

    int StartLoc; //As Integer
    QString  inputString, str;// As String
    QString  TempWord;// As String
    QStringList Word;//=[NSMutableArray arrayWithCapacity:100];//(-1) As String
    int index=0;// As Integer =0


    BookHandlerClass *bk = gTextBooks[BookIndexNumber];
    BookDBClass *bkdb = reinterpret_cast<BookDBClass *>(bk->Database);

    if (!bkdb)
        return "";

    inputString = rs->IdxField(1)+ rs->IdxField(WORD_FIELD)+ rs->IdxField(3);

    StartLoc =Instr(inputString,SUPstart);

   // BOOL CAP=false;

    if (StartLoc>0){//Then

        Word = mySplit(inputString, SUPstart);// get array of strings wrapped around <sup> in case there are multiple ones


        if (Word.count() == 1){ //then
            Word = mySplit(inputString,  SUPstartCAP);
            if (Word.count()==1)
                return inputString;
            //CAP=true;
        }


        TempWord =SUPstart + QString::number(bkdb->GetFootnoteIndex())+SUPend;

        // wsm add the book index number after the 'F' so we can look it up in correct database
        // wsm pick up the chapter,verse,sequence number for the footnote
        TempWord =str.sprintf(HTML_HREF_STARTQ FOOTNOTE_INDICATOR "-%d-%d+%d+%d+%ls" HTML_GENERIC_END_TAGQ "%ls" HTML_HREF_ENDS
                              ,BookIndexNumber,BookIndexNumber,Chapter,Verse,(wchar_t*)rs->Field(  WORD_SEQ_COLUMN ).utf16(), (wchar_t*)TempWord.utf16());

        Word[index]= Word[index] +TempWord;

        QString ts = Word[1];
        StartLoc =Instr(ts, SUPend);
        //CAP=false;

        if (StartLoc==0) {
            StartLoc = Instr(ts,  SUPendCAP);
        }

        if (StartLoc > 0 ){//Then
            int pos =StartLoc+QString(SUPend).length()-1;
            TempWord = Right(ts,pos);
            Word [1] =TempWord;
        }//end if


        return Word.join(" ");

    }//end if

    return inputString;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString CreateBookMarkTag( int Book,int Chapter,int VerseNumber ,QString  VerseTxt,BOOL DoHideNote ,int DisplayBible ,int BooksToUseIndex, BOOL OneRow)
{
    Q_UNUSED(DisplayBible)
    Q_UNUSED(BooksToUseIndex)
    //----------------------------------
    // book  == number of book
    // chapter == number of chapter
    // verse == number of verse
    // DoHideNote == hide the note link
    // DisplayBible == bible number in the TextBooks array
    // TypeOfBook == the kind of book (saves time)
    //----------------------------------
    QString  s="";// As String
    QString  PreVerse="";// As String
    int PStart=0;// As Integer
    QString str;
    //BookHandlerClass *bk = [gTextBooks objectAtIndex:DisplayBible];
  //  BookDBClass *bkdb = bk.Database;

    if (Chapter == 0)  //then return "" // we don't mark the table of contents
        return s;

    if (OneRow || VerseTxt.trimmed().length() <  20)  //then return VerseTxt  // don't bookmark short verses or when we are in the "OneRow" mode
        return VerseTxt;

    if (Instr(VerseTxt, HTML_CENTER_START)> 0)  //then return VerseTxt // don't bookmark "centered" text
        return VerseTxt;

    if (Instr(VerseTxt, "<center") || Instr(VerseTxt, "<CENTER")) {// don't bookmark centered text
        return VerseTxt;
    }



    PStart = Instr(VerseTxt,HTML_PARAGRAPH_START);

    if (PStart>0) { //then
        PStart = PStart + QString(HTML_PARAGRAPH_START).length();
        PreVerse = Left(VerseTxt,PStart-1);
        VerseTxt = Mid(VerseTxt,PStart-1,VerseTxt.length()-PStart);
    } //end if


    if (DoHideNote) {
        s = str.sprintf("%ls" HTML_HREF_STARTQ BOOKMARK_INDICATOR_DIRECT "%d_%d" HTML_COLON_F "%d" QUOTE HTML_GENERIC_END_TAG SUPstart BOOKMARK_INDICATOR_DIRECT SUPend HTML_HREF_END,
                        (wchar_t*)s.utf16() ,Book, Chapter,VerseNumber);
    } else {
        // build the speech href tag
      /*  if (Preferences.speechEnabled) { //then
            s = [NSString stringWithFormat: SUPstart HTML_HREF_STARTQ SPEECH_INDICATOR_DIRECT "-%d-%ls-%d+%d"HTML_COLON_F "%d" QUOTE HTML_GENERIC_END_TAG SPEECH_INDICATOR_DIRECT  HTML_HREF_END SUPend " " , DisplayBible, bkdb.Title,Book , Chapter, VerseNumber];
        } //end if
       */
        s = str.sprintf("%ls" HTML_HREF_STARTQ BOOKMARK_INDICATOR_DIRECT "%d_%d" HTML_COLON_F "%d" QUOTE HTML_GENERIC_END_TAG SUPstart BOOKMARK_INDICATOR_DIRECT SUPend HTML_HREF_END,(wchar_t*) s.utf16() ,Book, Chapter,VerseNumber);
    }

    s =str.sprintf("%ls%ls%ls", (wchar_t*)PreVerse.utf16() ,(wchar_t*)s.utf16(), (wchar_t*)VerseTxt.utf16()); //+ HTML_BRStr  // make paragraphs more


    return s;

}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
void LoadDefaultBookPrefs(void) {
    gTextBookPreferences->savePrefVariableWithCategory( CATEGORY_PREFERENCE , INI_BOOK_PREFERRED_TABLE_STYLE, TABLE_STYLE_NO_LINES );
    gTextBookPreferences->setBookPreferredTableStyle ( TABLE_STYLE_NO_LINES);
}
//----------------------------------------------------------------------------------------------------------------------------------
// return utf8 string of bible name
//----------------------------------------------------------------------------------------------------------------------------------
QString TextBook(int BookNumber)
{
    //return "No Such Book";
    if (!bmc)
        bmc = new BookManipulationClass();

    // BibleBookInfoStructType *bbi;// this is not thread safe
    //bbi= bmc->GetTextBookInfoForBook(BookNumber,0);

    QString s =   bmc->getTextBookNameForNumber(BookNumber);
    return s;
}
