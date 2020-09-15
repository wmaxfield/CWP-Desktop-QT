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
//  BibleInfo.m
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/24/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//converted

#include "mainwindow.h"
#include "bibleinfo.h"
#include "HTMLModule.h"
#include "notesLibrary.h"
#include "notesdbclass.h"

//#include "FirstViewController.h"

QString HandleVerseTag(QString bmt,QString versetext, int Type,SHOW_THIS_INFO_TYPE *ShowThisInfo);
QString BibleBook(int BookNumber);
QString CreateBookmarkTag(SEARCH_INFO_TYPE *bcv,
                                           SHOW_THIS_INFO_TYPE *ShowThisInfo,
                                           int BookToUse,
                                           BookInfoStructType *BooksToUse);


extern NotesDBClass *gNotes[MAX_NUM_BIBLES];// notesLibrary.h
extern QString CurrentWebPage;           // global for the web page

static QString BookNamesArray[] =
{ (QString)"",(QString)b01,(QString)b02,(QString)b03,(QString)b04,(QString)b05,(QString)b06,(QString)b07,(QString)b08,(QString)b09,
    b10,(QString)b11,(QString)b12,(QString)b13,(QString)b14,(QString)b15,(QString)book16,(QString)b17,(QString)b18,(QString)b19,
    b20,(QString)b21,(QString)b22,(QString)b23,(QString)b24,(QString)b25,(QString)b26,(QString)b27,(QString)b28,(QString)b29,
    b30,(QString)b31,(QString)b32,(QString)b33,(QString)b34,(QString)b35,(QString)b36,(QString)b37,(QString)b38,(QString)b39,
    b40,(QString)b41,(QString)b42,(QString)b43,(QString)b44,(QString)b45,(QString)b46,(QString)b47,(QString)b48,(QString)b49,
    b50,(QString)b51,(QString)b52,(QString)b53,(QString)b54,(QString)b55,(QString)b56,(QString)b57,(QString)b58,(QString)b59,
    b60,(QString)b61,(QString)b62,(QString)b63,(QString)b64,(QString)b65,(QString)b66
};
QStringList BookNamesList ;

QStringList getBookNamesList(){
    if (BookNamesList.length()<1){
        for (int i=0; i < 66; i++){
            BookNamesList.append(BookNamesArray[i]);
        }
    }
    return BookNamesList;
}

//----------------------------------------------------------------------------------------------------------------------------------
// return  string of bible name
//----------------------------------------------------------------------------------------------------------------------------------
QString BibleBook(int BookNumber)
{
    if (BookNumber <1 || BookNumber>66) {
        BookNumber=1;
    }
    //return NSString stringWithUTF8String:BookNamesArrayBookNumber]);
    return BookNamesArray[BookNumber];
}


QStringList TagArray;
static unsigned int bookMarkIDNumber; // the number for the bookmark ID tag
//----------------------------------------------------------------------------------------------------------------------------------
// book  == number of book
// chapter == number of chapter
// verse == number of verse
// DoHideNote == hide the note link
// DisplayBible == bible number in the Books array
// TypeOfBook == the kind of book (saves time)
//----------------------------------------------------------------------------------------------------------------------------------
QString CreateBookmarkTag(SEARCH_INFO_TYPE *bcv,
                            SHOW_THIS_INFO_TYPE *ShowThisInfo,
                            int BookToUse,
                            BookInfoStructType *BooksToUse)
{
    QString s,s1;
    int myType = BooksToUse[BookToUse].Type;
    //----------------------------------------------------------------------------------------------------------------------------------
    if (ShowThisInfo->DoHideNotes){// Then
        if (1/*!index*/){// Then
            s = s1.sprintf( HTML_HREF_STARTQ COPYRIGHT_INDICATOR_SHOW_WINDOW "%d_%d",bcv->BookNumber,bcv->ChapterNumber );
            s = s +HTML_COLON_F;
            s1.sprintf("%d\"" HTML_GENERIC_END_TAG SUPstart HTML_NBSP"%d" HTML_NBSP SUPend HTML_HREF_END , bcv->VerseNumber,bcv->VerseNumber);
            s = s +s1;
            return s;
        }
        else {
            s = s1.sprintf( SUPstart"%d" SUPend, bcv->VerseNumber );
            return s;
        }
    }
    //----------------------------------------------------------------------------------------------------------------------------------

    if (!BookToUse){// Then
        if (ShowThisInfo->showBookMarkTag){
            s = "<a id='b"+QString::number(bookMarkIDNumber)+"' href=\"http://" BOOKMARK_INDICATOR_DIRECT +QString::number(bcv->BookNumber);
            s=s+"_"+ QString::number(bcv->ChapterNumber)+HTML_COLON_F+QString::number(bcv->VerseNumber)+"_b"+QString::number(bookMarkIDNumber);
            s=s+"\"" HTML_GENERIC_END_TAG SUPstart BOOKMARK_INDICATOR_DIRECT;
        }
        bookMarkIDNumber++;

        // if this is _also_ the personal notes, then see if we should add the "N" to it
        if (TYPE_NOTES_TYPE==myType){// Then
            s = s + HTML_HREF_END HTML_NBSP ;
            goto buildNoteHref;  // go build the end of the personal note
        }

        s = s1.sprintf("%ls" SUPend HTML_HREF_END SUPstart HTML_NBSP "%d" SUPend,(wchar_t*)s.utf16(), bcv->VerseNumber);
    }
    else {
        s="";// empty string

        // if this is the personal commentary, then add an "ADD/UPDATE NOTE" line
        if ( TYPE_NOTES_TYPE==myType) {
            s = s + HTML_NBSP HTML_NBSP  ;
            // build the note href
        buildNoteHref:
            s = s + "<a id='n"+QString::number( bookMarkIDNumber)+"' href=\"http://" NOTES_INDICATOR_DIRECT +QString::number(bcv->BookNumber)+\
                    "_"+QString::number(bcv->ChapterNumber)\
                    +HTML_COLON_F+QString::number(bcv->VerseNumber)+\
                    "_n"+QString::number(bookMarkIDNumber) +"_"+\
                    QString::number(bcv->indexIntoArray)+ QUOTE HTML_GENERIC_END_TAG NOTES_INDICATOR_DIRECT HTML_HREF_END ;

            bookMarkIDNumber++;
        }
        s = s +QString(SUPstart HTML_NBSP)+QString::number(bcv->VerseNumber)+QString(SUPend);
    }



    // build the speech href tag
    if (ShowThisInfo->EnableMultimediaTag){
        int DisplayBible = BooksToUse[BookToUse].Index;

        BookHandlerClass *bd = Books[DisplayBible];
        //BibleDatabaseClass *bbld= bd.BibleDatabase;
        QString n;
        if (TYPE_NOTES_TYPE==myType) {
            n = gNotes[BooksToUse[BookToUse].Index]->_BibleName;
        }else {
            n = bd->name;
        }

        // Then                       //  s                                       -    #                           -   name
        s = s1.sprintf( SUPstart HTML_HREF_STARTQ SPEECH_INDICATOR_DIRECT "-%d-%ls-%d_%d_%d\"" HTML_GENERIC_END_TAG SPEECH_INDICATOR_DIRECT " " HTML_HREF_END SUPend "%ls "
                        , DisplayBible,(wchar_t*)n.utf16(), bcv->BookNumber, bcv->ChapterNumber , bcv->VerseNumber,(wchar_t*)s.utf16());
    }

    if (ShowThisInfo->EnableVerseEditing) {
        s = s1.sprintf( "<a id='e%u' href=\"http://" EDIT_INDICATOR_DIRECT "%d_%d_%d" HTML_COLON_F "%d_b%u\"" HTML_GENERIC_END_TAG  " " EDIT_INDICATOR_DIRECT " " HTML_HREF_END "%ls"
                        , bookMarkIDNumber,ShowThisInfo->IndexIntoSelectedBooksArray, bcv->BookNumber, bcv->ChapterNumber ,bcv->VerseNumber, bookMarkIDNumber,(wchar_t*)s.utf16());
    }

    if (ShowThisInfo->showCopyTag){
        int DisplayBible = BooksToUse[BookToUse].Index;

        BookHandlerClass *bd = Books[DisplayBible];
        //BibleDatabaseClass *bbld= bd.BibleDatabase;
        QString n;
        if (TYPE_NOTES_TYPE==myType) {
            n = gNotes[BooksToUse[BookToUse].Index]->_BibleName;
        }else {
            n = bd->name;
        }

        // "CCC" is the indicator for "copy" verse
        s = s1.sprintf( SUPstart HTML_HREF_STARTQ COPY_INDICATOR_DIRECT_SWITCH_VAR_STR COPY_INDICATOR_DIRECT_SWITCH_VAR_STR COPY_INDICATOR_DIRECT_SWITCH_VAR_STR"-%d-%ls_%d_%d_%d\"" HTML_GENERIC_END_TAG COPY_INDICATOR_DIRECT " " HTML_HREF_END SUPend "%ls "
                        , DisplayBible,(wchar_t*)n.utf16(), bcv->BookNumber, bcv->ChapterNumber , bcv->VerseNumber,(wchar_t*)s.utf16());
    }
    return s;


}

//----------------------------------------------------------------------------------------------------------------------------------
// Create the verse decorations
//----------------------------------------------------------------------------------------------------------------------------------
QString HandleVerseTag(QString bmt,QString versetext, int Type,SHOW_THIS_INFO_TYPE *ShowThisInfo)
{
    QString  s2,s1;

#define CELL_RIGHT_PAD 30
    // doing a vectored if() that did the split later saved about 10 milliseconds for 66 verses * 4
    // verseus doing a split() and then seeing if UBound > 0
    if ( Left(versetext,4) == HTML_SMALL_HEADER_START){// then
        QStringList s;

        s = versetext.split( HTML_SMALL_HEADER_END);
        //s2 = s[0);

        s2 = s[0].replace(HTML_SMALL_HEADER_START,s1.sprintf( "<font size=\"%d\">" HTML_BOLD_START, ShowThisInfo->HeaderSize) );
        s2 = s2 +"</font>" HTML_BOLD_END HTML_BRStr ;


        if (ShowThisInfo->doStackVerses) {
            switch (ShowThisInfo->IndexIntoSelectedBooksArray) {
                case 0:
                    s2 = s2+bmt+s[1];
                    break;

                default:
                    if (ShowThisInfo->IndexIntoSelectedBooksArray==ShowThisInfo->NumberOfBooksBeingShownOnPage) {
                        // this is the cell padding for the separation between books on a page, the first number after the comma is the padding
                        s2 =  s2+bmt+s[1];
                        break;
                    }

                    /*else*/

                    s2 = s2+bmt+s[1];

                    break;

            }
        }else {
            switch (ShowThisInfo->IndexIntoSelectedBooksArray) {
                case 0:
                    s2 = s1.sprintf( HTML_TABLE_CELL_START_RIGHT_PAD "%ls%ls%ls" HTML_TABLE_CELL_END, CELL_RIGHT_PAD/2,(wchar_t*)s2.utf16(),(wchar_t*)bmt.utf16(),(wchar_t*)s[1].utf16());
                    break;

                default:
                    if (ShowThisInfo->IndexIntoSelectedBooksArray==ShowThisInfo->NumberOfBooksBeingShownOnPage) {
                        // this is the cell padding for the separation between books on a page, the first number after the comma is the padding
                        s2 = s1.sprintf( HTML_TABLE_CELL_START_LEFT_PAD "%ls%ls%ls" HTML_TABLE_CELL_END,CELL_RIGHT_PAD/2, (wchar_t*)s2.utf16(),(wchar_t*)bmt.utf16(),(wchar_t*)s[1].utf16());
                        break;
                    }

                    /*else*/

                    s2 = s1.sprintf( HTML_TABLE_CELL_START_RIGHT_LEFT_PAD "%ls%ls%ls" HTML_TABLE_CELL_END,CELL_RIGHT_PAD/2, CELL_RIGHT_PAD/2,(wchar_t*)s2.utf16(),(wchar_t*)bmt.utf16(),(wchar_t*)s[1].utf16());

                    break;

            }
        }



        if (Instr(s2, "&@&")) {
         //   s2 = CurrentWebPage.replace("&@&",fvc->imagePath);
        }

        // s2 = s1.sprintf( HTML_TABLE_CELL_START "%ls%ls%ls" HTML_TABLE_CELL_END,s2,(char*)bmt,s[1]);
        return s2;
    }


    if  ( Type == TYPE_MORPHOLOGY_TYPE) {

        bmt = RemoveAll(bmt, SUPstart HTML_HREF_STARTQ "S", SUPend);

    }


    if (Type == TYPE_ABP_TYPE ){// then

        s2 = RemoveAll(bmt,SUPstart HTML_HREF_STARTQ "S", SUPend);

        if (ShowThisInfo->doStackVerses) {
            switch (ShowThisInfo->IndexIntoSelectedBooksArray) {
                case 0:
                    // this is the cell padding for the separation between books on a page, the first number after the comma is the padding
                    s2= s1.sprintf(  SPAN_CLASS_UNIT SPAN_CLASS_STRONGS HTML_NBSP UN_SPAN SPAN_CLASS_GREEK "%ls" UN_SPAN SPAN_CLASS_ENGLISH HTML_NBSP UN_SPAN   UN_SPAN "%ls"
                                     ,(wchar_t*)s2.utf16(),(wchar_t*)versetext.utf16());
                    break;

                default:
                    if (ShowThisInfo->IndexIntoSelectedBooksArray==ShowThisInfo->NumberOfBooksBeingShownOnPage) {
                        s2= s1.sprintf(  SPAN_CLASS_UNIT SPAN_CLASS_STRONGS HTML_NBSP UN_SPAN SPAN_CLASS_GREEK "%ls" UN_SPAN SPAN_CLASS_ENGLISH HTML_NBSP UN_SPAN   UN_SPAN "%ls"
                                         ,(wchar_t*)s2.utf16(),(wchar_t*)versetext.utf16());

                        break;
                    }

                    /*else*/
                    s2= s1.sprintf(  SPAN_CLASS_UNIT SPAN_CLASS_STRONGS HTML_NBSP UN_SPAN SPAN_CLASS_GREEK "%ls" UN_SPAN SPAN_CLASS_ENGLISH HTML_NBSP UN_SPAN   UN_SPAN "%ls"
                                     ,(wchar_t*)s2.utf16(),(wchar_t*)versetext.utf16());


                    break;

            }

        }else {
            switch (ShowThisInfo->IndexIntoSelectedBooksArray) {
                case 0:
                    // this is the cell padding for the separation between books on a page, the first number after the comma is the padding
                    s2= s1.sprintf( HTML_TABLE_CELL_START_RIGHT_PAD SPAN_CLASS_UNIT SPAN_CLASS_STRONGS HTML_NBSP UN_SPAN SPAN_CLASS_GREEK "%ls" UN_SPAN SPAN_CLASS_ENGLISH HTML_NBSP UN_SPAN   UN_SPAN "%ls" HTML_TABLE_CELL_END,CELL_RIGHT_PAD
                                    ,(wchar_t*)s2.utf16(),(wchar_t*)versetext.utf16());
                    break;

                default:
                    if (ShowThisInfo->IndexIntoSelectedBooksArray==ShowThisInfo->NumberOfBooksBeingShownOnPage) {
                        s2= s1.sprintf( HTML_TABLE_CELL_START_LEFT_PAD_ABP SPAN_CLASS_UNIT SPAN_CLASS_STRONGS HTML_NBSP UN_SPAN SPAN_CLASS_GREEK "%ls" UN_SPAN SPAN_CLASS_ENGLISH HTML_NBSP UN_SPAN   UN_SPAN "%ls" HTML_TABLE_CELL_END,CELL_RIGHT_PAD
                                        ,(wchar_t*)s2.utf16(),(wchar_t*)versetext.utf16());

                        break;
                    }

                    /*else*/
                    s2= s1.sprintf( HTML_TABLE_CELL_START_RIGHT_LEFT_PAD SPAN_CLASS_UNIT SPAN_CLASS_STRONGS HTML_NBSP UN_SPAN SPAN_CLASS_GREEK "%ls" UN_SPAN SPAN_CLASS_ENGLISH HTML_NBSP UN_SPAN   UN_SPAN "%ls" HTML_TABLE_CELL_END,CELL_RIGHT_PAD/2, CELL_RIGHT_PAD/2,
                                     (wchar_t*)s2.utf16(),(wchar_t*)versetext.utf16());


                    break;

            }

        }



        if (Instr(s2, "&@&")) {
          //  s2 = CurrentWebPage.replace("&@&",fvc.imagePath);
        }

        return s2;





    //-----------------------------------------------------------------
    }  else {


        if (ShowThisInfo->doStackVerses) {
            switch (ShowThisInfo->IndexIntoSelectedBooksArray) {
                case 0:
                    s2 =  bmt+ versetext;

                    break;

                default:
                    if (ShowThisInfo->IndexIntoSelectedBooksArray==ShowThisInfo->NumberOfBooksBeingShownOnPage) {
                        // this is the cell padding for the separation between books on a page, the first number after the comma is the padding
                        s2 =bmt+ versetext;
                        break;
                    }

                    /*else*/
                    s2 = bmt+ versetext;
                    break;

            }

        }else {
            switch (ShowThisInfo->IndexIntoSelectedBooksArray) {
                case 0:
                    s2 = s1.sprintf( HTML_TABLE_CELL_START_RIGHT_PAD "%ls%ls" "</td>", CELL_RIGHT_PAD/2,(wchar_t*)bmt.utf16(), (wchar_t*)versetext.utf16());

                    break;

                default:
                    if (ShowThisInfo->IndexIntoSelectedBooksArray==ShowThisInfo->NumberOfBooksBeingShownOnPage) {
                        // this is the cell padding for the separation between books on a page, the first number after the comma is the padding
                        s2 = s1.sprintf( HTML_TABLE_CELL_START_LEFT_PAD "%ls%ls" "</td>", CELL_RIGHT_PAD/2,(wchar_t*)bmt.utf16(),(wchar_t*) versetext.utf16());
                        break;
                    }

                    /*else*/
                    s2 = s1.sprintf( HTML_TABLE_CELL_START_RIGHT_LEFT_PAD "%ls%ls" "</td>",CELL_RIGHT_PAD/2, CELL_RIGHT_PAD/2,(wchar_t*)bmt.utf16(), (wchar_t*)versetext.utf16());
                    break;

            }

        }


        if (Instr(s2, "&@&")) {
            //s2 = CurrentWebPage.replace("&@&",fvc.imagePath);
        }

        return s2;
    }


}

//---------------------------------------------------------------------
// return the book in alphabetical order, where 1==Acts, 2==amos, etc
// use a pre-sorted array for speed.
//---------------------------------------------------------------------
BibleBookInfoStructType getAlphabeticalBibleBookInfo(int BookNumber)
{
    char AlphaArray[]= {
        0, /* not a book*/ 44 /*acts*/,       30 /*amos*/,        13 /*1chr*/,
        14 /*2chr*/,       51 /*col*/,        46 /*1cor*/,        47 /*2cor*/,
        27 /*dan*/,        5  /*deu*/,        21 /*ecc*/,         49 /*eph*/,
        17 /*est*/,        2  /*exo*/,        26 /*eze*/,         15 /*ezra*/,
        48 /*gal*/,        1  /*gen*/,        35 /*hab*/,         37 /*hag*/,
        58 /*heb*/,        28 /*hos*/,        23 /*isa*/,         59 /*jam*/,
        24 /*jer*/,        18 /*job*/,        29 /*joel*/,        43 /*john*/,
        62 /*1john*/,      63 /*2john*/,      64 /*3john*/,       32 /*jonah*/,
        6  /*jos*/,        65 /*jude*/,       7  /*judges*/,      11 /*1ki*/,
        12 /*2ki*/,        25 /*lam*/,        3  /*lev*/,         42 /*luk*/,
        39 /*mal*/,        41 /*mark*/,       40 /*mat*/,         33 /*micah*/,
        34 /*nahum*/,      16 /*neh*/,        4  /*num*/,         31 /*obadiah*/,
        60 /*1pet*/,       61 /*2pet*/,       57 /*philemon*/,    50 /*philip*/,
        20 /*prov*/,       19 /*psa*/,        66 /*Rev*/,         45 /*rom*/,
        8  /*ruth*/,       9  /*1sa*/,        10 /*2sa*/,         22 /*song*/,
        52 /*1thes*/,      53 /*2thes*/,      54 /*1tim*/,        55 /*2tim*/,
        56 /*titus*/,      38 /*zec*/,        36 /*zep*/    };

    return getBibleBookInfo(AlphaArray[BookNumber]);

}
//---------------------------------------------------------------------
// return the book in numerical order, where 1==Genesis, 2==exodus, etc
//---------------------------------------------------------------------
BibleBookInfoStructType getBibleBookInfo(int BookNumber)
{
    BibleBookInfoStructType bis;

   // memset(&bis, 0, sizeof(bis));

    bis.BookNumber = BookNumber;
    //myStrnCpy(bis.BookName, (char*)(BibleBook(BookNumber).toUtf8()) ,40);
    bis.BookName = BibleBook(BookNumber);

    if (BookNumber < 40)
        bis.Testament=(  OT_TABLE);
    else
        bis.Testament=( NT_TABLE);


    bis.TableName =bis.BookName; // will be overwritten for names with spaces
    if (BookNumber < 40)
        bis.Language =( LANGUAGE_HEBREW );
    else
        bis.Language =( LANGUAGE_GREEK );



#define myStrnCpy(a,b,c)   a=b
    //select case BookNumber
    switch (BookNumber)
    {
        default:
        case 1 : //gen
            bis.NumChapters = 50;
            bis.NumVerses = 1533;
            bis.BookSName = (char*) "gen";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfLawType;
            break;
        case 2 : //ex
            bis.NumChapters = 40;
            bis.NumVerses = 1213;
            bis.BookSName = (char*) "exo";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfLawType;
            break;
        case 3 : //lev
            bis.NumChapters = 27;
            bis.NumVerses = 1213;
            bis.BookSName = (char*) "lev";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfLawType;
            break;
        case 4://num
            bis.NumChapters = 36;
            bis.NumVerses = 1213;
            bis.BookSName = (char*) "num";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfLawType;
            break;
        case 5://deu
            bis.NumChapters = 34;
            bis.NumVerses = 1213;
            bis.BookSName = (char*) "deu";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfLawType;
            break;
        case 6://jos
            bis.NumChapters = 24;
            bis.NumVerses = 1213;
            bis.BookSName = (char*) "jos";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =BookOfHistoryType;
            break;
        case 7://jud
            bis.NumChapters = 21;
            bis.NumVerses = 1213;
            bis.BookSName = (char*) "jud";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =BookOfHistoryType;
            break;
        case 8://ruth
            bis.NumChapters = 4;
            bis.NumVerses = 1213;
            bis.BookSName = (char*) "rut";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfHistoryType;
            break;
        case 9://1 sam
            bis.NumChapters = 31;
            bis.NumVerses = 1213;
            myStrnCpy(bis.TableName , t09,40);
            bis.BookSName = (char*) "1sam";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =BookOfHistoryType;
            break;
        case 10://2 sam
            bis.NumChapters = 24;
            bis.NumVerses = 1213;
            myStrnCpy(bis.TableName , t10,40);
            bis.BookSName = (char*) "2sam";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =BookOfHistoryType;
            break;
        case 11://1 king
            bis.NumChapters = 22;
            bis.NumVerses = 1213;
            myStrnCpy(bis.TableName , t11,40);
            bis.BookSName = (char*) "1kin";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =BookOfHistoryType;
            break;
        case 12://2 ki
            bis.NumChapters = 25;
            bis.NumVerses = 1213;
            myStrnCpy(bis.TableName , t12,40);
            bis.BookSName= (char*) "2kin";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =BookOfHistoryType;
            break;
        case 13://1 cro
            bis.NumChapters = 29;
            bis.NumVerses = 1213;
            myStrnCpy(bis.TableName , t13,40);
            bis.BookSName = (char*) "1chr";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =BookOfHistoryType;
            break;
        case 14://2 cro
            bis.NumChapters = 36;
            bis.NumVerses = 1213;
            bis.BookSName = (char*) "2chr";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            myStrnCpy(bis.TableName , t14,40);
            bis.BookType =BookOfHistoryType;
            break;
        case 15://ezra
            bis.NumChapters = 10;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "ezr";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =BookOfHistoryType;
            break;
        case 16://neh
            bis.NumChapters = 13;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "neh";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfHistoryType;
            break;
        case 17://esthr
            bis.NumChapters = 10;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "est";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfHistoryType;
            break;
        case 18://job
            bis.NumChapters = 42;
            bis.NumVerses = 1213;
            bis.BookSName = (char*) "job";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =BookOfPoetryType;
            break;
        case 19://psalms
            bis.NumChapters = 150;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "psa";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfPoetryType;
            break;
        case 20://prov
            bis.NumChapters = 31;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "pro";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfPoetryType;
            break;
        case 21://ecc
            bis.NumChapters = 12;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "ecc";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfPoetryType;
            break;
        case 22://song
            bis.NumChapters = 8;
            bis.NumVerses = 1213;
            myStrnCpy(bis.TableName , t22,40);
            bis.BookSName = (char*) "sos";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfPoetryType;
            break;
        case 23://Isaiah
            bis.NumChapters = 66;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "isa";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =MajorProphetType;
            break;
        case 24://jer
            bis.NumChapters = 52;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "jer";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =MajorProphetType;
            break;
        case 25://lam
            bis.NumChapters = 5;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "lam";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =MajorProphetType;
            break;
        case 26://eze
            bis.NumChapters = 48;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "eze";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =MajorProphetType;
            break;
        case 27://dan
            bis.NumChapters = 12;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "dan";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =MajorProphetType;
            break;
        case 28://hos
            bis.NumChapters = 14;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "hos";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =MinorProphetType;
            break;
        case 29://joel
            bis.NumChapters = 3;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "joe";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =MinorProphetType;
            break;
        case 30://Amos
            bis.NumChapters = 9;
            bis.NumVerses = 1213;
            bis.BookSName = (char*) "amo";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =MinorProphetType;
            break;
        case 31://Obadiah
            bis.NumChapters = 1;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "oba";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =MinorProphetType;
            break;
        case 32://Jonah
            bis.NumChapters = 4;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "jon";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =MinorProphetType;
            break;
        case 33://mic
            bis.NumChapters = 7;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "mic";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =MinorProphetType;
            break;
        case 34://Nah
            bis.NumChapters = 3;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "nah";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =MinorProphetType;
            break;
        case 35://hab
            bis.NumChapters = 3;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "hab";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =MinorProphetType;
            break;
        case 36://zep
            bis.NumChapters = 3;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "zep";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =MinorProphetType;
            break;
        case 37://haggai
            bis.NumChapters = 2;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "hag";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =MinorProphetType;
            break;
        case 38://zec
            bis.NumChapters = 14;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "zec";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =MinorProphetType;
            break;
        case 39://mal
            bis.NumChapters = 4;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "mal";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =MinorProphetType;
            break;
        case 40://mat
            bis.NumChapters = 28;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "mat";
            bis.BookVSName="mt";
            bis.BookType =GospelsType;
            break;
        case 41://mark
            bis.NumChapters = 16;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "mar";
            bis.BookVSName="mr";
            bis.BookType =GospelsType;
            break;
        case 42://luk
            bis.NumChapters = 24;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "luk";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =GospelsType;
            break;
        case 43://john
            bis.NumChapters = 21;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "joh";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =GospelsType;
            break;
        case 44://acts
            bis.NumChapters = 28;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "act";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfHistoryType;
            break;
        case 45://rom
            bis.NumChapters = 16;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "rom";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =PaulineEpistlesType;
            break;
        case 46://1 cor
            bis.NumChapters = 16;
            bis.NumVerses = 1213;
            myStrnCpy(bis.TableName , t46,40);
            bis.BookSName= (char*) "1cor";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =PaulineEpistlesType;
            break;
        case 47://2 corinthians
            bis.NumChapters = 13;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "2cor";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            myStrnCpy(bis.TableName , t47,40);
            bis.BookType =PaulineEpistlesType;
            break;
        case 48://gal
            bis.NumChapters = 6;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "gal";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =PaulineEpistlesType;
            break;
        case 49://eph
            bis.NumChapters = 6;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "eph";
            bis.BookType =PaulineEpistlesType;
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            break;
        case 50://phil
            bis.NumChapters = 4;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "php";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =PaulineEpistlesType;
            break;
        case 51://col
            bis.NumChapters = 4;
            bis.NumVerses = 1533;
            bis.BookSName= (char*) "col";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =PaulineEpistlesType;
            break;
        case 52://1 the
            bis.NumChapters = 5;
            bis.NumVerses = 1213;
            myStrnCpy(bis.TableName , t52,40);
            bis.BookSName= (char*) "1the";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =PaulineEpistlesType;
            break;

        case 53://2 thes
            bis.NumChapters = 3;
            bis.NumVerses = 1213;
            myStrnCpy(bis.TableName , t53,40);
            bis.BookSName= (char*) "2the";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =PaulineEpistlesType;
            break;

        case 54://1 ti
            bis.NumChapters = 6;
            bis.NumVerses = 1213;
            myStrnCpy(bis.TableName , t54,40);
            bis.BookSName= (char*) "1tim";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =PaulineEpistlesType;
            break;

        case 55://2 ti
            bis.NumChapters = 4;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "2tim";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            myStrnCpy(bis.TableName , t55,40);
            bis.BookType =PaulineEpistlesType;
            break;

        case 56://tit
            bis.NumChapters = 3;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "tit";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            bis.BookType =PaulineEpistlesType;
            break;

        case 57://phil;
            bis.NumChapters = 1;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "phil";
            bis.BookVSName =( "phm");
            bis.BookType =PaulineEpistlesType;
            break;

        case 58://heb
            bis.NumChapters = 13;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "heb";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =PaulineEpistlesType;
            break;

        case 59://james
            bis.NumChapters = 5;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "jam";
            bis.BookVSName =( "jas");
            bis.BookType =GeneralEpistlesType;
            break;

        case 60://1 peter;
            bis.NumChapters = 5;
            bis.NumVerses = 1533;
            bis.BookSName= (char*) "1pet";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            myStrnCpy(bis.TableName , t60,40);
            bis.BookType =GeneralEpistlesType;
            break;

        case 61://2 peter
            bis.NumChapters = 3;
            bis.NumVerses = 1533;
            bis.BookSName= (char*) "2pet";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            myStrnCpy(bis.TableName , t61,40);
            bis.BookType =GeneralEpistlesType;
            break;

        case 62://1 john
            bis.NumChapters = 5;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "1joh";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            myStrnCpy(bis.TableName , t62,40);
            bis.BookType =GeneralEpistlesType;
            break;

        case 63 ://2 john
            bis.NumChapters = 1;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "2joh";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            myStrnCpy(bis.TableName , t63,40);
            bis.BookType =GeneralEpistlesType;
            break;

        case 64 ://3 john
            bis.NumChapters = 1;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "3joh";
            myStrnCpy(bis.BookVSName, bis.BookSName,3);
            myStrnCpy(bis.TableName , t64,40);
            bis.BookType =GeneralEpistlesType;
            break;

        case 65 ://jude
            bis.NumChapters = 1;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "jude";
            myStrnCpy(bis.BookVSName, bis.BookSName,4);
            bis.BookType =GeneralEpistlesType;
            break;

        case 66 ://Rev
            bis.NumChapters =22;
            bis.NumVerses = 1213;
            bis.BookSName= (char*) "rev";
            myStrnCpy(bis.BookVSName, bis.BookSName,2);
            bis.BookType =BookOfProphecyType;
            break;

    }


    return bis;
}


//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
void GetBookChapterVerse(int BookNumber, BookChapterVerseStructType& bcv)
{


    //BookChapterVerseStructType bcv ;

    bcv.BookNumber = BookNumber;
    bcv.NumberOfVerses[0]=1;
    bcv.NumberOfChapters=1;

    switch ( BookNumber)
    {


        case 1:
            bcv.NumberOfChapters = 50;;
            bcv.NumberOfVerses[1] = 31;
            bcv.NumberOfVerses[2] = 25;
            bcv.NumberOfVerses[3] = 24;
            bcv.NumberOfVerses[4] = 26;
            bcv.NumberOfVerses[5] = 32;
            bcv.NumberOfVerses[6] = 22;
            bcv.NumberOfVerses[7] = 24;
            bcv.NumberOfVerses[8] = 22;
            bcv.NumberOfVerses[9] = 29;
            bcv.NumberOfVerses[10] = 32;
            bcv.NumberOfVerses[11] = 32;
            bcv.NumberOfVerses[12] = 20;
            bcv.NumberOfVerses[13] = 18;
            bcv.NumberOfVerses[14] = 24;
            bcv.NumberOfVerses[15] = 21;
            bcv.NumberOfVerses[16] = 16;
            bcv.NumberOfVerses[17] = 27;
            bcv.NumberOfVerses[18] = 33;
            bcv.NumberOfVerses[19] = 38;
            bcv.NumberOfVerses[20] = 18;
            bcv.NumberOfVerses[21] = 34;
            bcv.NumberOfVerses[22] = 24;
            bcv.NumberOfVerses[23] = 20;
            bcv.NumberOfVerses[24] = 67;
            bcv.NumberOfVerses[25] = 34;
            bcv.NumberOfVerses[26] = 35;
            bcv.NumberOfVerses[27] = 46;
            bcv.NumberOfVerses[28] = 22;
            bcv.NumberOfVerses[29] = 35;
            bcv.NumberOfVerses[30] = 43;
            bcv.NumberOfVerses[31] = 55;
            bcv.NumberOfVerses[32] = 32;
            bcv.NumberOfVerses[33] = 20;
            bcv.NumberOfVerses[34] = 31;
            bcv.NumberOfVerses[35] = 29;
            bcv.NumberOfVerses[36] = 43;
            bcv.NumberOfVerses[37] = 36;
            bcv.NumberOfVerses[38] = 30;
            bcv.NumberOfVerses[39] = 23;
            bcv.NumberOfVerses[40] = 23;
            bcv.NumberOfVerses[41] = 57;
            bcv.NumberOfVerses[42] = 38;
            bcv.NumberOfVerses[43] = 34;
            bcv.NumberOfVerses[44] = 34;
            bcv.NumberOfVerses[45] = 28;
            bcv.NumberOfVerses[46] = 34;
            bcv.NumberOfVerses[47] = 31;
            bcv.NumberOfVerses[48] = 22;
            bcv.NumberOfVerses[49] = 33;
            bcv.NumberOfVerses[50] = 26;

            break;
        case 2:
            bcv.NumberOfChapters = 40;
            bcv.NumberOfVerses[1] = 22;
            bcv.NumberOfVerses[2] = 25;
            bcv.NumberOfVerses[3] = 22;
            bcv.NumberOfVerses[4] = 31;
            bcv.NumberOfVerses[5] = 23;
            bcv.NumberOfVerses[6] = 30;
            bcv.NumberOfVerses[7] = 25;
            bcv.NumberOfVerses[8] = 32;
            bcv.NumberOfVerses[9] = 35;
            bcv.NumberOfVerses[10] = 29;
            bcv.NumberOfVerses[11] = 10;
            bcv.NumberOfVerses[12] = 51;
            bcv.NumberOfVerses[13] = 22;
            bcv.NumberOfVerses[14] = 31;
            bcv.NumberOfVerses[15] = 27;
            bcv.NumberOfVerses[16] = 36;
            bcv.NumberOfVerses[17] = 16;
            bcv.NumberOfVerses[18] = 27;
            bcv.NumberOfVerses[19] = 25;
            bcv.NumberOfVerses[20] = 26;
            bcv.NumberOfVerses[21] = 36;
            bcv.NumberOfVerses[22] = 31;
            bcv.NumberOfVerses[23] = 33;
            bcv.NumberOfVerses[24] = 18;
            bcv.NumberOfVerses[25] = 40;
            bcv.NumberOfVerses[26] = 37;
            bcv.NumberOfVerses[27] = 21;
            bcv.NumberOfVerses[28] = 43;
            bcv.NumberOfVerses[29] = 46;
            bcv.NumberOfVerses[30] = 38;
            bcv.NumberOfVerses[31] = 18;
            bcv.NumberOfVerses[32] = 35;
            bcv.NumberOfVerses[33] = 23;
            bcv.NumberOfVerses[34] = 35;
            bcv.NumberOfVerses[35] = 35;
            bcv.NumberOfVerses[36] = 38;
            bcv.NumberOfVerses[37] = 29;
            bcv.NumberOfVerses[38] = 31;
            bcv.NumberOfVerses[39] = 43;
            bcv.NumberOfVerses[40] = 38;


            break;
        case 3:
            bcv.NumberOfChapters = 27;
            bcv.NumberOfVerses[1] = 17;
            bcv.NumberOfVerses[2] = 16;
            bcv.NumberOfVerses[3] = 17;
            bcv.NumberOfVerses[4] = 35;
            bcv.NumberOfVerses[5] = 19;
            bcv.NumberOfVerses[6] = 30;
            bcv.NumberOfVerses[7] = 38;
            bcv.NumberOfVerses[8] = 36;
            bcv.NumberOfVerses[9] = 24;
            bcv.NumberOfVerses[10] = 20;
            bcv.NumberOfVerses[11] = 47;
            bcv.NumberOfVerses[12] = 8;
            bcv.NumberOfVerses[13] = 59;
            bcv.NumberOfVerses[14] = 57;
            bcv.NumberOfVerses[15] = 33;
            bcv.NumberOfVerses[16] = 34;
            bcv.NumberOfVerses[17] = 16;
            bcv.NumberOfVerses[18] = 30;
            bcv.NumberOfVerses[19] = 37;
            bcv.NumberOfVerses[20] = 27;
            bcv.NumberOfVerses[21] = 24;
            bcv.NumberOfVerses[22] = 33;
            bcv.NumberOfVerses[23] = 44;
            bcv.NumberOfVerses[24] = 23;
            bcv.NumberOfVerses[25] = 55;
            bcv.NumberOfVerses[26] = 46;
            bcv.NumberOfVerses[27] = 34;


            break;
        case 4:
            bcv.NumberOfChapters = 36;
            bcv.NumberOfVerses[1] = 54;
            bcv.NumberOfVerses[2] = 34;
            bcv.NumberOfVerses[3] = 51;
            bcv.NumberOfVerses[4] = 49;
            bcv.NumberOfVerses[5] = 31;
            bcv.NumberOfVerses[6] = 27;
            bcv.NumberOfVerses[7] = 89;
            bcv.NumberOfVerses[8] = 26;
            bcv.NumberOfVerses[9] = 23;
            bcv.NumberOfVerses[10] = 36;
            bcv.NumberOfVerses[11] = 35;
            bcv.NumberOfVerses[12] = 16;
            bcv.NumberOfVerses[13] = 33;
            bcv.NumberOfVerses[14] = 45;
            bcv.NumberOfVerses[15] = 41;
            bcv.NumberOfVerses[16] = 50;
            bcv.NumberOfVerses[17] = 13;
            bcv.NumberOfVerses[18] = 32;
            bcv.NumberOfVerses[19] = 22;
            bcv.NumberOfVerses[20] = 29;
            bcv.NumberOfVerses[21] = 35;
            bcv.NumberOfVerses[22] = 41;
            bcv.NumberOfVerses[23] = 30;
            bcv.NumberOfVerses[24] = 25;
            bcv.NumberOfVerses[25] = 18;
            bcv.NumberOfVerses[26] = 65;
            bcv.NumberOfVerses[27] = 23;
            bcv.NumberOfVerses[28] = 31;
            bcv.NumberOfVerses[29] = 40;
            bcv.NumberOfVerses[30] = 16;
            bcv.NumberOfVerses[31] = 54;
            bcv.NumberOfVerses[32] = 42;
            bcv.NumberOfVerses[33] = 56;
            bcv.NumberOfVerses[34] = 29;
            bcv.NumberOfVerses[35] = 34;
            bcv.NumberOfVerses[36] = 13;


            break;

        case 5:
            bcv.NumberOfChapters = 34;
            bcv.NumberOfVerses[1] = 46;
            bcv.NumberOfVerses[2] = 37;
            bcv.NumberOfVerses[3] = 29;
            bcv.NumberOfVerses[4] = 49;
            bcv.NumberOfVerses[5] = 33;
            bcv.NumberOfVerses[6] = 25;
            bcv.NumberOfVerses[7] = 26;
            bcv.NumberOfVerses[8] = 20;
            bcv.NumberOfVerses[9] = 29;
            bcv.NumberOfVerses[10] = 22;
            bcv.NumberOfVerses[11] = 32;
            bcv.NumberOfVerses[12] = 32;
            bcv.NumberOfVerses[13] = 18;
            bcv.NumberOfVerses[14] = 29;
            bcv.NumberOfVerses[15] = 23;
            bcv.NumberOfVerses[16] = 22;
            bcv.NumberOfVerses[17] = 20;
            bcv.NumberOfVerses[18] = 22;
            bcv.NumberOfVerses[19] = 21;
            bcv.NumberOfVerses[20] = 20;
            bcv.NumberOfVerses[21] = 23;
            bcv.NumberOfVerses[22] = 30;
            bcv.NumberOfVerses[23] = 25;
            bcv.NumberOfVerses[24] = 22;
            bcv.NumberOfVerses[25] = 19;
            bcv.NumberOfVerses[26] = 19;
            bcv.NumberOfVerses[27] = 26;
            bcv.NumberOfVerses[28] = 68;
            bcv.NumberOfVerses[29] = 29;
            bcv.NumberOfVerses[30] = 20;
            bcv.NumberOfVerses[31] = 30;
            bcv.NumberOfVerses[32] = 52;
            bcv.NumberOfVerses[33] = 29;
            bcv.NumberOfVerses[34] = 12;


            break;
        case 6:
            bcv.NumberOfChapters = 24;
            bcv.NumberOfVerses[1] = 18;
            bcv.NumberOfVerses[2] = 24;
            bcv.NumberOfVerses[3] = 17;
            bcv.NumberOfVerses[4] = 24;
            bcv.NumberOfVerses[5] = 15;
            bcv.NumberOfVerses[6] = 27;
            bcv.NumberOfVerses[7] = 26;
            bcv.NumberOfVerses[8] = 35;
            bcv.NumberOfVerses[9] = 27;
            bcv.NumberOfVerses[10] = 43;
            bcv.NumberOfVerses[11] = 23;
            bcv.NumberOfVerses[12] = 24;
            bcv.NumberOfVerses[13] = 33;
            bcv.NumberOfVerses[14] = 15;
            bcv.NumberOfVerses[15] = 63;
            bcv.NumberOfVerses[16] = 10;
            bcv.NumberOfVerses[17] = 18;
            bcv.NumberOfVerses[18] = 28;
            bcv.NumberOfVerses[19] = 51;
            bcv.NumberOfVerses[20] = 9;
            bcv.NumberOfVerses[21] = 45;
            bcv.NumberOfVerses[22] = 34;
            bcv.NumberOfVerses[23] = 16;
            bcv.NumberOfVerses[24] = 33;


            break;
        case 7:
            bcv.NumberOfChapters = 21;
            bcv.NumberOfVerses[1] = 36;
            bcv.NumberOfVerses[2] = 23;
            bcv.NumberOfVerses[3] = 31;
            bcv.NumberOfVerses[4] = 24;
            bcv.NumberOfVerses[5] = 31;
            bcv.NumberOfVerses[6] = 40;
            bcv.NumberOfVerses[7] = 25;
            bcv.NumberOfVerses[8] = 35;
            bcv.NumberOfVerses[9] = 57;
            bcv.NumberOfVerses[10] = 18;
            bcv.NumberOfVerses[11] = 40;
            bcv.NumberOfVerses[12] = 15;
            bcv.NumberOfVerses[13] = 25;
            bcv.NumberOfVerses[14] = 20;
            bcv.NumberOfVerses[15] = 20;
            bcv.NumberOfVerses[16] = 31;
            bcv.NumberOfVerses[17] = 13;
            bcv.NumberOfVerses[18] = 31;
            bcv.NumberOfVerses[19] = 30;
            bcv.NumberOfVerses[20] = 48;
            bcv.NumberOfVerses[21] = 25;


            break;
        case 8:
            bcv.NumberOfChapters = 4;
            bcv.NumberOfVerses[1] = 22;
            bcv.NumberOfVerses[2] = 23;
            bcv.NumberOfVerses[3] = 18;
            bcv.NumberOfVerses[4] = 22;


            break;
        case 9:
            bcv.NumberOfChapters = 31;
            bcv.NumberOfVerses[1] = 28;
            bcv.NumberOfVerses[2] = 36;
            bcv.NumberOfVerses[3] = 21;
            bcv.NumberOfVerses[4] = 22;
            bcv.NumberOfVerses[5] = 12;
            bcv.NumberOfVerses[6] = 21;
            bcv.NumberOfVerses[7] = 17;
            bcv.NumberOfVerses[8] = 22;
            bcv.NumberOfVerses[9] = 27;
            bcv.NumberOfVerses[10] = 27;
            bcv.NumberOfVerses[11] = 15;
            bcv.NumberOfVerses[12] = 25;
            bcv.NumberOfVerses[13] = 23;
            bcv.NumberOfVerses[14] = 52;
            bcv.NumberOfVerses[15] = 35;
            bcv.NumberOfVerses[16] = 23;
            bcv.NumberOfVerses[17] = 58;
            bcv.NumberOfVerses[18] = 30;
            bcv.NumberOfVerses[19] = 24;
            bcv.NumberOfVerses[20] = 42;
            bcv.NumberOfVerses[21] = 15;
            bcv.NumberOfVerses[22] = 23;
            bcv.NumberOfVerses[23] = 29;
            bcv.NumberOfVerses[24] = 22;
            bcv.NumberOfVerses[25] = 44;
            bcv.NumberOfVerses[26] = 25;
            bcv.NumberOfVerses[27] = 12;
            bcv.NumberOfVerses[28] = 25;
            bcv.NumberOfVerses[29] = 11;
            bcv.NumberOfVerses[30] = 31;
            bcv.NumberOfVerses[31] = 13;


            break;
        case 10:
            bcv.NumberOfChapters = 24;
            bcv.NumberOfVerses[1] = 27;
            bcv.NumberOfVerses[2] = 32;
            bcv.NumberOfVerses[3] = 39;
            bcv.NumberOfVerses[4] = 12;
            bcv.NumberOfVerses[5] = 25;
            bcv.NumberOfVerses[6] = 23;
            bcv.NumberOfVerses[7] = 29;
            bcv.NumberOfVerses[8] = 18;
            bcv.NumberOfVerses[9] = 13;
            bcv.NumberOfVerses[10] = 19;
            bcv.NumberOfVerses[11] = 27;
            bcv.NumberOfVerses[12] = 31;
            bcv.NumberOfVerses[13] = 39;
            bcv.NumberOfVerses[14] = 33;
            bcv.NumberOfVerses[15] = 37;
            bcv.NumberOfVerses[16] = 23;
            bcv.NumberOfVerses[17] = 29;
            bcv.NumberOfVerses[18] = 33;
            bcv.NumberOfVerses[19] = 43;
            bcv.NumberOfVerses[20] = 26;
            bcv.NumberOfVerses[21] = 22;
            bcv.NumberOfVerses[22] = 51;
            bcv.NumberOfVerses[23] = 39;
            bcv.NumberOfVerses[24] = 25;


            break;
        case 11:
            bcv.NumberOfChapters = 22;
            bcv.NumberOfVerses[1] = 53;
            bcv.NumberOfVerses[2] = 46;
            bcv.NumberOfVerses[3] = 28;
            bcv.NumberOfVerses[4] = 34;
            bcv.NumberOfVerses[5] = 18;
            bcv.NumberOfVerses[6] = 38;
            bcv.NumberOfVerses[7] = 51;
            bcv.NumberOfVerses[8] = 66;
            bcv.NumberOfVerses[9] = 28;
            bcv.NumberOfVerses[10] = 29;
            bcv.NumberOfVerses[11] = 43;
            bcv.NumberOfVerses[12] = 33;
            bcv.NumberOfVerses[13] = 34;
            bcv.NumberOfVerses[14] = 31;
            bcv.NumberOfVerses[15] = 34;
            bcv.NumberOfVerses[16] = 34;
            bcv.NumberOfVerses[17] = 24;
            bcv.NumberOfVerses[18] = 46;
            bcv.NumberOfVerses[19] = 21;
            bcv.NumberOfVerses[20] = 43;
            bcv.NumberOfVerses[21] = 29;
            bcv.NumberOfVerses[22] = 53;


            break;
        case 12:
            bcv.NumberOfChapters = 25;
            bcv.NumberOfVerses[1] = 18;
            bcv.NumberOfVerses[2] = 25;
            bcv.NumberOfVerses[3] = 27;
            bcv.NumberOfVerses[4] = 44;
            bcv.NumberOfVerses[5] = 27;
            bcv.NumberOfVerses[6] = 33;
            bcv.NumberOfVerses[7] = 20;
            bcv.NumberOfVerses[8] = 29;
            bcv.NumberOfVerses[9] = 37;
            bcv.NumberOfVerses[10] = 36;
            bcv.NumberOfVerses[11] = 21;
            bcv.NumberOfVerses[12] = 21;
            bcv.NumberOfVerses[13] = 25;
            bcv.NumberOfVerses[14] = 29;
            bcv.NumberOfVerses[15] = 38;
            bcv.NumberOfVerses[16] = 20;
            bcv.NumberOfVerses[17] = 41;
            bcv.NumberOfVerses[18] = 37;
            bcv.NumberOfVerses[19] = 37;
            bcv.NumberOfVerses[20] = 21;
            bcv.NumberOfVerses[21] = 26;
            bcv.NumberOfVerses[22] = 20;
            bcv.NumberOfVerses[23] = 37;
            bcv.NumberOfVerses[24] = 20;
            bcv.NumberOfVerses[25] = 30;


            break;
        case 13:
            bcv.NumberOfChapters = 29;
            bcv.NumberOfVerses[1] = 54;
            bcv.NumberOfVerses[2] = 55;
            bcv.NumberOfVerses[3] = 24;
            bcv.NumberOfVerses[4] = 43;
            bcv.NumberOfVerses[5] = 26;
            bcv.NumberOfVerses[6] = 81;
            bcv.NumberOfVerses[7] = 40;
            bcv.NumberOfVerses[8] = 40;
            bcv.NumberOfVerses[9] = 44;
            bcv.NumberOfVerses[10] = 14;
            bcv.NumberOfVerses[11] = 47;
            bcv.NumberOfVerses[12] = 40;
            bcv.NumberOfVerses[13] = 14;
            bcv.NumberOfVerses[14] = 17;
            bcv.NumberOfVerses[15] = 29;
            bcv.NumberOfVerses[16] = 43;
            bcv.NumberOfVerses[17] = 27;
            bcv.NumberOfVerses[18] = 17;
            bcv.NumberOfVerses[19] = 19;
            bcv.NumberOfVerses[20] = 8;
            bcv.NumberOfVerses[21] = 30;
            bcv.NumberOfVerses[22] = 19;
            bcv.NumberOfVerses[23] = 32;
            bcv.NumberOfVerses[24] = 31;
            bcv.NumberOfVerses[25] = 31;
            bcv.NumberOfVerses[26] = 32;
            bcv.NumberOfVerses[27] = 34;
            bcv.NumberOfVerses[28] = 21;
            bcv.NumberOfVerses[29] = 30;


            break;
        case 14:
            bcv.NumberOfChapters = 36;
            bcv.NumberOfVerses[1] = 17;
            bcv.NumberOfVerses[2] = 18;
            bcv.NumberOfVerses[3] = 17;
            bcv.NumberOfVerses[4] = 22;
            bcv.NumberOfVerses[5] = 14;
            bcv.NumberOfVerses[6] = 42;
            bcv.NumberOfVerses[7] = 22;
            bcv.NumberOfVerses[8] = 18;
            bcv.NumberOfVerses[9] = 31;
            bcv.NumberOfVerses[10] = 19;
            bcv.NumberOfVerses[11] = 23;
            bcv.NumberOfVerses[12] = 16;
            bcv.NumberOfVerses[13] = 22;
            bcv.NumberOfVerses[14] = 15;
            bcv.NumberOfVerses[15] = 19;
            bcv.NumberOfVerses[16] = 14;
            bcv.NumberOfVerses[17] = 19;
            bcv.NumberOfVerses[18] = 34;
            bcv.NumberOfVerses[19] = 11;
            bcv.NumberOfVerses[20] = 37;
            bcv.NumberOfVerses[21] = 20;
            bcv.NumberOfVerses[22] = 12;
            bcv.NumberOfVerses[23] = 21;
            bcv.NumberOfVerses[24] = 27;
            bcv.NumberOfVerses[25] = 28;
            bcv.NumberOfVerses[26] = 23;
            bcv.NumberOfVerses[27] = 9;
            bcv.NumberOfVerses[28] = 27;
            bcv.NumberOfVerses[29] = 36;
            bcv.NumberOfVerses[30] = 27;
            bcv.NumberOfVerses[31] = 21;
            bcv.NumberOfVerses[32] = 33;
            bcv.NumberOfVerses[33] = 25;
            bcv.NumberOfVerses[34] = 33;
            bcv.NumberOfVerses[35] = 27;
            bcv.NumberOfVerses[36] = 23;


            break;
        case 15:
            bcv.NumberOfChapters = 10;
            bcv.NumberOfVerses[1] = 11;
            bcv.NumberOfVerses[2] = 70;
            bcv.NumberOfVerses[3] = 13;
            bcv.NumberOfVerses[4] = 24;
            bcv.NumberOfVerses[5] = 17;
            bcv.NumberOfVerses[6] = 22;
            bcv.NumberOfVerses[7] = 28;
            bcv.NumberOfVerses[8] = 36;
            bcv.NumberOfVerses[9] = 15;
            bcv.NumberOfVerses[10] = 44;


            break;
        case 16:
            bcv.NumberOfChapters = 13;
            bcv.NumberOfVerses[1] = 11;
            bcv.NumberOfVerses[2] = 20;
            bcv.NumberOfVerses[3] = 32;
            bcv.NumberOfVerses[4] = 23;
            bcv.NumberOfVerses[5] = 19;
            bcv.NumberOfVerses[6] = 19;
            bcv.NumberOfVerses[7] = 73;
            bcv.NumberOfVerses[8] = 18;
            bcv.NumberOfVerses[9] = 38;
            bcv.NumberOfVerses[10] = 39;
            bcv.NumberOfVerses[11] = 36;
            bcv.NumberOfVerses[12] = 47;
            bcv.NumberOfVerses[13] = 31;


            break;
        case 17:
            bcv.NumberOfChapters = 10;
            bcv.NumberOfVerses[1] = 22;
            bcv.NumberOfVerses[2] = 23;
            bcv.NumberOfVerses[3] = 15;
            bcv.NumberOfVerses[4] = 17;
            bcv.NumberOfVerses[5] = 14;
            bcv.NumberOfVerses[6] = 14;
            bcv.NumberOfVerses[7] = 10;
            bcv.NumberOfVerses[8] = 17;
            bcv.NumberOfVerses[9] = 32;
            bcv.NumberOfVerses[10] = 3;


            break;
        case 18:
            bcv.NumberOfChapters = 42;
            bcv.NumberOfVerses[1] = 22;
            bcv.NumberOfVerses[2] = 13;
            bcv.NumberOfVerses[3] = 26;
            bcv.NumberOfVerses[4] = 21;
            bcv.NumberOfVerses[5] = 27;
            bcv.NumberOfVerses[6] = 30;
            bcv.NumberOfVerses[7] = 21;
            bcv.NumberOfVerses[8] = 22;
            bcv.NumberOfVerses[9] = 35;
            bcv.NumberOfVerses[10] = 22;
            bcv.NumberOfVerses[11] = 20;
            bcv.NumberOfVerses[12] = 25;
            bcv.NumberOfVerses[13] = 28;
            bcv.NumberOfVerses[14] = 22;
            bcv.NumberOfVerses[15] = 35;
            bcv.NumberOfVerses[16] = 22;
            bcv.NumberOfVerses[17] = 16;
            bcv.NumberOfVerses[18] = 21;
            bcv.NumberOfVerses[19] = 29;
            bcv.NumberOfVerses[20] = 29;
            bcv.NumberOfVerses[21] = 34;
            bcv.NumberOfVerses[22] = 30;
            bcv.NumberOfVerses[23] = 17;
            bcv.NumberOfVerses[24] = 25;
            bcv.NumberOfVerses[25] = 6;
            bcv.NumberOfVerses[26] = 14;
            bcv.NumberOfVerses[27] = 23;
            bcv.NumberOfVerses[28] = 28;
            bcv.NumberOfVerses[29] = 25;
            bcv.NumberOfVerses[30] = 31;
            bcv.NumberOfVerses[31] = 40;
            bcv.NumberOfVerses[32] = 22;
            bcv.NumberOfVerses[33] = 33;
            bcv.NumberOfVerses[34] = 37;
            bcv.NumberOfVerses[35] = 16;
            bcv.NumberOfVerses[36] = 33;
            bcv.NumberOfVerses[37] = 24;
            bcv.NumberOfVerses[38] = 41;
            bcv.NumberOfVerses[39] = 30;
            bcv.NumberOfVerses[40] = 24;
            bcv.NumberOfVerses[41] = 34;
            bcv.NumberOfVerses[42] = 17;


            break;
        case 19:
            bcv.NumberOfChapters = 150;
            bcv.NumberOfVerses[1] = 6;
            bcv.NumberOfVerses[2] = 12;
            bcv.NumberOfVerses[3] = 8;
            bcv.NumberOfVerses[4] = 8;
            bcv.NumberOfVerses[5] = 12;
            bcv.NumberOfVerses[6] = 10;
            bcv.NumberOfVerses[7] = 17;
            bcv.NumberOfVerses[8] = 9;
            bcv.NumberOfVerses[9] = 20;
            bcv.NumberOfVerses[10] = 18;
            bcv.NumberOfVerses[11] = 7;
            bcv.NumberOfVerses[12] = 8;
            bcv.NumberOfVerses[13] = 6;
            bcv.NumberOfVerses[14] = 7;
            bcv.NumberOfVerses[15] = 5;
            bcv.NumberOfVerses[16] = 11;
            bcv.NumberOfVerses[17] = 15;
            bcv.NumberOfVerses[18] = 50;
            bcv.NumberOfVerses[19] = 14;
            bcv.NumberOfVerses[20] = 9;
            bcv.NumberOfVerses[21] = 13;
            bcv.NumberOfVerses[22] = 31;
            bcv.NumberOfVerses[23] = 6;
            bcv.NumberOfVerses[24] = 10;
            bcv.NumberOfVerses[25] = 22;
            bcv.NumberOfVerses[26] = 12;
            bcv.NumberOfVerses[27] = 14;
            bcv.NumberOfVerses[28] = 9;
            bcv.NumberOfVerses[29] = 11;
            bcv.NumberOfVerses[30] = 12;
            bcv.NumberOfVerses[31] = 24;
            bcv.NumberOfVerses[32] = 11;
            bcv.NumberOfVerses[33] = 22;
            bcv.NumberOfVerses[34] = 22;
            bcv.NumberOfVerses[35] = 28;
            bcv.NumberOfVerses[36] = 12;
            bcv.NumberOfVerses[37] = 40;
            bcv.NumberOfVerses[38] = 22;
            bcv.NumberOfVerses[39] = 13;
            bcv.NumberOfVerses[40] = 17;
            bcv.NumberOfVerses[41] = 13;
            bcv.NumberOfVerses[42] = 11;
            bcv.NumberOfVerses[43] = 5;
            bcv.NumberOfVerses[44] = 26;
            bcv.NumberOfVerses[45] = 17;
            bcv.NumberOfVerses[46] = 11;
            bcv.NumberOfVerses[47] = 9;
            bcv.NumberOfVerses[48] = 14;
            bcv.NumberOfVerses[49] = 20;
            bcv.NumberOfVerses[50] = 23;
            bcv.NumberOfVerses[51] = 19;
            bcv.NumberOfVerses[52] = 9;
            bcv.NumberOfVerses[53] = 6;
            bcv.NumberOfVerses[54] = 7;
            bcv.NumberOfVerses[55] = 23;
            bcv.NumberOfVerses[56] = 13;
            bcv.NumberOfVerses[57] = 11;
            bcv.NumberOfVerses[58] = 11;
            bcv.NumberOfVerses[59] = 17;
            bcv.NumberOfVerses[60] = 12;
            bcv.NumberOfVerses[61] = 8;
            bcv.NumberOfVerses[62] = 12;
            bcv.NumberOfVerses[63] = 11;
            bcv.NumberOfVerses[64] = 10;
            bcv.NumberOfVerses[65] = 13;
            bcv.NumberOfVerses[66] = 20;
            bcv.NumberOfVerses[67] = 7;
            bcv.NumberOfVerses[68] = 35;
            bcv.NumberOfVerses[69] = 36;
            bcv.NumberOfVerses[70] = 5;
            bcv.NumberOfVerses[71] = 24;
            bcv.NumberOfVerses[72] = 20;
            bcv.NumberOfVerses[73] = 28;
            bcv.NumberOfVerses[74] = 23;
            bcv.NumberOfVerses[75] = 10;
            bcv.NumberOfVerses[76] = 12;
            bcv.NumberOfVerses[77] = 20;
            bcv.NumberOfVerses[78] = 72;
            bcv.NumberOfVerses[79] = 13;
            bcv.NumberOfVerses[80] = 19;
            bcv.NumberOfVerses[81] = 16;
            bcv.NumberOfVerses[82] = 8;
            bcv.NumberOfVerses[83] = 18;
            bcv.NumberOfVerses[84] = 12;
            bcv.NumberOfVerses[85] = 13;
            bcv.NumberOfVerses[86] = 17;
            bcv.NumberOfVerses[87] = 7;
            bcv.NumberOfVerses[88] = 18;
            bcv.NumberOfVerses[89] = 52;
            bcv.NumberOfVerses[90] = 17;
            bcv.NumberOfVerses[91] = 16;
            bcv.NumberOfVerses[92] = 15;
            bcv.NumberOfVerses[93] = 5;
            bcv.NumberOfVerses[94] = 23;
            bcv.NumberOfVerses[95] = 11;
            bcv.NumberOfVerses[96] = 13;
            bcv.NumberOfVerses[97] = 12;
            bcv.NumberOfVerses[98] = 9;
            bcv.NumberOfVerses[99] = 9;
            bcv.NumberOfVerses[100] = 5;
            bcv.NumberOfVerses[101] = 8;
            bcv.NumberOfVerses[102] = 28;
            bcv.NumberOfVerses[103] = 22;
            bcv.NumberOfVerses[104] = 35;
            bcv.NumberOfVerses[105] = 45;
            bcv.NumberOfVerses[106] = 48;
            bcv.NumberOfVerses[107] = 43;
            bcv.NumberOfVerses[108] = 13;
            bcv.NumberOfVerses[109] = 31;
            bcv.NumberOfVerses[110] = 7;
            bcv.NumberOfVerses[111] = 10;
            bcv.NumberOfVerses[112] = 10;
            bcv.NumberOfVerses[113] = 9;
            bcv.NumberOfVerses[114] = 8;
            bcv.NumberOfVerses[115] = 18;
            bcv.NumberOfVerses[116] = 19;
            bcv.NumberOfVerses[117] = 2;
            bcv.NumberOfVerses[118] = 29;
            bcv.NumberOfVerses[119] = 176;
            bcv.NumberOfVerses[120] = 7;
            bcv.NumberOfVerses[121] = 8;
            bcv.NumberOfVerses[122] = 9;
            bcv.NumberOfVerses[123] = 4;
            bcv.NumberOfVerses[124] = 8;
            bcv.NumberOfVerses[125] = 5;
            bcv.NumberOfVerses[126] = 6;
            bcv.NumberOfVerses[127] = 5;
            bcv.NumberOfVerses[128] = 6;
            bcv.NumberOfVerses[129] = 8;
            bcv.NumberOfVerses[130] = 8;
            bcv.NumberOfVerses[131] = 3;
            bcv.NumberOfVerses[132] = 18;
            bcv.NumberOfVerses[133] = 3;
            bcv.NumberOfVerses[134] = 3;
            bcv.NumberOfVerses[135] = 21;
            bcv.NumberOfVerses[136] = 26;
            bcv.NumberOfVerses[137] = 9;
            bcv.NumberOfVerses[138] = 8;
            bcv.NumberOfVerses[139] = 24;
            bcv.NumberOfVerses[140] = 13;
            bcv.NumberOfVerses[141] = 10;
            bcv.NumberOfVerses[142] = 7;
            bcv.NumberOfVerses[143] = 12;
            bcv.NumberOfVerses[144] = 15;
            bcv.NumberOfVerses[145] = 21;
            bcv.NumberOfVerses[146] = 10;
            bcv.NumberOfVerses[147] = 20;
            bcv.NumberOfVerses[148] = 14;
            bcv.NumberOfVerses[149] = 9;
            bcv.NumberOfVerses[150] = 6;


            break;
        case 20:
            bcv.NumberOfChapters = 31;
            bcv.NumberOfVerses[1] = 33;
            bcv.NumberOfVerses[2] = 22;
            bcv.NumberOfVerses[3] = 35;
            bcv.NumberOfVerses[4] = 27;
            bcv.NumberOfVerses[5] = 23;
            bcv.NumberOfVerses[6] = 35;
            bcv.NumberOfVerses[7] = 27;
            bcv.NumberOfVerses[8] = 36;
            bcv.NumberOfVerses[9] = 18;
            bcv.NumberOfVerses[10] = 32;
            bcv.NumberOfVerses[11] = 31;
            bcv.NumberOfVerses[12] = 28;
            bcv.NumberOfVerses[13] = 25;
            bcv.NumberOfVerses[14] = 35;
            bcv.NumberOfVerses[15] = 33;
            bcv.NumberOfVerses[16] = 33;
            bcv.NumberOfVerses[17] = 28;
            bcv.NumberOfVerses[18] = 24;
            bcv.NumberOfVerses[19] = 29;
            bcv.NumberOfVerses[20] = 30;
            bcv.NumberOfVerses[21] = 31;
            bcv.NumberOfVerses[22] = 29;
            bcv.NumberOfVerses[23] = 35;
            bcv.NumberOfVerses[24] = 34;
            bcv.NumberOfVerses[25] = 28;
            bcv.NumberOfVerses[26] = 28;
            bcv.NumberOfVerses[27] = 27;
            bcv.NumberOfVerses[28] = 28;
            bcv.NumberOfVerses[29] = 27;
            bcv.NumberOfVerses[30] = 33;
            bcv.NumberOfVerses[31] = 31;


            break;
        case 21:
            bcv.NumberOfChapters = 12;
            bcv.NumberOfVerses[1] = 18;
            bcv.NumberOfVerses[2] = 26;
            bcv.NumberOfVerses[3] = 22;
            bcv.NumberOfVerses[4] = 16;
            bcv.NumberOfVerses[5] = 20;
            bcv.NumberOfVerses[6] = 12;
            bcv.NumberOfVerses[7] = 29;
            bcv.NumberOfVerses[8] = 17;
            bcv.NumberOfVerses[9] = 18;
            bcv.NumberOfVerses[10] = 20;
            bcv.NumberOfVerses[11] = 10;
            bcv.NumberOfVerses[12] = 14;


            break;
        case 22:
            bcv.NumberOfChapters = 8;
            bcv.NumberOfVerses[1] = 17;
            bcv.NumberOfVerses[2] = 17;
            bcv.NumberOfVerses[3] = 11;
            bcv.NumberOfVerses[4] = 16;
            bcv.NumberOfVerses[5] = 16;
            bcv.NumberOfVerses[6] = 13;
            bcv.NumberOfVerses[7] = 13;
            bcv.NumberOfVerses[8] = 14;


            break;
        case 23:
            bcv.NumberOfChapters = 66;
            bcv.NumberOfVerses[1] = 31;
            bcv.NumberOfVerses[2] = 22;
            bcv.NumberOfVerses[3] = 26;
            bcv.NumberOfVerses[4] = 6;
            bcv.NumberOfVerses[5] = 30;
            bcv.NumberOfVerses[6] = 13;
            bcv.NumberOfVerses[7] = 25;
            bcv.NumberOfVerses[8] = 22;
            bcv.NumberOfVerses[9] = 21;
            bcv.NumberOfVerses[10] = 34;
            bcv.NumberOfVerses[11] = 16;
            bcv.NumberOfVerses[12] = 6;
            bcv.NumberOfVerses[13] = 22;
            bcv.NumberOfVerses[14] = 32;
            bcv.NumberOfVerses[15] = 9;
            bcv.NumberOfVerses[16] = 14;
            bcv.NumberOfVerses[17] = 14;
            bcv.NumberOfVerses[18] = 7;
            bcv.NumberOfVerses[19] = 25;
            bcv.NumberOfVerses[20] = 6;
            bcv.NumberOfVerses[21] = 17;
            bcv.NumberOfVerses[22] = 25;
            bcv.NumberOfVerses[23] = 18;
            bcv.NumberOfVerses[24] = 23;
            bcv.NumberOfVerses[25] = 12;
            bcv.NumberOfVerses[26] = 21;
            bcv.NumberOfVerses[27] = 13;
            bcv.NumberOfVerses[28] = 29;
            bcv.NumberOfVerses[29] = 24;
            bcv.NumberOfVerses[30] = 33;
            bcv.NumberOfVerses[31] = 9;
            bcv.NumberOfVerses[32] = 20;
            bcv.NumberOfVerses[33] = 24;
            bcv.NumberOfVerses[34] = 17;
            bcv.NumberOfVerses[35] = 10;
            bcv.NumberOfVerses[36] = 22;
            bcv.NumberOfVerses[37] = 38;
            bcv.NumberOfVerses[38] = 22;
            bcv.NumberOfVerses[39] = 8;
            bcv.NumberOfVerses[40] = 31;
            bcv.NumberOfVerses[41] = 29;
            bcv.NumberOfVerses[42] = 25;
            bcv.NumberOfVerses[43] = 28;
            bcv.NumberOfVerses[44] = 28;
            bcv.NumberOfVerses[45] = 25;
            bcv.NumberOfVerses[46] = 13;
            bcv.NumberOfVerses[47] = 15;
            bcv.NumberOfVerses[48] = 22;
            bcv.NumberOfVerses[49] = 26;
            bcv.NumberOfVerses[50] = 11;
            bcv.NumberOfVerses[51] = 23;
            bcv.NumberOfVerses[52] = 15;
            bcv.NumberOfVerses[53] = 12;
            bcv.NumberOfVerses[54] = 17;
            bcv.NumberOfVerses[55] = 13;
            bcv.NumberOfVerses[56] = 12;
            bcv.NumberOfVerses[57] = 21;
            bcv.NumberOfVerses[58] = 14;
            bcv.NumberOfVerses[59] = 21;
            bcv.NumberOfVerses[60] = 22;
            bcv.NumberOfVerses[61] = 11;
            bcv.NumberOfVerses[62] = 12;
            bcv.NumberOfVerses[63] = 19;
            bcv.NumberOfVerses[64] = 12;
            bcv.NumberOfVerses[65] = 25;
            bcv.NumberOfVerses[66] = 24;


            break;
        case 24:
            bcv.NumberOfChapters = 52;
            bcv.NumberOfVerses[1] = 19;
            bcv.NumberOfVerses[2] = 37;
            bcv.NumberOfVerses[3] = 25;
            bcv.NumberOfVerses[4] = 31;
            bcv.NumberOfVerses[5] = 31;
            bcv.NumberOfVerses[6] = 30;
            bcv.NumberOfVerses[7] = 34;
            bcv.NumberOfVerses[8] = 22;
            bcv.NumberOfVerses[9] = 26;
            bcv.NumberOfVerses[10] = 25;
            bcv.NumberOfVerses[11] = 23;
            bcv.NumberOfVerses[12] = 17;
            bcv.NumberOfVerses[13] = 27;
            bcv.NumberOfVerses[14] = 22;
            bcv.NumberOfVerses[15] = 21;
            bcv.NumberOfVerses[16] = 21;
            bcv.NumberOfVerses[17] = 27;
            bcv.NumberOfVerses[18] = 23;
            bcv.NumberOfVerses[19] = 15;
            bcv.NumberOfVerses[20] = 18;
            bcv.NumberOfVerses[21] = 14;
            bcv.NumberOfVerses[22] = 30;
            bcv.NumberOfVerses[23] = 40;
            bcv.NumberOfVerses[24] = 10;
            bcv.NumberOfVerses[25] = 38;
            bcv.NumberOfVerses[26] = 24;
            bcv.NumberOfVerses[27] = 22;
            bcv.NumberOfVerses[28] = 17;
            bcv.NumberOfVerses[29] = 32;
            bcv.NumberOfVerses[30] = 24;
            bcv.NumberOfVerses[31] = 40;
            bcv.NumberOfVerses[32] = 44;
            bcv.NumberOfVerses[33] = 26;
            bcv.NumberOfVerses[34] = 22;
            bcv.NumberOfVerses[35] = 19;
            bcv.NumberOfVerses[36] = 32;
            bcv.NumberOfVerses[37] = 21;
            bcv.NumberOfVerses[38] = 28;
            bcv.NumberOfVerses[39] = 18;
            bcv.NumberOfVerses[40] = 16;
            bcv.NumberOfVerses[41] = 18;
            bcv.NumberOfVerses[42] = 22;
            bcv.NumberOfVerses[43] = 13;
            bcv.NumberOfVerses[44] = 30;
            bcv.NumberOfVerses[45] = 5;
            bcv.NumberOfVerses[46] = 28;
            bcv.NumberOfVerses[47] = 7;
            bcv.NumberOfVerses[48] = 47;
            bcv.NumberOfVerses[49] = 39;
            bcv.NumberOfVerses[50] = 46;
            bcv.NumberOfVerses[51] = 64;
            bcv.NumberOfVerses[52] = 34;


            break;
        case 25:
            bcv.NumberOfChapters = 5;
            bcv.NumberOfVerses[1] = 22;
            bcv.NumberOfVerses[2] = 22;
            bcv.NumberOfVerses[3] = 66;
            bcv.NumberOfVerses[4] = 22;
            bcv.NumberOfVerses[5] = 22;


            break;
        case 26:
            bcv.NumberOfChapters = 48;
            bcv.NumberOfVerses[1] = 28;
            bcv.NumberOfVerses[2] = 10;
            bcv.NumberOfVerses[3] = 27;
            bcv.NumberOfVerses[4] = 17;
            bcv.NumberOfVerses[5] = 17;
            bcv.NumberOfVerses[6] = 14;
            bcv.NumberOfVerses[7] = 27;
            bcv.NumberOfVerses[8] = 18;
            bcv.NumberOfVerses[9] = 11;
            bcv.NumberOfVerses[10] = 22;
            bcv.NumberOfVerses[11] = 25;
            bcv.NumberOfVerses[12] = 28;
            bcv.NumberOfVerses[13] = 23;
            bcv.NumberOfVerses[14] = 23;
            bcv.NumberOfVerses[15] = 8;
            bcv.NumberOfVerses[16] = 63;
            bcv.NumberOfVerses[17] = 24;
            bcv.NumberOfVerses[18] = 32;
            bcv.NumberOfVerses[19] = 14;
            bcv.NumberOfVerses[20] = 49;
            bcv.NumberOfVerses[21] = 32;
            bcv.NumberOfVerses[22] = 31;
            bcv.NumberOfVerses[23] = 49;
            bcv.NumberOfVerses[24] = 27;
            bcv.NumberOfVerses[25] = 17;
            bcv.NumberOfVerses[26] = 21;
            bcv.NumberOfVerses[27] = 36;
            bcv.NumberOfVerses[28] = 26;
            bcv.NumberOfVerses[29] = 21;
            bcv.NumberOfVerses[30] = 26;
            bcv.NumberOfVerses[31] = 18;
            bcv.NumberOfVerses[32] = 32;
            bcv.NumberOfVerses[33] = 33;
            bcv.NumberOfVerses[34] = 31;
            bcv.NumberOfVerses[35] = 15;
            bcv.NumberOfVerses[36] = 38;
            bcv.NumberOfVerses[37] = 28;
            bcv.NumberOfVerses[38] = 23;
            bcv.NumberOfVerses[39] = 29;
            bcv.NumberOfVerses[40] = 49;
            bcv.NumberOfVerses[41] = 26;
            bcv.NumberOfVerses[42] = 20;
            bcv.NumberOfVerses[43] = 27;
            bcv.NumberOfVerses[44] = 31;
            bcv.NumberOfVerses[45] = 25;
            bcv.NumberOfVerses[46] = 24;
            bcv.NumberOfVerses[47] = 23;
            bcv.NumberOfVerses[48] = 35;


            break;
        case 27:
            bcv.NumberOfChapters = 12;
            bcv.NumberOfVerses[1] = 21;
            bcv.NumberOfVerses[2] = 49;
            bcv.NumberOfVerses[3] = 30;
            bcv.NumberOfVerses[4] = 37;
            bcv.NumberOfVerses[5] = 31;
            bcv.NumberOfVerses[6] = 28;
            bcv.NumberOfVerses[7] = 28;
            bcv.NumberOfVerses[8] = 27;
            bcv.NumberOfVerses[9] = 27;
            bcv.NumberOfVerses[10] = 21;
            bcv.NumberOfVerses[11] = 45;
            bcv.NumberOfVerses[12] = 13;


            break;
        case 28:
            bcv.NumberOfChapters = 14;
            bcv.NumberOfVerses[1] = 11;
            bcv.NumberOfVerses[2] = 23;
            bcv.NumberOfVerses[3] = 5;
            bcv.NumberOfVerses[4] = 19;
            bcv.NumberOfVerses[5] = 15;
            bcv.NumberOfVerses[6] = 11;
            bcv.NumberOfVerses[7] = 16;
            bcv.NumberOfVerses[8] = 14;
            bcv.NumberOfVerses[9] = 17;
            bcv.NumberOfVerses[10] = 15;
            bcv.NumberOfVerses[11] = 12;
            bcv.NumberOfVerses[12] = 14;
            bcv.NumberOfVerses[13] = 16;
            bcv.NumberOfVerses[14] = 9;


            break;
        case 29:
            bcv.NumberOfChapters = 3;
            bcv.NumberOfVerses[1] = 20;
            bcv.NumberOfVerses[2] = 32;
            bcv.NumberOfVerses[3] = 21;


            break;
        case 30:
            bcv.NumberOfChapters = 9;
            bcv.NumberOfVerses[1] = 15;
            bcv.NumberOfVerses[2] = 16;
            bcv.NumberOfVerses[3] = 15;
            bcv.NumberOfVerses[4] = 13;
            bcv.NumberOfVerses[5] = 27;
            bcv.NumberOfVerses[6] = 14;
            bcv.NumberOfVerses[7] = 17;
            bcv.NumberOfVerses[8] = 14;
            bcv.NumberOfVerses[9] = 15;


            break;
        case 31:
            bcv.NumberOfChapters = 1;
            bcv.NumberOfVerses[1] = 21;


            break;
        case 32:
            bcv.NumberOfChapters = 4;
            bcv.NumberOfVerses[1] = 17;
            bcv.NumberOfVerses[2] = 10;
            bcv.NumberOfVerses[3] = 10;
            bcv.NumberOfVerses[4] = 11;


            break;
        case 33:
            bcv.NumberOfChapters = 7;
            bcv.NumberOfVerses[1] = 16;
            bcv.NumberOfVerses[2] = 13;
            bcv.NumberOfVerses[3] = 12;
            bcv.NumberOfVerses[4] = 13;
            bcv.NumberOfVerses[5] = 15;
            bcv.NumberOfVerses[6] = 16;
            bcv.NumberOfVerses[7] = 20;


            break;
        case 34:
            bcv.NumberOfChapters = 3;
            bcv.NumberOfVerses[1] = 15;
            bcv.NumberOfVerses[2] = 13;
            bcv.NumberOfVerses[3] = 19;


            break;
        case 35:
            bcv.NumberOfChapters = 3;
            bcv.NumberOfVerses[1] = 17;
            bcv.NumberOfVerses[2] = 20;
            bcv.NumberOfVerses[3] = 19;


            break;
        case 36:
            bcv.NumberOfChapters = 3;
            bcv.NumberOfVerses[1] = 18;
            bcv.NumberOfVerses[2] = 15;
            bcv.NumberOfVerses[3] = 20;


            break;
        case 37:
            bcv.NumberOfChapters = 2;
            bcv.NumberOfVerses[1] = 15;
            bcv.NumberOfVerses[2] = 23;


            break;
        case 38:
            bcv.NumberOfChapters = 14;
            bcv.NumberOfVerses[1] = 21;
            bcv.NumberOfVerses[2] = 13;
            bcv.NumberOfVerses[3] = 10;
            bcv.NumberOfVerses[4] = 14;
            bcv.NumberOfVerses[5] = 11;
            bcv.NumberOfVerses[6] = 15;
            bcv.NumberOfVerses[7] = 14;
            bcv.NumberOfVerses[8] = 23;
            bcv.NumberOfVerses[9] = 17;
            bcv.NumberOfVerses[10] = 12;
            bcv.NumberOfVerses[11] = 17;
            bcv.NumberOfVerses[12] = 14;
            bcv.NumberOfVerses[13] = 9;
            bcv.NumberOfVerses[14] = 21;


            break;
        case 39:
            bcv.NumberOfChapters = 4;
            bcv.NumberOfVerses[1] = 14;
            bcv.NumberOfVerses[2] = 17;
            bcv.NumberOfVerses[3] = 18;
            bcv.NumberOfVerses[4] = 6;


            break;
        case 40:
            bcv.NumberOfChapters = 28;
            bcv.NumberOfVerses[1] = 25;
            bcv.NumberOfVerses[2] = 23;
            bcv.NumberOfVerses[3] = 17;
            bcv.NumberOfVerses[4] = 25;
            bcv.NumberOfVerses[5] = 48;
            bcv.NumberOfVerses[6] = 34;
            bcv.NumberOfVerses[7] = 29;
            bcv.NumberOfVerses[8] = 34;
            bcv.NumberOfVerses[9] = 38;
            bcv.NumberOfVerses[10] = 42;
            bcv.NumberOfVerses[11] = 30;
            bcv.NumberOfVerses[12] = 50;
            bcv.NumberOfVerses[13] = 58;
            bcv.NumberOfVerses[14] = 36;
            bcv.NumberOfVerses[15] = 39;
            bcv.NumberOfVerses[16] = 28;
            bcv.NumberOfVerses[17] = 27;
            bcv.NumberOfVerses[18] = 35;
            bcv.NumberOfVerses[19] = 30;
            bcv.NumberOfVerses[20] = 34;
            bcv.NumberOfVerses[21] = 46;
            bcv.NumberOfVerses[22] = 46;
            bcv.NumberOfVerses[23] = 39;
            bcv.NumberOfVerses[24] = 51;
            bcv.NumberOfVerses[25] = 46;
            bcv.NumberOfVerses[26] = 75;
            bcv.NumberOfVerses[27] = 66;
            bcv.NumberOfVerses[28] = 20;


            break;
        case 41:
            bcv.NumberOfChapters = 16;
            bcv.NumberOfVerses[1] = 45;
            bcv.NumberOfVerses[2] = 28;
            bcv.NumberOfVerses[3] = 35;
            bcv.NumberOfVerses[4] = 41;
            bcv.NumberOfVerses[5] = 43;
            bcv.NumberOfVerses[6] = 56;
            bcv.NumberOfVerses[7] = 37;
            bcv.NumberOfVerses[8] = 38;
            bcv.NumberOfVerses[9] = 50;
            bcv.NumberOfVerses[10] = 52;
            bcv.NumberOfVerses[11] = 33;
            bcv.NumberOfVerses[12] = 44;
            bcv.NumberOfVerses[13] = 37;
            bcv.NumberOfVerses[14] = 72;
            bcv.NumberOfVerses[15] = 47;
            bcv.NumberOfVerses[16] = 20;


            break;
        case 42:
            bcv.NumberOfChapters = 24;
            bcv.NumberOfVerses[1] = 80;
            bcv.NumberOfVerses[2] = 52;
            bcv.NumberOfVerses[3] = 38;
            bcv.NumberOfVerses[4] = 44;
            bcv.NumberOfVerses[5] = 39;
            bcv.NumberOfVerses[6] = 49;
            bcv.NumberOfVerses[7] = 50;
            bcv.NumberOfVerses[8] = 56;
            bcv.NumberOfVerses[9] = 62;
            bcv.NumberOfVerses[10] = 42;
            bcv.NumberOfVerses[11] = 54;
            bcv.NumberOfVerses[12] = 59;
            bcv.NumberOfVerses[13] = 35;
            bcv.NumberOfVerses[14] = 35;
            bcv.NumberOfVerses[15] = 32;
            bcv.NumberOfVerses[16] = 31;
            bcv.NumberOfVerses[17] = 37;
            bcv.NumberOfVerses[18] = 43;
            bcv.NumberOfVerses[19] = 48;
            bcv.NumberOfVerses[20] = 47;
            bcv.NumberOfVerses[21] = 38;
            bcv.NumberOfVerses[22] = 71;
            bcv.NumberOfVerses[23] = 56;
            bcv.NumberOfVerses[24] = 53;


            break;
        case 43:
            bcv.NumberOfChapters = 21;
            bcv.NumberOfVerses[1] = 51;
            bcv.NumberOfVerses[2] = 25;
            bcv.NumberOfVerses[3] = 36;
            bcv.NumberOfVerses[4] = 54;
            bcv.NumberOfVerses[5] = 47;
            bcv.NumberOfVerses[6] = 71;
            bcv.NumberOfVerses[7] = 53;
            bcv.NumberOfVerses[8] = 59;
            bcv.NumberOfVerses[9] = 41;
            bcv.NumberOfVerses[10] = 42;
            bcv.NumberOfVerses[11] = 57;
            bcv.NumberOfVerses[12] = 50;
            bcv.NumberOfVerses[13] = 38;
            bcv.NumberOfVerses[14] = 31;
            bcv.NumberOfVerses[15] = 27;
            bcv.NumberOfVerses[16] = 33;
            bcv.NumberOfVerses[17] = 26;
            bcv.NumberOfVerses[18] = 40;
            bcv.NumberOfVerses[19] = 42;
            bcv.NumberOfVerses[20] = 31;
            bcv.NumberOfVerses[21] = 25;


            break;
        case 44:
            bcv.NumberOfChapters = 28;
            bcv.NumberOfVerses[1] = 26;
            bcv.NumberOfVerses[2] = 47;
            bcv.NumberOfVerses[3] = 26;
            bcv.NumberOfVerses[4] = 37;
            bcv.NumberOfVerses[5] = 42;
            bcv.NumberOfVerses[6] = 15;
            bcv.NumberOfVerses[7] = 60;
            bcv.NumberOfVerses[8] = 40;
            bcv.NumberOfVerses[9] = 43;
            bcv.NumberOfVerses[10] = 48;
            bcv.NumberOfVerses[11] = 30;
            bcv.NumberOfVerses[12] = 25;
            bcv.NumberOfVerses[13] = 52;
            bcv.NumberOfVerses[14] = 28;
            bcv.NumberOfVerses[15] = 41;
            bcv.NumberOfVerses[16] = 40;
            bcv.NumberOfVerses[17] = 34;
            bcv.NumberOfVerses[18] = 28;
            bcv.NumberOfVerses[19] = 41;
            bcv.NumberOfVerses[20] = 38;
            bcv.NumberOfVerses[21] = 40;
            bcv.NumberOfVerses[22] = 30;
            bcv.NumberOfVerses[23] = 35;
            bcv.NumberOfVerses[24] = 27;
            bcv.NumberOfVerses[25] = 27;
            bcv.NumberOfVerses[26] = 32;
            bcv.NumberOfVerses[27] = 44;
            bcv.NumberOfVerses[28] = 31;


            break;
        case 45:
            bcv.NumberOfChapters = 16;
            bcv.NumberOfVerses[1] = 32;
            bcv.NumberOfVerses[2] = 29;
            bcv.NumberOfVerses[3] = 31;
            bcv.NumberOfVerses[4] = 25;
            bcv.NumberOfVerses[5] = 21;
            bcv.NumberOfVerses[6] = 23;
            bcv.NumberOfVerses[7] = 25;
            bcv.NumberOfVerses[8] = 39;
            bcv.NumberOfVerses[9] = 33;
            bcv.NumberOfVerses[10] = 21;
            bcv.NumberOfVerses[11] = 36;
            bcv.NumberOfVerses[12] = 21;
            bcv.NumberOfVerses[13] = 14;
            bcv.NumberOfVerses[14] = 23;
            bcv.NumberOfVerses[15] = 33;
            bcv.NumberOfVerses[16] = 27;


            break;
        case 46:
            bcv.NumberOfChapters = 16;
            bcv.NumberOfVerses[1] = 31;
            bcv.NumberOfVerses[2] = 16;
            bcv.NumberOfVerses[3] = 23;
            bcv.NumberOfVerses[4] = 21;
            bcv.NumberOfVerses[5] = 13;
            bcv.NumberOfVerses[6] = 20;
            bcv.NumberOfVerses[7] = 40;
            bcv.NumberOfVerses[8] = 13;
            bcv.NumberOfVerses[9] = 27;
            bcv.NumberOfVerses[10] = 33;
            bcv.NumberOfVerses[11] = 34;
            bcv.NumberOfVerses[12] = 31;
            bcv.NumberOfVerses[13] = 13;
            bcv.NumberOfVerses[14] = 40;
            bcv.NumberOfVerses[15] = 58;
            bcv.NumberOfVerses[16] = 24;


            break;
        case 47:
            bcv.NumberOfChapters = 13;
            bcv.NumberOfVerses[1] = 24;
            bcv.NumberOfVerses[2] = 17;
            bcv.NumberOfVerses[3] = 18;
            bcv.NumberOfVerses[4] = 18;
            bcv.NumberOfVerses[5] = 21;
            bcv.NumberOfVerses[6] = 18;
            bcv.NumberOfVerses[7] = 16;
            bcv.NumberOfVerses[8] = 24;
            bcv.NumberOfVerses[9] = 15;
            bcv.NumberOfVerses[10] = 18;
            bcv.NumberOfVerses[11] = 33;
            bcv.NumberOfVerses[12] = 21;
            bcv.NumberOfVerses[13] = 14;


            break;
        case 48:
            bcv.NumberOfChapters = 6;
            bcv.NumberOfVerses[1] = 24;
            bcv.NumberOfVerses[2] = 21;
            bcv.NumberOfVerses[3] = 29;
            bcv.NumberOfVerses[4] = 31;
            bcv.NumberOfVerses[5] = 26;
            bcv.NumberOfVerses[6] = 18;


            break;
        case 49:
            bcv.NumberOfChapters = 6;
            bcv.NumberOfVerses[1] = 23;
            bcv.NumberOfVerses[2] = 22;
            bcv.NumberOfVerses[3] = 21;
            bcv.NumberOfVerses[4] = 32;
            bcv.NumberOfVerses[5] = 33;
            bcv.NumberOfVerses[6] = 24;


            break;
        case 50:
            bcv.NumberOfChapters = 4;
            bcv.NumberOfVerses[1] = 30;
            bcv.NumberOfVerses[2] = 30;
            bcv.NumberOfVerses[3] = 21;
            bcv.NumberOfVerses[4] = 23;


            break;
        case 51:
            bcv.NumberOfChapters = 4;
            bcv.NumberOfVerses[1] = 29;
            bcv.NumberOfVerses[2] = 23;
            bcv.NumberOfVerses[3] = 25;
            bcv.NumberOfVerses[4] = 18;

            break;
        case 52:
            bcv.NumberOfChapters = 5;
            bcv.NumberOfVerses[1] = 10;
            bcv.NumberOfVerses[2] = 20;
            bcv.NumberOfVerses[3] = 13;
            bcv.NumberOfVerses[4] = 18;
            bcv.NumberOfVerses[5] = 28;


            break;
        case 53:
            bcv.NumberOfChapters = 3;
            bcv.NumberOfVerses[1] = 12;
            bcv.NumberOfVerses[2] = 17;
            bcv.NumberOfVerses[3] = 18;


            break;
        case 54:
            bcv.NumberOfChapters = 6;
            bcv.NumberOfVerses[1] = 20;
            bcv.NumberOfVerses[2] = 15;
            bcv.NumberOfVerses[3] = 16;
            bcv.NumberOfVerses[4] = 16;
            bcv.NumberOfVerses[5] = 25;
            bcv.NumberOfVerses[6] = 21;


            break;
        case 55:
            bcv.NumberOfChapters = 4;
            bcv.NumberOfVerses[1] = 18;
            bcv.NumberOfVerses[2] = 26;
            bcv.NumberOfVerses[3] = 17;
            bcv.NumberOfVerses[4] = 22;


            break;
        case 56:
            bcv.NumberOfChapters = 3;
            bcv.NumberOfVerses[1] = 16;
            bcv.NumberOfVerses[2] = 15;
            bcv.NumberOfVerses[3] = 15;


            break;
        case 57:
            bcv.NumberOfChapters = 1;
            bcv.NumberOfVerses[1] = 25;


            break;
        case 58:
            bcv.NumberOfChapters = 13;
            bcv.NumberOfVerses[1] = 14;
            bcv.NumberOfVerses[2] = 18;
            bcv.NumberOfVerses[3] = 19;
            bcv.NumberOfVerses[4] = 16;
            bcv.NumberOfVerses[5] = 14;
            bcv.NumberOfVerses[6] = 20;
            bcv.NumberOfVerses[7] = 28;
            bcv.NumberOfVerses[8] = 13;
            bcv.NumberOfVerses[9] = 28;
            bcv.NumberOfVerses[10] = 39;
            bcv.NumberOfVerses[11] = 40;
            bcv.NumberOfVerses[12] = 29;
            bcv.NumberOfVerses[13] = 25;


            break;
        case 59:
            bcv.NumberOfChapters = 5;
            bcv.NumberOfVerses[1] = 27;
            bcv.NumberOfVerses[2] = 26;
            bcv.NumberOfVerses[3] = 18;
            bcv.NumberOfVerses[4] = 17;
            bcv.NumberOfVerses[5] = 20;


            break;
        case 60:
            bcv.NumberOfChapters = 5;
            bcv.NumberOfVerses[1] = 25;
            bcv.NumberOfVerses[2] = 25;
            bcv.NumberOfVerses[3] = 22;
            bcv.NumberOfVerses[4] = 19;
            bcv.NumberOfVerses[5] = 14;


            break;
        case 61:
            bcv.NumberOfChapters = 3;
            bcv.NumberOfVerses[1] = 21;
            bcv.NumberOfVerses[2] = 22;
            bcv.NumberOfVerses[3] = 18;


            break;
        case 62:
            bcv.NumberOfChapters = 5;
            bcv.NumberOfVerses[1] = 10;
            bcv.NumberOfVerses[2] = 29;
            bcv.NumberOfVerses[3] = 24;
            bcv.NumberOfVerses[4] = 21;
            bcv.NumberOfVerses[5] = 21;


            break;
        case 63:
            bcv.NumberOfChapters = 1;
            bcv.NumberOfVerses[1] = 13;


            break;
        case 64:
            bcv.NumberOfChapters = 1;
            bcv.NumberOfVerses[1] = 14;


            break;
        case 65:
            bcv.NumberOfChapters = 1;
            bcv.NumberOfVerses[1] = 25;


            break;
        case 66:
            bcv.NumberOfChapters = 22;
            bcv.NumberOfVerses[1] = 20;
            bcv.NumberOfVerses[2] = 29;
            bcv.NumberOfVerses[3] = 22;
            bcv.NumberOfVerses[4] = 11;
            bcv.NumberOfVerses[5] = 14;
            bcv.NumberOfVerses[6] = 17;
            bcv.NumberOfVerses[7] = 17;
            bcv.NumberOfVerses[8] = 13;
            bcv.NumberOfVerses[9] = 21;
            bcv.NumberOfVerses[10] = 11;
            bcv.NumberOfVerses[11] = 19;
            bcv.NumberOfVerses[12] = 17;
            bcv.NumberOfVerses[13] = 18;
            bcv.NumberOfVerses[14] = 20;
            bcv.NumberOfVerses[15] = 8;
            bcv.NumberOfVerses[16] = 21;
            bcv.NumberOfVerses[17] = 18;
            bcv.NumberOfVerses[18] = 24;
            bcv.NumberOfVerses[19] = 21;
            bcv.NumberOfVerses[20] = 15;
            bcv.NumberOfVerses[21] = 27;
            bcv.NumberOfVerses[22] = 21;

            break;
    }


    //return bcv;
}

QString bsnames[67] =
{ (QString)"",(QString)bs01,(QString)bs02,(QString)bs03,(QString)bs04,(QString)bs05,(QString)bs06,(QString)bs07,(QString)bs08,(QString)bs09,
    bs10,(QString)bs11,(QString)bs12,(QString)bs13,(QString)bs14,(QString)bs15,(QString)bs16,(QString)bs17,(QString)bs18,(QString)bs19,
    bs20,(QString)bs21,(QString)bs22,(QString)bs23,(QString)bs24,(QString)bs25,(QString)bs26,(QString)bs27,(QString)bs28,(QString)bs29,
    bs30,(QString)bs31,(QString)bs32,(QString)bs33,(QString)bs34,(QString)bs35,(QString)bs36,(QString)bs37,(QString)bs38,(QString)bs39,
    bs40,(QString)bs41,(QString)bs42,(QString)bs43,(QString)bs44,(QString)bs45,(QString)bs46,(QString)bs47,(QString)bs48,(QString)bs49,
    bs50,(QString)bs51,(QString)bs52,(QString)bs53,(QString)bs54,(QString)bs55,(QString)bs56,(QString)bs57,(QString)bs58,(QString)bs59,
    bs60,(QString)bs61,(QString)bs62,(QString)bs63,(QString)bs64,(QString)bs65,(QString)bs66
};
QString vsnames[67] ;// filled in by the following function
BibleBookInfoStructType getBibleBookInfoByName(QString bookName)
{
    BOOL foundNumber=NO;
  static  BibleBookInfoStructType bis;// As BibleBookInfoStruct
    int index;// As integer
    QString sbook;// As String

   //memset(&bis, 0, sizeof(bis));

    bis.BookName= "Not Found";//,40);

    bis.BookNumber =-1;

    if (!vsnames[1].length()) {
        //myStrnCpy(bsnames[0], "", 4);
       // bsnames
        for(index=1; index < 67; index++){
            BibleBookInfoStructType bis = getBibleBookInfo(index);
            vsnames[index]= bis.BookVSName;//, 4 );// fill in the very short name

        }
    }

    bookName = bookName.toLower();


    bookName = myReplace(bookName, " ","");// remove spaces
    bookName = myReplace(bookName,"%20","");// remove html spaces

    if (Instr(bookName,"1")> 0 || Instr(bookName,"2") > 0 || Instr(bookName,"3")){ //; then
        sbook = Left(bookName, 4);
        foundNumber=YES;
    } else{
        sbook = Left(bookName,3);
    }

    // handle exception here. jud(ges) and jud(e) is same, knock out immediately
    if (StringsAreEqual(bookName, "jude")){//Then
        return getBibleBookInfo(65);
    }

    if (StringsAreEqual(bookName, "song")){//Then
        return getBibleBookInfo(22);
    }


    if (StringsAreEqual(bookName , "philemon") || StringsAreEqual(bookName, "phm")){// Then
        return getBibleBookInfo(57);
    }//end if

    if (StringsAreEqual(bookName , "philippians") || StringsAreEqual(bookName, "php") || StringsAreEqual(bookName, "phi")){// Then
        return getBibleBookInfo(50);
    }//end if


    if (StringsAreEqual(bookName,  "songofsongs") || StringsAreEqual(bookName , "son")){// then
        return getBibleBookInfo(22); // return song of songs
    }//end if

    QChar c;
    if (sbook.length()==0)
            return getBibleBookInfo(1);

     c= sbook.at(0);

        if (sbook.length()==2 || (foundNumber && sbook.length()==3) ) {
            for ( index = 1; index <= Preferences->NumberOfBooksInBible; index++) {
                if (c == bsnames[index][0]) {
                    if (StringsAreEqual(sbook ,  bsnames[index] )){// then
                        return getBibleBookInfo(index);
                    }
                }// end if
            }//next
        }

        if (sbook.length()<=4){
            for ( index = 1; index <= Preferences->NumberOfBooksInBible; index++) {
                if (c == bsnames[index][0]) {
                    if (StringsAreEqual(sbook ,  bsnames[index] )){// then
                        return getBibleBookInfo(index);
                    }
                }// end if
            }//next
        }



    for (index = 1 ; index <= Preferences->NumberOfBooksInBible; index++){
        bis = getBibleBookInfo(index);
        if (c==bis.BookSName[0] ) {
            if ( StringsAreEqual(sbook ,  bis.BookSName )){// Then
                return getBibleBookInfo(index); // bail, we are done
            }//end if
        }
    }//next

    for ( index = 1; index <= Preferences->NumberOfBooksInBible; index++) {
        if (c == vsnames[index][0]) {
            if (StringsAreEqual(sbook ,  bsnames[index] )){// then
                return getBibleBookInfo(index);
            }
        }// end if
    }//next


    if (index == 67) {
        bis.BookNumber = -1;
    }
    return bis;

}

