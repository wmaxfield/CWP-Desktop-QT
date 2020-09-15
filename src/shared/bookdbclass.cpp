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
//  BookDB.m
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 6/22/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//converted

#include <QString>
#include <QStringList>
#include "misclibrary.h"
#include "HTMLModule.h"
#include "WebDefinitionModule.h"
#include "bookmanipulationlibrary.h"
#include "../shared/bibleinfo.h"
#include "bookdbclass.h"

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BookDBClass::BookDBClass()
{
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int BookDBClass::GetFootnoteIndex() {
    //return  FootNote++;
    FootNote++;
    return FootNote;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BookDBClass::Open(QString NameOfBook)
{
    int rslt;

    if (Instr(NameOfBook, BOOK_DATABASE_SUFFIX )) {
        QStringList n = mySplit(NameOfBook,".");
        NameOfBook = n [0];
        // super.dbType = TYPE_COMMENTARY_TYPE ;

        _BibleName = NameOfBook;
        rslt= BibleDatabaseClass::Open();// pick up other code benefits
        if (rslt==YES && getDbType()==TYPE_BBL_TYPE) {
            setDbType( TYPE_BOOK_TYPE);
        }
    }

    return NO;

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BookDBClass::initAndOpenWithDBName(QString  dbName)
{

    QStringList n = mySplit(dbName,".");

        setTypeBasedOnSuffixType(BOOK_DATABASE_SUFFIX_TYPE);
        databaseName= dbName;

        if (n.count()) {
            _BibleName = n [0];
        }
        int Type = getDbType();

        openWithType(Type);


}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int  BookDBClass::GetNumChapters() {
    QString  Query;// As String
    RecordSet *rs; //dim rs As RecordSet
    int r;//dim r As Integer


    Query = "Select MAX(" CHAPTER_COLUMN ") as MaxChapter from " BOOK_TEXT_TABLE;


    rs = SQLSelect(Query);

    if (rs != Nil ){ //Then
                     //----------------------
        r  = rs->Field("MaxChapter").toInt();
        return r;
    } //end if

    return 0;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int  BookDBClass::GetNumParagraphs(int Chapter)
{
    QString Query;//Dim Query As String
    RecordSet *rs;//dim rs As RecordSet
    int r;//  dim r As Integer


    Query ="Select MAX(" VERSE_COLUMN ") as MaxVerse from " BOOK_TEXT_TABLE " where chapter = '"+QString::number(Chapter)+"'";


    rs = SQLSelect(Query);

    if (rs != Nil ) {//then
                     //----------------------
        r  = rs->Field("MaxVerse").toInt();//.IntegerValue
        return r;
    }//end if

    return 0;

}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
QString BookDBClass::HandleWord(QString TheWord ,/*withSearchWord*/QString s, /*andKeyHilite:*/BOOL Hilite, /*withSearchInfo:*/SEARCH_INFO_TYPE * SearchInformation)
{

    if (Hilite) {// then
        if (SearchInformation->SearchForAWholeWord) {// then
            if ( StringsAreEqual(s ,TheWord)){// then
                TheWord= HTML_BOLD_START +TheWord+ HTML_BOLD_END;
            }//end if
        } else {
            if (Instr(TheWord,s) > 0){// then
                TheWord= HTML_BOLD_START +TheWord+ HTML_BOLD_END;
            }// end if
        }//                 end if
    }//                         end if

    return TheWord;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
QString BookDBClass::GetParagraph(SHOW_THIS_INFO_TYPE * STI, /* SearchParmsStructPointer:*/ SEARCH_INFO_TYPE *sit)
{
    //----------------------------------------------------
    // get an entire verse from the database and return it as a string
    //----------------------------------------------------
    //static rs As RecordSet
    RecordSet *rs;
    QString TheWord;
    QString VerseTxt="";//    static VerseText,TheWord As string
    QString Query;// static Query As String
    QStringList Words ;//= [NSMutableArray arrayWithCapacity:200);// dim Words(-1) As string
    QStringList Sentence;// = [NSMutableArray arrayWithCapacity:200] ;// dim Sentence(-1) As String
    int index;//dim Index As Integer
              //BOOL result;

#if ENABLE_MENUS
    QString CustomMenu = CUSTOM_MENU_HREF_INTERNAL;
#else
    //dim CustomMenu As String
    QString CustomMenu="";

#endif

    QString DictionaryStart;//dim DictionaryStart As String

    QString ts;
    QString w1;
    QString s2;
    QString s3;

    // this must be a "modern" database
    Query = QueryBegin+ BOOK_TEXT_TABLE " where " CHAPTER_COLUMN "="+QString::number(sit->ChapterNumber)+" and " VERSE_COLUMN "="+sit->VerseNumber+" "+QueryEnd;

    rs = SQLSelect(Query);

    if (rs != Nil ){ //Then

        if (STI->enableEnglishDictionary  ){ //Then
            DictionaryStart = "<a style=" QUOTE "color:black" QUOTE " href=" QUOTE HTML_DICT_INDICATOR "+";

            //-------------------------------------------------------------------------------------
            while (! rs->atEOF()) {
                TheWord = CheckSup(rs,sit->BookNumber,sit->ChapterNumber,sit->VerseNumber);


                if (StringsAreEqual(TheWord, HTML_BRStr)) {
                    Words.append( TheWord);
                } else {
                    // we'll have to format the records in the commentaries to make this work
                    if (Instr(TheWord,"<a") > 0) { //Then
                        Words.append( TheWord);
                        goto NextRecord;
                    } //end if

                    if (Instr(TheWord,HTML_HREF_END)>0) { //Then
                        Words.append( TheWord);
                        goto NextRecord;
                    } //end if

                    if (Instr(TheWord,"/>")>0){ //Then
                        Words.append( TheWord);
                        goto NextRecord;
                    } //end if




                    Sentence = mySplit(TheWord," ");
                    QString s;

                    if (!STI->HilightSearch) {
                        sit->WordToSearchFor[0]=0;// remove possible garbage
                    }
                    s=  sit->WordToSearchFor;

                    for (index = 0 ; index < Sentence.count();index++) {
                        w1 =HandleWord(Sentence [index],/* withSearchWord:*/s,/* andKeyHilite:*/STI->HilightSearch, /*withSearchInfo*/sit);
                        if (Instr(w1,"<")) {
                            ts = w1;
                        } else {

                            if (Instr(w1, ">")) {
                                QStringList a;

                                a = mySplit(w1, ">");
                                ts = a [0];
                                a.removeAt(0);
                                Words.append(ts);
                                w1 = a.join( "");
                            }

                            if (w1.length()) {
                                ts = HTML_HREF_END +DictionaryStart+Sentence [index]+ CustomMenu+HTML_GENERIC_END_TAGQ+w1;
                            } else {
                                ts = "";
                            }
                        }
                        Words.append( ts);
                    }//next
                }// if

            NextRecord:
                rs->MoveNext() ;//
            }// wend

            //-------------------------------------------------------------------------------------
        } else {// not enableEnglishDictionary
                //-------------------------------------------------------------------------------------
            if (STI->HilightSearch){ //Then
                while (! rs->atEOF() ) {
                    TheWord = CheckSup(rs,sit->BookNumber,sit->ChapterNumber,sit->VerseNumber);

                    if (Instr(TheWord,"<div") > 0 || Instr(TheWord,"DIV>")>0) { //Then
                        Words.append( TheWord);
                    }  else {

                        if (sit->PhraseSearch) { //Then
                            ts = sit->PhraseToSearchFor;
                            if (Instr(TheWord,ts)> 0 ) { //Then
                                w1 =  HTML_BOLD_START+QString(sit->PhraseToSearchFor)+ HTML_BOLD_END;
                                w1 = myReplace(TheWord,ts, w1);

                                Words.append(  w1);//)
                            } else {
                                Words.append( TheWord);
                            } //end if
                        } else {
                            ts = sit->WordToSearchFor;
                            if (Instr(TheWord,ts) > 0){ //Then
                                w1 = "C"+ts+"+";
                                int posn = Instr(TheWord,w1);

                                if (posn == 0){ //Then
                                    w1 =HTML_BOLD_START +ts+HTML_BOLD_END;
                                    Words.append( myReplace(TheWord,ts,w1));
                                } else {
                                    posn = posn +ts.length(); // pick up the length of the word being replaced

                                    if (Instrp(TheWord,ts,posn) > 0){ //Then
                                        posn = TheWord.length()-posn;
                                        s2= Right(TheWord, posn);
                                        s3 = HTML_BOLD_START +ts+ HTML_BOLD_END;

                                        w1 =  Left(TheWord,posn) + myReplace(s2,ts,s3);
                                        Words.append( w1);

                                    } else{
                                        Words.append( TheWord);
                                    } //end if
                                } //end if

                            }  else {
                                Words.append( TheWord);
                            } //end if
                        } //end if

                    } //end if

                    rs->MoveNext();
                }//  wend
            }  else {
                // DON"T HILIGHT SEARCH
                while (! rs->atEOF() ) {
                    TheWord = CheckSup(rs,sit->BookNumber,sit->ChapterNumber,sit->VerseNumber);
                    Words.append( TheWord); // (rs.IdxField(1).StringValue+rs.IdxField(WORD_FIELD).StringValue + rs.IdxField(3).StringValue)

                }//    wend
            } //end if

        } //end if // show dictionarywindow


        VerseTxt =Words.join(" ");

        return VerseTxt;

    } //end if


    //result = YES;
    return VerseTxt; // empty string
}


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
QStringList BookDBClass::searchForString(QString stringSearchWord, /*forSearchInstance:*/ int numberOfDefinitions)
{

    RecordSet *rs;
    // break the query up into words to look for
    QString Query;
    //ChapterVerseStructType cvs;
    int chapter,lastChapter=-1;
    int verse,lastVerse=-1;
    QStringList arrayToReturn;// = [[NSMutableArray arrayWithCapacity:numberOfDefinitions] retain);

    //cvs.BookNumber = -1;// trigger for a failed search
   // cvs.Verse = -1;
   // cvs.Posn = -1;
   // cvs.Chapter = -1;


    //  index = 0;
    stringSearchWord = dbEscapeSingleQuoteString(stringSearchWord);// prevent sql poisoning


     Query =  "Select " CHAPTER_COLUMN ", " VERSE_COLUMN " from " BOOK_TEXT_TABLE" where word like '%%"+stringSearchWord+"%%' ";

    rs =  SQLSelect(Query);


    if (rs  != Nil ){ //then
        //[rs retain);// make sure it does not go away in the search loop

        // skip to the definition requested by the user

        while (!rs->atEOF()) {
            if (--numberOfDefinitions >=0) {
                // don't find in table of contents
                Query = rs->IdxField(1);
                    chapter = Query.toInt();
                Query = rs->IdxField(2);
                    verse = Query.toInt();

                if (chapter ==0 || (chapter==lastChapter && verse == lastVerse)) {
                    ++numberOfDefinitions;
                    goto nextRecord;
                }
                lastVerse = verse;
                lastChapter = chapter;
                    Query = "0 "+QString::number(chapter)+":"+QString::number(verse);
                arrayToReturn.append(Query);
            }
        nextRecord:
            rs->MoveNext();
        }

        delete rs;// clean up after ourselves
    } //end if


//SearchFinished:

    return arrayToReturn;

}

