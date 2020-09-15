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
//  BookManipulation.m
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 6/22/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//CONVERTED

#include "bookmanipulationclass.h"
#include "bibleinfo.h"
#include "misclibrary.h"
#include "structs.h"
#include "bookdbclass.h"
#include "HTMLModule.h"
#include "bookmanipulationlibrary.h"
#include "preferencesmodule.h"
#include "WebDefinitionModule.h"
#include "PreferencesLibrary.h"
#include "bookpreferencesdbclass.h"
#include "searchlibrary.h"

#include <QString>
#include <QList>
#include <QStringList>

extern SearchInfoStructType globalSIT;


BookManipulationClass::BookManipulationClass()
{
    NumberOfBooks = 1;
    DoMetaTag = NO;
    DontShowLines = YES;
    OneRow = NO;
}

QString BookManipulationClass::getTextBookNameForNumber(int BookNumber){
    QString sl="No Such Book";
    BookHandlerClass *bk=nullptr;
    BookDBClass *BookDB=nullptr;

    if (UBound(gTextBooks) >= BookNumber ) {
        if (gTextBooks [BookNumber]!= Nil) { //then
            bk = gTextBooks [BookNumber];
            if (bk)
                BookDB = (BookDBClass*)bk->Database;
            if (BookDB)
                sl =BookDB->getTitle();
            }
    }

    return sl;
}

#if 1
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BibleBookInfoStructType BookManipulationClass::GetTextBookInfoForBook( int BookNumber, /*andChapter*/ int ChapterNumber)
{
    static BibleBookInfoStructType bbi;//dim bbi As BibleBookInfoStructType
    BookHandlerClass *bk;
    BookDBClass *BookDB;

    bbi.NumVerses = 0;
    bbi.NumChapters=0;
    bbi.BookName="Genesis";
    bbi.BookNumber =1;
    bbi.BookSName="Gen";
    bbi.BookVSName = "Ge";
    bbi.Language="H";
    bbi.TableName="Genesis";
    bbi.Testament="OT";

    if (BookNumber < 0)  //then
        goto skip1;

    if (UBound(gTextBooks) >= BookNumber ) { //then

        if (gTextBooks [BookNumber]!= Nil) { //then
            bk = gTextBooks [BookNumber];
            BookDB = (BookDBClass*)bk->Database;

             bbi.BookName =BookDB->getTitle();

            bbi.NumChapters =  BookDB->GetNumChapters();
            bbi.Title=BookDB->getTitle();//.toUtf8() ,sizeof(bbi.Title));
            bbi.NumVerses = BookDB->GetNumParagraphs(ChapterNumber);
        } // end if
    } // end if

skip1:
    bbi.BookNumber = BookNumber;
    bbi.TableName = BOOK_TEXT_TABLE;

    return bbi;
}
#endif

#if 1
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BookManipulationClass:: buildTextPageForBook( int BookIndex, /*andChapter*/ int ChapterNumber, /* shouldScrollToVerse*/ BOOL bScrollToVerse, /* withVerseToScrollTo*/ int iVerseToScrollTo, /*andHiliteSearchTerm*/ BOOL HilightSearch,/* forSearchString*/ QString searchString)
{
    //----------------------------------------------------------
    // BookIndex --> The number of the book to show the chapter from
    // ChapterNumber --> the chapter to show
    //----------------------------------------------------------


    BookHandlerClass *bk;
    BookDBClass *bkDB;

    DoMetaTag = bScrollToVerse;
    MTag = iVerseToScrollTo;

    if (gTextBooks.count()==  0)  { //Then
        MsgBox ( "There are no books to view.");
        return "";
    } // end if

    static int Verse; //static Verse As int16
    QString versetxt;//    dim versetxt As String
    static int index;//  static index As Integer
    static int DisplayBook;// As Integer

    QStringList viewpage;// = [NSMutableArray arrayWithCapacity:20];    // dim viewpage(-1) As String
    static BibleBookInfoStructType BookInfo;// As BibleBookInfoStruct
    static int limit;// As int16




    QString ts=   "<html><head> <title></title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"> <!DOCTYPE HTML PUBLIC \"//W3C//DTD HTML 4.01 Transitional//EN\"http://www.w3.org/TR/html4/loose.dtd\">\n";

    viewpage.append(ts);

    static QString Anchor;// As String
    static QString DoMTString;// As String


    if (DoMetaTag)  { //then
        Anchor =  JavaBookMark;
    } // end if


    viewpage.append( javascriptScrollOffset);


    viewpage.append(  "</head>");

    //------------------------------
    // turn off underlines on all links
    //------------------------------
    viewpage.append(  HTML_NO_UNDERLINE_STYLE);



    if ((DoMetaTag))  { //then
        DoMTString =  "<body onload=" QUOTE "jumpScroll('"+Anchor+"')" QUOTE "  link=" QUOTE +Preferences->LinkColor+ QUOTE ">";
        DoMTString = myReplace(DoMTString,"&h","#");
        viewpage.append( DoMTString);
    }  else {
        DoMTString =  "<body  link=" QUOTE+ Preferences->LinkColor+ QUOTE ">";
        DoMTString = myReplace(DoMTString,"&h","#");
        viewpage.append(DoMTString);

    } // end if

    int fontSize =Preferences->getFontSize();
    ts = QString::number(fontSize)+"px";

    viewpage.append("<style type=\"text/css\">body {font-size:"+QString::number(fontSize)+"px;}</style>\n");

    //------------------------------
    //   build the table header
    //------------------------------
    // change the zz% to 50% or whatever to force the table width
    versetxt = GetWidthPercentage(NumberOfBooks);

    Anchor = myReplace(gTextBookPreferences->BookPreferredTableStyle(),"zz",versetxt);
    //[NSString stringWithFormat:@"%dpt", Preferences.FontSize];
    Anchor = myReplace(Anchor,"ffpt",ts);
    Anchor = myReplace(Anchor,"btm%","15"); // give 15 pixel padding to bottom of cell
    viewpage.append( Anchor);
    //------------------------------
    //------------------------------




    viewpage.append( "<table class=" QUOTE "sample" QUOTE "/>");

    //-------------------------------------------------------------------------------
    // for now only build one book
    //-------------------------------------------------------------------------------
    limit =BookIndex;

    //-------------------------------------------------------------------------------
    // Handle footnote indicator
    //-------------------------------------------------------------------------------
    for (index =BookIndex; index <= limit; index++) {
        bk = gTextBooks [index];
        bkDB =(BookDBClass*) bk->Database;

        if (!bkDB->isOpen) {
            bkDB->Open("F");
        }
        BookInfo = GetTextBookInfoForBook(BookIndex ,ChapterNumber);


        DisplayBook = index;
        if ((DisplayBook >= 0 ))  { //then
            switch (bkDB->getDbType()) {
                case BOOK_DATABASE_SUFFIX_TYPE:
                    bkDB->FootNote =0;
                    break;

                case NOTES_DATABASE_SUFFIX_TYPE:
                    bkDB->FootNote=0;
                    break;
                default:
                    break;
            }
        } // end if

    }//next


    //-------------------------------------------------------------------------------
    // go build the verses
    //-------------------------------------------------------------------------------
//DoBuildTheVerses:
    Verse = 1;

    SHOW_THIS_INFO_TYPE sti;
    SEARCH_INFO_TYPE sit;
    // DoHideNotes = sti.DoHideNotes;

    sti = globalSTI;
    sti.ShowStrongs = NO;
    sti.EnableMultimediaTag = Preferences->getSpeechEnabled();
    //sti.enableEnglishDictionary=globalSTI.enableEnglishDictionary;
    sit.BookNumber=BookIndex;
    sit.ChapterNumber=ChapterNumber;
    if (HilightSearch) {
        sit.WordToSearchFor=searchString;//.toUtf8(),sizeof(sit.WordToSearchFor));
        sit.PhraseSearch = NO;
        sit.SearchForAWord = YES;
    }

    do {
        sit.VerseNumber = Verse;


        // MARK THE ROW FOR THE PARAGRAPH TO PARAGRAPH TRANSITIONS
        viewpage.append(  "\n" HTML_ROW_START HTML_TABLE_CELL_START  );
        viewpage.append("<A id=\"f"+QString::number(Verse)+"\"></a>" HTML_TABLE_CELL_END);
        viewpage.append(   HTML_ROW_END );


       //-------------------------------------
        // go through the selected book
        //--------------------------------------
        if (! OneRow)
            viewpage.append(  "\n" HTML_ROW_START);

        //============================
        for (index =BookIndex ;index <= limit; index++) {
            DisplayBook = index;
            bk = gTextBooks [index];
            bkDB = (BookDBClass*)bk->Database;

            if ((DisplayBook < 0  || bk->Database == Nil ))  { //then
                continue;
            } // end if

            switch (bkDB->getDbType()) {
                case BOOK_DATABASE_SUFFIX_TYPE:
                    versetxt = bkDB->GetParagraph(&sti,/* SearchParmsStructPointer*/&sit);
                    break;

                case NOTES_DATABASE_SUFFIX_TYPE:
                    versetxt = bkDB->GetPlainVerse(&sti,&sit,TYPE_BBL_TYPE);
                    break;

                default:
                    break;
            }

            //-------------------------------------------------------------------------------
            // if (we should, mark the verse to scroll to
            //-------------------------------------------------------------------------------
            if (DoMetaTag)  { //then
                              //if ( index == 0)  { //then
                    if (Verse == MTag)  { //then
                        versetxt =  "<A NAME=" QUOTE JavaBookMark QUOTE "></a>"+versetxt;
                    } // end if
                      // } // end if
            } // end if



            if (Instr(versetxt,ONE_ROW_START)>0)  { //then
                OneRow = YES;
                viewpage.append(HTML_TABLE_CELL_START);
                versetxt = myReplace(versetxt, ONE_ROW_START,"");
            } // end if


            if (OneRow)  { //then
                           // make table have a bottom border
                versetxt = myReplace(versetxt,"border-bottom:none", "border-bottom:solid");

                if (sti.DoHideNotes) {
                    if (ChapterNumber > 0)  { //then
                        ts = CreateBookMarkTag(DisplayBook,ChapterNumber,Verse,versetxt,DoHideNotes,DisplayBook,index,OneRow);
                        viewpage.append(ts);
                    }
                } else {
                    viewpage.append(versetxt);
                } // end if

                if (Instr(versetxt, ONE_ROW_END)>0)  { //then
                    OneRow = NO;
                    versetxt = myReplace(versetxt, ONE_ROW_END,"");
                    viewpage.append(versetxt);
                    viewpage.append(HTML_TABLE_CELL_END);
                    viewpage.append( Anchor);
                } // end if
            } else {

                if (ChapterNumber > 0)  { //then
                    viewpage.append(HTML_TABLE_CELL_START);
                    ts=CreateBookMarkTag(DisplayBook,ChapterNumber,Verse,versetxt,sti.DoHideTagsForPrinting,DisplayBook,index,OneRow);
                    viewpage.append(ts);
                    viewpage.append( HTML_TABLE_CELL_END);
                }else {
                    viewpage.append(HTML_TABLE_CELL_START);
                    viewpage.append(versetxt);
                    viewpage.append( HTML_TABLE_CELL_END);
                } // end if
            } // end if
        }// next
         //============================
        if (! OneRow)   //then viewpage .append( HTML_ROW_END
            viewpage.append( HTML_ROW_END );

        // if (this is the last verse,)  { //then exit the for loop
        if (BookInfo.NumVerses <= Verse)  { //then
            goto FinishedBuildingVerses;
        } // end if

        Verse++;
    } while(Verse <= BookInfo.NumVerses );
    //-------------------------------------------------------------------------------

    //---------------------------
FinishedBuildingVerses:

    versetxt = viewpage.join("");

    versetxt =versetxt + "</table>" "</body></html>" ;

    return versetxt;
}
#endif

QString BookManipulationClass:: getFootNoteForBook( int bookIndex, /*andChapter*/ int chapterNumber, /*andVerse*/ int Verse, /*andFootNoteNumber*/ int footNote )
{
    QString str;

    // '----------------------------------------------------
    //' get a booknote from the database and return it as a string
    //'----------------------------------------------------
    RecordSet *rs;// As RecordSet
    QString  Query;// As String
    QString  FootNoteText;// As String

    BookHandlerClass *bk;
    BookDBClass *bkDB;
    BOOL StripFormatting = NO;

    // this must be a "modern" database
    Query = str.sprintf("select " WORD_COLUMN " from "  FOOTNOTE_TABLENAME " where " CHAPTER_COLUMN "=%d and " VERSE_COLUMN "=%d and " WORD_SEQ_COLUMN "=%d limit 1",chapterNumber,Verse, footNote);

    if (gTextBooks.size()==0)
        return "no footnote available";// avoid crash

    bk = gTextBooks [bookIndex];
    bkDB =(BookDBClass*) bk->Database;

    if (!bkDB->isOpen) {
        bkDB->Open(bkDB->databaseName);
    }



    rs = bkDB->SQLSelect(Query);

    if (rs != Nil ) {// then

    //----------------------------------------------------------------
    // IF LINUX, then font style does not work
    //add <font> tag into the  text
    //------------------------------------------------------------------
        FootNoteText = rs->IdxField(1);//.StringValue)
        if (StripFormatting ){//Then
            return FootNoteText;
        }  else {
    // do it with style
            FootNoteText =str.sprintf( "<p style=" QUOTE "font-size:%dpt" QUOTE HTML_GENERIC_END_TAG "%ls</p>" HTML_NO_UNDERLINE_STYLE,Preferences->getFontSize()/2,(wchar_t*)FootNoteText.utf16());


            return  FootNoteText;
        }//    end if

    }//    end if


    return "no footnote available";
}

