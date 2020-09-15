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
//  BibleModule.m
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/23/2010.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//converted 2019-2020
#include <QString>
#include "biblemodule.h"
#include "biblemaxvalues.h"

#include "../shared/bibleinfo.h"
#include "../shared/HTMLModule.h"
#include "../shared/WebDefinitionModule.h"
#include "notesdbclass.h"
#include "searchlibrary.h"
#include "BookMarkDB.h"
#include "helpLibrary/helplibrary.h"

#include <QStringList>


extern PreferencesModuleClass       *Preferences;
extern QList<BibleDatabaseClass *>Bibles;// the bibles we have found
extern QList<BookHandlerClass *>Books;// the bibles, commentaries, and books

QString CurrentWebPage;           // global for the web page


extern QString        BuildPageForBookChapterVerse(BookInfoStructType *BooksToUse, int Book, int Chapter, int Verse, BibleBookInfoStructType *bis) ;
QString BuildVersePage(BookInfoStructType *BooksToUse,// array of indexes
                            BookChapterVerseStructType *bcv, /* number of chapters in this book, and number of verses in the chapter*/
                            // int NumBooks, /* number of books to show*/
                            SHOW_THIS_INFO_TYPE *ShowThisInfo,
                            SEARCH_INFO_TYPE *sit // also includes book, chapter, verse number
                            ); /* HiliteSearch, DoMetaTag, etc*/
void GetBookChapterVerse(int BookNumber, BookChapterVerseStructType& bcv);

static QString str;
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
QString BuildPageForBookChapterVerse(BookInfoStructType *BooksToUse, int Book, int Chapter, int Verse, BibleBookInfoStructType *bis)
{
    // BIBLE_BCV_TYPE bcv;
    SHOW_THIS_INFO_TYPE sti;
    SEARCH_INFO_TYPE sit;
    BookChapterVerseStructType bcv;

    sti = globalSTI;

    if (globalSIT.PhraseSearch) {
        sit.PhraseSearch = globalSIT.PhraseSearch;
        sit.PhraseToSearchFor= globalSIT.PhraseToSearchFor;//.toUtf8(),sizeof(globalSIT.PhraseToSearchFor));
    }else {
        sit.SearchForAWord = globalSIT.SearchForAWord;
        sit.WordToSearchFor=globalSIT.WordToSearchFor;//.toUtf8(),sizeof(globalSIT.WordToSearchFor));
    }



    sti.NumberOfBooksBeingShownOnPage = Preferences->getNumberOfSelectedBooks()+Preferences->getNumberOfSelectedNotes()+Preferences->numberOfSelectedCommentaries;
    //globalSTI.DoScrollToVerse=NO; // only do this once

    sit.BookNumber=Book;
    sit.ChapterNumber=Chapter;
    sit.VerseNumber = Verse;


        GetBookChapterVerse(sit.BookNumber,bcv);


    sit.BookTableName =bis->TableName;
    sit.NumberOfBooksInBible = 66;

    return BuildVersePage(BooksToUse,&bcv, &sti, &sit);

}





extern QString HandleVerseTag(QString bmt,QString versetext, int Type,SHOW_THIS_INFO_TYPE *ShowThisInfo);

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
QString BuildVersePage(BookInfoStructType *BooksToUse,// array of indexes
                         BookChapterVerseStructType *bcv, /* number of chapters in this book, and number of verses in the chapter*/
                         // int NumBooks, /* number of books to show*/
                         SHOW_THIS_INFO_TYPE *ShowThisInfo,
                         SEARCH_INFO_TYPE *sit // also includes book, chapter, verse number
                         ) /* HiliteSearch, DoMetaTag, etc*/
{
    QString Anchor;// for the scroll to verse feature

    static BibleBookInfoStructType BibleInfo;

    QString bmt="";
    BookHandlerClass *bd=nil;
    BibleDatabaseClass *bbld=nil;

    static int      DisplayBible;
    QString DoMTString;// As String

    static int      index;
    int             index2;

    static int      lb;
    static int       limit2;
    int             NumBooks = ShowThisInfo->NumberOfBooksBeingShownOnPage;


    QString tempString;
    QString stitle;                // short title being shown on stacked mode
    QString versetxt;

    // make porting easier with these  dereferences
    int BookNumber = sit->BookNumber;
    int Chapter = sit->ChapterNumber;// ->ChapterNumber;
    BOOL DoMetaTag = ShowThisInfo->DoScrollToVerse;
    BOOL EnableHTMLHeader = ShowThisInfo->EnableHTMLHeader; // if true we are printing
    int MTag = ShowThisInfo->VerseToScrollTo;
    BookInfoStructType *BTU;

    ShowThisInfo->CompletedRun = YES;
    //uint64_t myCurrentTime = mach_absolute_time();// start the clock

    if (Books.count() == 0 || Chapter<1 || NumBooks < 1 || BookNumber > sit->NumberOfBooksInBible) {
        return EMPTY_PAGE_STRING;
    }

    ShowThisInfo->NumberOfBooksBeingShownOnPage--; // subtracting here prevents further math in some subroutines.  This is done once, the subroutines are called 50 to 100 times.

     QStringList VSTitles; // up to 20 short titles.  more than will fit on a screen

     for (int i =0; i < 20; i++)
         VSTitles.append("");

    QStringList Xref;

    QStringList viewpage;
    sit->VerseNumber =1; // reset this so we build the whole page
    ShowThisInfo->IDNumber=1; // start at item number 1  used for calculation of where strongs number should be

    //----------------------------------------------------------
    // BookNumber --> The number of the book to show the chapter from
    // Chapter --> the chapter to show
    // BooksToUse() --> the books to pull hte chapter from [ SAME AS Preferences->SelectedBooks [] ]
    // NumBooks --> THe number of books to show
    // HilightSearch --> A search was done, so hilight the searched word
    // Searched --> THe Searched for word
    // DoMetaTag --> use JavasScript to scroll to a position on the page
    // MTag --> the verse to scroll to
    // DoHideNotes --> When true, notes are hidden inside the verse number, false they are a clickable link
    // ShowThisInfo->IDNumber is used to tag each strong's number for location information
    //----------------------------------------------------------


    //viewpage = new StringHandleMBS

    if (lb != BookNumber ){// then
        extern BibleBookInfoStructType getBibleBookInfo(int BookNumber);

        BibleInfo = getBibleBookInfo(BookNumber);
        lb = BookNumber;
    }

    tempString = "<html><head> <title></title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"" \
    "> <!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"http://www.w3.org/TR/html4/loose.dtd\">";

    viewpage.append(tempString);


    if (DoMetaTag){// then
        Anchor =  JavaBookMark;
    }


    viewpage.append(javascriptScrollOffset);// scrollto and find object offset



#if ENABLE_MENUS
    // the following create a drop down menu.
    // as of 9.2009 is fixed. make custom later
    viewpage.append(CUSTOM_MENU_STYLE);
    viewpage.append(CUSTOM_MENU_TEXT);
    viewpage.append(CUSTOM_MENU_JS );
#endif


    viewpage.append("</head>");

    //------------------------------
    // turn off underlines on all links
    //------------------------------
    viewpage.append(  HTML_NO_UNDERLINE_STYLE );

#if 0
    // test code
    viewpage.append(  "<br>");
    viewpage.append(  CUSTOM_MENU_LINK_AUTO);

#endif

#if 1 // onload does not work on ipad/iphone by itself....HOWEVER, if you don't have the onload here, running the window.location.hash='bookmark'; later won't work.
    // so, you have to have the onload here, and then run the javascript after the document has loaded.  Maybe it triggers this???
    if (DoMetaTag) {//then
        DoMTString = "<body onload=\"jumpScroll('" + Anchor + str.sprintf("');\"  link=\"#%02x%02x%02x\">" ,Preferences->red,Preferences->green,Preferences->blue);
    }
    else
#endif
    {
        DoMTString = str.sprintf("<body link=\"#%02x%02x%02x\">",Preferences->red,Preferences->green,Preferences->blue);// style="+QUOTE+"font-size:45px;"+QUOTE+">"
    }

    myReplace(DoMTString,"&h","#");
   // DoMTString = [DoMTString stringByReplacingOccurrencesOfString:"&h" withString:"#");// myReplace(DoMTString,"&h","#")
    viewpage.append( DoMTString);

    //    viewpage.append( posn); // add the mouse position object
    //------------------------------
    // build the book/chapter header info
    //-------------------------------


    //------------------------------
    //   build the table header
    //------------------------------
    // change the zz% to 50% or whatever to force the table width
    versetxt = GetWidthPercentage(NumBooks);

    Anchor = Preferences->PreferredTableStyle;
    myReplace(Anchor,"zz",versetxt);
    QString lstr =QString::number(Preferences->getFontSize());
    myReplace(Anchor,"ff",lstr);
    myReplace(Anchor,"btm%","13");
    myReplace(Anchor,"%leftpad%","1");

    viewpage.append(Anchor);
    //------------------------------
    //------------------------------
    viewpage.append("<table class=\"sample\"/>" );


    //-------------------------------------------------------------------------------
    // make EnableHTMLHeader = True if we are showing web page for print or copy
    // page with the table header being the books
    //-------------------------------------------------------------------------------
    //const EnableHTMLHeader = False
    EnableHTMLHeader = NO;
    //if (EnableHTMLHeader)
    {// then
        QString Hdr;// As String;

        //-------------------------------------------------------------------------------
        // build the Copyright header and the name of the book
        // if this is the top-to-bottom stacking, then create vstitle.
        //-------------------------------------------------------------------------------
        if (ShowThisInfo->doStackVerses) {
            viewpage.append("<th style=" QUOTE "font-size:"+QString::number(Preferences->getFontSize()*2)+"px;" QUOTE ">" );
        }

        for (index =0 ; index < NumBooks; index++){
#define BACKGROUND_COLOR "#FFFFFF"
#define FOREGROUND_COLOR "#660000"



            BTU=&BooksToUse[index];

            DisplayBible =BTU->Index;// this is an index into the SelectedBooks [] array

            if (DisplayBible <0 || Books.count()==0) {
                continue; // skip this one
            }

            if (DisplayBible>=0) {
                QString titleString;
                static unsigned short copyrightID;
                // TITLE STRINGS
                switch(BTU->Type){
                    case TYPE_ABP_TYPE:
                        viewpage.append("<!--ABP_TYPE-->");
#if FALLTHROUGH_ALLOWED == 1
  [[fallthrough]];
#endif
                    case TYPE_BBL_TYPE:
                    case TYPE_MORPHOLOGY_TYPE:
                    {

                        bd = Books [DisplayBible];
                        bbld =(BibleDatabaseClass *)( bd->Database);
                        bbld->BibleIndexNumber=index; // index into the selected books array for the FootNote section

                        // the type is the second entry, so we can know what array to look up the copyright info from
                        // this is kludgy, and will probably be changed in the next version.
                        titleString = bbld->getTitle()+HTML_HREF_END;
                        //titleString = "<a href=http://\"I_0_0_0\">"+titleString;<-- this works for now
                        titleString = str.sprintf( "<a id='c%u' href=\"http://" COPYRIGHT_INDICATOR_SHOW_WINDOW "_%d_%d_c%u" QUOTE ">"
                                                   ,copyrightID, bbld->BibleIndexNumber, BooksToUse[index].Type,copyrightID ) + titleString;


                        if (ShowThisInfo->doStackVerses) {
                            // stack verses
                            Hdr = titleString + HTML_BRStr ;

                            stitle = bbld->getSTitle();
                            if (stitle.length()) {
                                VSTitles[index]=stitle;
                            } else {

                                VSTitles[index]="";// make a zero string
                                if (bbld->getTitle().length()==0) {
                                    goto noTitle;
                                }
                                QStringList ta = mySplit(bbld->getTitle()," ");
                                if (ta.count()) {
                                    VSTitles[index]="";
                                    for (index2 = 0 ;index2 < ta.count(); index2++)
                                        VSTitles[index]= VSTitles[index]+ta[index2].left(1).toUpper();
                                    }

                            }
                        noTitle:
                            str = str.sprintf( HTML_NBSP "<a id='c%u' href=\"http://"  COPYRIGHT_INDICATOR_SHOW_WINDOW "_%d_%d_c%u" QUOTE "><FONT style='bold' color='" FOREGROUND_COLOR "' style='BACKGROUND-COLOR:" BACKGROUND_COLOR "'>"
                                               , copyrightID ,   bbld->BibleIndexNumber, BooksToUse[index].Type,copyrightID);
                            str = str +  VSTitles[index]+"</FONT>"  HTML_HREF_END;
                            Xref.append(str);

                        }else {
                            Hdr = "<th style=" QUOTE "font-size:"+QString::number(Preferences->getFontSize()*2)+"px;" QUOTE ">"  ;
                            Hdr = Hdr + titleString + HTML_TABLE_HEADER_END;
                        }

                        copyrightID++;

                        if (!ShowThisInfo->doStackVerses) {
                            viewpage.append(Hdr);
                        }
                        bbld->ResetFootNote();// reset the footnote index counter for all the bibles
                    }
                        break;

                    case TYPE_COMMENTARY_TYPE:
                    {//
                        int bIndex=BTU->Index;
                        BookHandlerClass *bk;

                        bk = Books[BTU->Index];
                        bbld =(BibleDatabaseClass *) bk->Database;

                        titleString = str.sprintf( "<a id='c%u' href=\"http://" COPYRIGHT_INDICATOR_SHOW_WINDOW "_%d_%d_c%u" QUOTE ">",copyrightID, bIndex, BTU->Type,copyrightID);
                        titleString = titleString +  bbld->getTitle()+  HTML_HREF_END;
//                        titleString = str.sprintf( HTML_HREF_STARTQ "I_%d_%d" QUOTE ">%ls"  HTML_HREF_END, bIndex,TYPE_COMMENTARY_TYPE, bbld->getTitle());

                        if (ShowThisInfo->doStackVerses) {
                            // stack verses
                            Hdr = titleString + HTML_BRStr ;


                            QString stitle;

                            stitle = bbld->getSTitle();

                            if (stitle.length()) {
                                VSTitles[index]=stitle;//.toUtf8() ,4);
                            } else {
                                VSTitles[index]=bbld->getTitle();//.toUtf8() ,3);
                            }

                            str = str.sprintf( HTML_NBSP "<a id='c%u' href=\"http://"  COPYRIGHT_INDICATOR_SHOW_WINDOW "_%d_%d_c%u" QUOTE "><FONT style='bold' color='" FOREGROUND_COLOR "' style='BACKGROUND-COLOR:" BACKGROUND_COLOR "'>" HTML_HREF_END, copyrightID ,   bIndex, BTU->Type,copyrightID);
                            str = str + VSTitles[index]+"</FONT>";

                            Xref.append(str);

                        }else {
                            Hdr =str.sprintf( "<th style=" QUOTE "font-size:%dpx;" QUOTE ">",Preferences->getFontSize()*2);
                            Hdr = Hdr + titleString+ HTML_TABLE_HEADER_END;
                        }

                        copyrightID++;
                        if (!ShowThisInfo->doStackVerses) {
                            viewpage.append(Hdr);
                        }
                        bbld->ResetFootNote();// reset the footnote index counter for all the bibles
                    }
                        break;

                    case TYPE_NOTES_TYPE:
                    {//
                        extern NotesDBClass *gNotes[MAX_NUM_BIBLES];
                        int bIndex=BTU->Index;

                        bbld = gNotes[bIndex];

                        if (ShowThisInfo->doStackVerses) {
                            // stack verses
                            Hdr = bbld->getTitle() + HTML_BRStr ;
                            VSTitles[index]=bbld->getTitle();//.toUtf8(),4);


                           str= HTML_NBSP "<a id='c"+QString::number(copyrightID)+"' href=\"http://"  COPYRIGHT_INDICATOR_SHOW_WINDOW "_"+QString::number(bIndex)+"_"+QString::number( BTU->Type)+"_c"+QString::number(copyrightID)+ QUOTE "><FONT style='bold' color='" FOREGROUND_COLOR "' style='BACKGROUND-COLOR:" BACKGROUND_COLOR "'>"+VSTitles[index]+"</FONT>"  HTML_HREF_END;//                            [Xref.append(str.sprintf( HTML_NBSP "<FONT style='bold' color='"FOREGROUND_COLOR"' style='BACKGROUND-COLOR:"BACKGROUND_COLOR"'>%s</FONT>"  ,  VSTitles[index]]);
                            Xref.append(str);

                        }else {
                            titleString =  "<a id='c"+QString::number(copyrightID)+"' href=\"http://" COPYRIGHT_INDICATOR_SHOW_WINDOW "_"+QString::number(bIndex)+"_"+QString::number(BTU->Type)+"_c"+QString::number(copyrightID)+ QUOTE ">" ;
                            titleString = titleString +bbld->getTitle()+  HTML_HREF_END;

                            Hdr = "<th style=" QUOTE "font-size:"+QString::number(Preferences->getFontSize()*2)+"px;" QUOTE ">";
                            Hdr = Hdr + titleString+ HTML_TABLE_HEADER_END;
                        }

                        copyrightID++;

                        if (!ShowThisInfo->doStackVerses) {
                            viewpage.append(Hdr);
                        }
                        bbld->ResetFootNote();// reset the footnote index counter for all the bibles
                    }
                        break;

                    case TYPE_DICTIONARY_TYPE:

                        break;
                }
                if (!ShowThisInfo->doStackVerses && index < NumBooks-1) // only if not stacking
                    viewpage.append("<th></th>");// wsm -- fix ios7
//                viewpage.append("<th style=\"\twidth: " PERCENTAGE_SPACE_BETWEEN_COLUMNS_STR "%;\"></th>");// wsm -- fix ios7
            }//displayBible
        }
    }

    // if we are stacking the versions, then the header is one cell, stacked.
    if (ShowThisInfo->doStackVerses) {
        viewpage.append(HTML_TABLE_HEADER_END );
    }


    //-------------------------------------------------------------------------------
    // go build the verses
    //-------------------------------------------------------------------------------
//DoBuildTheVerses:
    limit2 = bcv->NumberOfVerses[Chapter ];
    if (limit2 >  MAX_POSSIBLE_VERSE_NUM )
        return QString("Page build error, book:")+QString::number(bcv->BookNumber)+QString(" chapter:")+QString::number(Chapter);
    viewpage.append("\n\n");

    viewpage.append(  TABLE_STYLE_ABP);

    do{

        //-------------------------------------
        // go through the selected bibles
        //--------------------------------------
        if (!ShowThisInfo->doStackVerses) {
            viewpage.append(  HTML_ROW_START);
        }
        // VERSES
        //============================
        for (index =0; index < NumBooks; index++){
            BTU=&BooksToUse[index];

            DisplayBible = BTU->Index;

            if (DisplayBible < 0 || Books.count()==0)// Then
                continue;

            ShowThisInfo->IndexIntoSelectedBooksArray=index; // used in subsequent calls
            if (ShowThisInfo->doStackVerses) {
                viewpage.append(   HTML_ROW_START);
                viewpage.append(HTML_TABLE_CELL_START);
                viewpage.append("<A id=\"v"+QString::number(sit->VerseNumber+1)+"\"></a>" );
            }


            switch (BTU->Type){
                case TYPE_BBL_TYPE:
                case TYPE_ABP_TYPE:
                case TYPE_MORPHOLOGY_TYPE:
                    bd = Books[DisplayBible];
                    bbld =(BibleDatabaseClass *) bd->Database;


                    if (EnableHTMLHeader ){//Then
                        SHOW_THIS_INFO_TYPE mySti=*ShowThisInfo;
                        mySti.ShowStrongs=NO;

                        // MENUS--> GetVerse() has the magic for each word for a menu.
                        versetxt = bbld->GetVerse(&mySti, /*SearchParmsStructPointer:*/sit);
                    }
                    else {
                        versetxt = bbld->GetVerse(ShowThisInfo, /*SearchParmsStructPointer*/sit);
                    }
                    break;

                case TYPE_COMMENTARY_TYPE:
                {
                    bd = nullptr; // keep the bd tests from failing

                    CommentaryDatabaseClass *nd =(CommentaryDatabaseClass*)((BookHandlerClass *) Books[BTU->Index])->Database;
                    if (nd) {
                        versetxt = nd->GetVerse(ShowThisInfo,/* SearchParmsStructPointer:*/sit );
                    }
                }  break;

                case TYPE_NOTES_TYPE:
                {
                    extern NotesDBClass *gNotes[];
                    //nd = nil; // keep the bd tests from failing

                    NotesDBClass *nd =  gNotes[BTU->Index];
                    sit->indexIntoArray = BTU->Index; // update this index for note identification
                    if (nd) {
                        versetxt = nd->GetVerse(ShowThisInfo,/* SearchParmsStructPointer:*/sit );
                    }
                }  break;
            }//switch


            //-------------------------------------------------------------------------------
            // if we should, mark the verse to scroll to
            //-------------------------------------------------------------------------------
            if (DoMetaTag){// Then
                if  (index == 0){// then
                    if (sit->VerseNumber == MTag){// Then
                        versetxt =  "<A NAME=" QUOTE JavaBookMark QUOTE "></a>"+ versetxt;
                    }
                }
            }

            // wsm -- side by side verse tag
            if (index==0 && !ShowThisInfo->doStackVerses) {
                versetxt = "<A id=\"v"+QString::number(sit->VerseNumber)+"\"></a>" + versetxt;
             //   viewpage.append(versetxt);
            }

            if  (EnableHTMLHeader){// Then
                bmt =str.sprintf(SUPstart "%d" SUPend, sit->VerseNumber);
            }
            else {
                extern QString CreateBookmarkTag(SEARCH_INFO_TYPE *bcv,
                                                 SHOW_THIS_INFO_TYPE *ShowThisInfo,
                                                 int BookToUse,
                                                 BookInfoStructType *BooksToUse);

                bmt= CreateBookmarkTag(sit,ShowThisInfo,index,BooksToUse);//was BooksToUse[index].Index);
            }

            static int BType;// As integer;
            if (bd){// Then
                if (bd->Database) {
                    bbld =(BibleDatabaseClass*) bd->Database;
                    BType = bbld->getDbType();
                }
                else
                    BType = TYPE_BBL_TYPE;// generic
            }
            else{
                BType =TYPE_BBL_TYPE;
            }

            //-------------------------------------------------------------------------------
            // if we are stacking verses, then do so
            //-------------------------------------------------------------------------------
            if (ShowThisInfo->doStackVerses) {
                if (index < Xref.count()) {
                    bmt = Xref [index]+" "+bmt ;
                }

                versetxt=HandleVerseTag(bmt,versetxt, BType,ShowThisInfo) ;// stitch the "1" and the verse together
                viewpage.append( versetxt);

            }else {
                versetxt =  HandleVerseTag(bmt,versetxt, BType,ShowThisInfo);
               //--------------------------------
                viewpage.append(versetxt );// stitch the "1" and the verse together
            }


            if (ShowThisInfo->doStackVerses) {
                // below works, but is not a good solution
                //  if (index == NumBooks-1) {
                //    viewpage.append(HTML_BRStr HTML_BRStr HTML_BRStr );
                // }
                viewpage.append(HTML_TABLE_CELL_END );
                viewpage.append(  HTML_ROW_END );
            } else {
                if (index < NumBooks-1) {
                    viewpage.append("<td style=\"width: " PERCENTAGE_SPACE_BETWEEN_COLUMNS_STR "%;\"></td>");// wsm fix ios7
                }
            }


        }
        //============================
        if (!ShowThisInfo->doStackVerses) {
            viewpage.append(   HTML_ROW_END );

            // the following is for jump forward a verse
            viewpage.append(   HTML_ROW_START HTML_TABLE_CELL_START  );
            viewpage.append(str.sprintf("<A id=\"f%d\"></a>" HTML_TABLE_CELL_END ,sit->VerseNumber ));

            viewpage.append(   HTML_ROW_END );

        }else {
            viewpage.append(  HTML_ROW_END );
            // the following is for jump forward a verse
            viewpage.append(   HTML_ROW_START HTML_TABLE_CELL_START  );
            viewpage.append(str.sprintf("<A id=\"f%d\"></a>" HTML_TABLE_CELL_END ,sit->VerseNumber ));

            viewpage.append(   HTML_ROW_END );
        }


    } while (++sit->VerseNumber <= limit2);



    // add some blank lines
    viewpage.append(  "</table>" BLANK_LINES_24 BLANK_LINES_24 BLANK_LINES_24 "</body></html>");


    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------


//FinishedBuildingVerses:

    versetxt = viewpage.join("");


    //   DurationToNanoSecs(mach_absolute_time()-myCurrentTime);

    return versetxt;//.DefineEncoding(Encodings.UTF8)


}



#if 1

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void BuildPageForBookMark(QString Topic,BookInfoStructType *BooksToUse)
{
    // BIBLE_BCV_TYPE bcv;
    SHOW_THIS_INFO_TYPE sti;
    SEARCH_INFO_TYPE sit;

    sti = globalSTI;

    if (globalSIT.PhraseSearch) {
        sit.PhraseSearch = globalSIT.PhraseSearch;
        sit.PhraseToSearchFor= globalSIT.PhraseToSearchFor;//.toUtf8(),sizeof(globalSIT.PhraseToSearchFor));
    }else {
        sit.SearchForAWord = globalSIT.SearchForAWord;
        sit.WordToSearchFor=globalSIT.WordToSearchFor;//.toUtf8(),sizeof(globalSIT.WordToSearchFor));
    }



    sti.NumberOfBooksBeingShownOnPage = Preferences->getNumberOfSelectedBooks()+Preferences->getNumberOfSelectedNotes()+Preferences->numberOfSelectedCommentaries;

    QStringList verseListArray;

    verseListArray=gBookMarkDB->getBookmarkNamesArrayForTopic(Topic);// retain);


    CurrentWebPage = BuildBookMarkPage(BooksToUse,Topic,verseListArray, &sti, &sit);

}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
QString BuildBookMarkPage(BookInfoStructType *BooksToUse,// array of indexes
                            QString topic,// the topic in the bookmark array
                            QStringList VerseListArray, /* number of chapters in this book, and number of verses in the chapter*/
                            // int NumBooks, /* number of books to show*/
                            SHOW_THIS_INFO_TYPE *ShowThisInfo,
                            SEARCH_INFO_TYPE *sit // also includes book, chapter, verse number
                            ) /* HiliteSearch, DoMetaTag, etc*/
{
    QString Anchor;// for the scroll to verse feature

    //   static BibleBookInfoStructType BibleInfo;

    QString bmt=nil;
    BookHandlerClass *bd=nil;
    BibleDatabaseClass *bbld=nil;

    static int      DisplayBible;
    QString DoMTString;// As String

    static int      index;
    int             index2;

    //  static int      lb;
    static int       limit2;
    int             NumBooks = ShowThisInfo->NumberOfBooksBeingShownOnPage;

    ShowThisInfo->NumberOfBooksBeingShownOnPage--; // subtracting here prevents further math in some subroutines.  This is done once, the subroutines are called 50 to 100 times.

    QString tempString;
    QString stitle;                // short title being shown on stacked mode
    QString versetxt;

    // make porting easier with these  dereferences
    // int BookNumber = sit->BookNumber;
    // int Chapter = sit->ChapterNumber;// ->ChapterNumber;
    BOOL DoMetaTag = ShowThisInfo->DoScrollToVerse;
    BOOL EnableHTMLHeader = ShowThisInfo->EnableHTMLHeader; // if true we are printing
    int MTag = ShowThisInfo->VerseToScrollTo;
    BookInfoStructType *BTU;



   // (void)EnableHTMLHeader; // quiet analyzer

    ShowThisInfo->CompletedRun = YES;



    //uint64_t myCurrentTime = mach_absolute_time();// start the clock


     QStringList VSTitles; // up to 20 short titles.  more than will fit on a screen
    QStringList Xref;

    QStringList viewpage;
    sit->VerseNumber =1; // reset this so we build the whole page
    ShowThisInfo->IDNumber=1; // start at item number 1  used for calculation of where strongs number should be

    //----------------------------------------------------------
    // BookNumber --> The number of the book to show the chapter from
    // Chapter --> the chapter to show
    // BooksToUse() --> the books to pull hte chapter from [ SAME AS Preferences->SelectedBooks [] ]
    // NumBooks --> THe number of books to show
    // HilightSearch --> A search was done, so hilight the searched word
    // Searched --> THe Searched for word
    // DoMetaTag --> use JavasScript to scroll to a position on the page
    // MTag --> the verse to scroll to
    // DoHideNotes --> When true, notes are hidden inside the verse number, false they are a clickable link
    // ShowThisInfo->IDNumber is used to tag each strong's number for location information
    //----------------------------------------------------------

    //Xref = [NSMutableArray arrayWithCapacity:ShowThisInfo->NumberOfBooksBeingShownOnPage);
   // viewpage = [NSMutableArray arrayWithCapacity:[VerseListArray count]]  ;


    //viewpage = HTML_HEADER2_ALIGN_CENTER
    tempString = "<html><head> <title></title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"" \
    "> <!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"http://www.w3.org/TR/html4/loose.dtd\">";

    viewpage.append(tempString);


    if (DoMetaTag){// then
        Anchor =  JavaBookMark;
    }


    viewpage.append(javascriptScrollOffset);// scrollto and find object offset




    viewpage.append("</head>");

    //------------------------------
    // turn off underlines on all links
    //------------------------------
    viewpage.append( HTML_NO_UNDERLINE_STYLE );


#if 1 // onload does not work on ipad/iphone by itself....HOWEVER, if you don't have the onload here, running the window.location.hash='bookmark'; later won't work.
      // so, you have to have the onload here, and then run the javascript after the document has loaded.  Maybe it triggers this???
    if (DoMetaTag) {//then
        DoMTString = str.sprintf("');\"  link=\"#%02x%02x%02x\">" ,Preferences->red,Preferences->green,Preferences->blue);
        DoMTString = "<body onload=\"jumpScroll('" + Anchor + DoMTString;
    }
    else
#endif
    {
        DoMTString = str.sprintf("<body link=\"#%02x%02x%02x\">",Preferences->red,Preferences->green,Preferences->blue);// style="+QUOTE+"font-size:45px;"+QUOTE+">"
    }

    DoMTString = DoMTString.replace("&h","#",Qt::CaseInsensitive);// myReplace(DoMTString,"&h","#")
    viewpage.append(   DoMTString);

    //    viewpage.append( posn); // add the mouse position object


    //------------------------------
    //   build the table header
    //------------------------------
    // change the zz% to 50% or whatever to force the table width
    versetxt = GetWidthPercentage(NumBooks);
    Anchor =Preferences->PreferredTableStyle;
    Anchor = Anchor.replace("zz",/* withString:*/versetxt, Qt::CaseInsensitive);
    QString lstr =QString::number(Preferences->getFontSize());
    Anchor = Anchor.replace("ff",lstr,Qt::CaseInsensitive); //Anchor = myReplace(Anchor,"ffpt",Str(app.FontSize)+"pt")
    Anchor = Anchor.replace("btm%","13",Qt::CaseInsensitive);   // give 1 pixel padding to bottom of header
    Anchor = Anchor.replace("%leftpad%","1",Qt::CaseInsensitive);

    viewpage.append(   Anchor);
    //------------------------------
    //------------------------------
    //viewpage = viewpage+"<p>"

    viewpage.append("<table class=\"sample\"/>" );


    //-------------------------------------------------------------------------------
    // make EnableHTMLHeader = True if we are showing web page for print or copy
    // page with the table header being the books
    //-------------------------------------------------------------------------------
    //const EnableHTMLHeader = False
    EnableHTMLHeader = NO;
    //if (EnableHTMLHeader)
    {// then
        QString Hdr;// As String;
        QString titleString;




        titleString = topic;
        Hdr ="<center><h1><font style='bold' style=" QUOTE "font-size:"+QString::number(Preferences->getFontSize()*4) +";" QUOTE  "color='BLUE' style='BACKGROUND-COLOR:'" BACKGROUND_COLOR "'" ">"+titleString +"</font></h1></center>";


        viewpage.append(Hdr);

        //-------------------------------------------------------------------------------
        // build the Copyright header and the name of the book
        // if this is the top-to-bottom stacking, then create vstitle.
        //-------------------------------------------------------------------------------
        if (ShowThisInfo->doStackVerses) {
            viewpage.append(str.sprintf( "<th style=" QUOTE "font-size:%dpx;" QUOTE ">" ,Preferences->getFontSize()*2 ));
        }
        for (index =0 ; index < NumBooks; index++){
#define BACKGROUND_COLOR "#FFFFFF"
#define FOREGROUND_COLOR "#660000"



            BTU=&BooksToUse[index];

            DisplayBible =BTU->Index;// this is an index into the SelectedBooks [] array

            if (DisplayBible <0) {
                continue; // skip this one
            }

            if (DisplayBible>=0) {
                static unsigned short copyrightID;

                switch(BTU->Type){
                    case TYPE_BBL_TYPE:
                    case TYPE_ABP_TYPE:
                    case TYPE_MORPHOLOGY_TYPE:
                    {

                        bd = Books[DisplayBible];
                        bbld =(BibleDatabaseClass*) bd->Database;
                        bbld->BibleIndexNumber=index; // index into the selected books array for the FootNote section

                        // the type is the second entry, so we can know what array to look up the copyright info from
                        // this is kludgy, and will probably be changed in the next version.
                        titleString = str.sprintf( "<a id='c%u' href=\"http://" COPYRIGHT_INDICATOR_SHOW_WINDOW "_%d_%d_c%u" QUOTE ">"
                                                   ,copyrightID, bbld->BibleIndexNumber, BooksToUse[index].Type,copyrightID);

                        titleString = titleString +  bbld->getTitle()+  HTML_HREF_END;


                        if (ShowThisInfo->doStackVerses) {
                            // stack verses
                            Hdr = titleString + HTML_BRStr ;

                            stitle = bbld->getSTitle();
                            if (stitle.length()) {
                                VSTitles[index]=stitle;
                            } else {
                                QStringList ta = mySplit(bbld->getTitle()," ");
                                if (ta.count()) {
                                    for (index2=0; index2<5; index2++) {
                                        if (index2>ta.count()-1) {
                                            VSTitles[index][index2]=0;
                                            continue;
                                        }
                                        QString tat = ta[index2];

                                        VSTitles[index][index2]= tat.at(0).toLatin1() ;// characterAtIndex:0 );
                                    }
                                    VSTitles[index]=""; // make sure is null terminated, 3 letters for the Bibles

                                    // if the length is too small, make bigger
                                    if (VSTitles[index].length() < 3) {
                                        VSTitles[index]=bbld->getTitle();//.toUtf8(),3);
                                        //for (int index3=0; index3<3; index3++) {
                                        //    VSTitles[index]= [index3]=toupper( VSTitles[index][index3]);
                                        //}
                                    }
                                }

                            }
                            str = HTML_NBSP "<a id='c"+QString::number(copyrightID)+"' href=\"http://"  COPYRIGHT_INDICATOR_SHOW_WINDOW "_"+QString::number(bbld->BibleIndexNumber)+"_"+QString::number(BooksToUse[index].Type)+"_c"+QString::number(copyrightID)+ QUOTE "><FONT style='bold' color='" FOREGROUND_COLOR "' style='BACKGROUND-COLOR:" BACKGROUND_COLOR "'>"+VSTitles[index]+"</FONT>"  HTML_HREF_END;
                            Xref.append(str);
                        }else {
                            Hdr = "<th style=" QUOTE "font-size:"+QString::number(Preferences->getFontSize()*2 )+"px;" QUOTE ">";
                            Hdr = Hdr + titleString+ HTML_TABLE_HEADER_END;
                        }

                        copyrightID++;

                        if (!ShowThisInfo->doStackVerses) {
                            viewpage.append(Hdr);
                        }
                        bbld->ResetFootNote();// reset the footnote index counter for all the bibles
                    }
                        break;

                    case TYPE_COMMENTARY_TYPE:
                    {//
                        int bIndex=BTU->Index;
                        BookHandlerClass *bk;

                        bk = Books[BTU->Index];
                        bbld =(BibleDatabaseClass*) bk->Database;

                        titleString = str.sprintf( "<a id='c%u' href=\"http://" COPYRIGHT_INDICATOR_SHOW_WINDOW "_%d_%d_c%u" QUOTE ">"
                                                   ,copyrightID, bIndex, BTU->Type,copyrightID);
                        titleString = titleString + bbld->getTitle() + HTML_HREF_END;
                        //                        titleString = str.sprintf( HTML_HREF_STARTQ "I+%d+%d" QUOTE ">%ls"  HTML_HREF_END, bIndex,TYPE_COMMENTARY_TYPE, bbld->getTitle());

                        if (ShowThisInfo->doStackVerses) {
                            // stack verses
                            Hdr = titleString + HTML_BRStr ;


                            QString stitle;

                            stitle = bbld->getSTitle();

                            if (stitle.length()) {
                                VSTitles[index]=stitle;//.toUtf8() ,4);
                            } else {
                                VSTitles[index]=bbld->getTitle();//.toUtf8() ,3);
                            }

                            VSTitles[index]=""; // terminate c string

                            //for (int index3=0; index3<3; index3++) {
                            //    VSTitles[index][index3]=toupper( VSTitles[index][index3]);
                            //}

                            str= HTML_NBSP "<a id='c"+QString::number(copyrightID)+"' href=\"http://"  COPYRIGHT_INDICATOR_SHOW_WINDOW "_"+QString::number(bIndex)+"_"+QString::number(BTU->Type)+"_c"+QString::number(copyrightID)+ QUOTE "><FONT style='bold' color='" FOREGROUND_COLOR "' style='BACKGROUND-COLOR:" BACKGROUND_COLOR "'>"+ VSTitles[index]+"</FONT>"  HTML_HREF_END;
                            //   [Xref.append(str.sprintf( HTML_NBSP HTML_HREF_STARTQ "I+%d+%d" QUOTE "><FONT style='bold' color='"FOREGROUND_COLOR"' style='BACKGROUND-COLOR:"BACKGROUND_COLOR"'>%s</FONT>"  HTML_HREF_END, bIndex, TYPE_COMMENTARY_TYPE,VSTitles[index]]);
                            Xref.append(str);

                        }else {
                            Hdr = "<th style=" QUOTE "font-size:"+QString::number(Preferences->getFontSize()*2)+"px;" QUOTE ">" + titleString+ HTML_TABLE_HEADER_END;

                        }

                        copyrightID++;
                        if (!ShowThisInfo->doStackVerses) {
                            viewpage.append(Hdr);
                        }
                        bbld->ResetFootNote();// reset the footnote index counter for all the bibles
                    }
                        break;

                    case TYPE_NOTES_TYPE:
                    {//
                        extern NotesDBClass *gNotes[MAX_NUM_BIBLES];
                        int bIndex=BTU->Index;

                        bbld = gNotes[bIndex];

                        if (ShowThisInfo->doStackVerses) {
                            // stack verses
                            Hdr = bbld->getTitle() + HTML_BRStr ;
                            VSTitles[index]=bbld->getTitle();//.toUtf8(),4);
                            //VSTitles[index][4]=0; // terminate c string
                            //for (int index3=0; index3<4; index3++) {
                            //    VSTitles[index][index3]=toupper( VSTitles[index][index3]);
                            //}


                            str= HTML_NBSP "<a id='c"+QString::number(copyrightID)+"' href=\"http://"  COPYRIGHT_INDICATOR_SHOW_WINDOW "_"+QString::number(bIndex)+"_"+QString::number(BTU->Type)+"_c"+QString::number(copyrightID)+ QUOTE "><FONT style='bold' color='" FOREGROUND_COLOR "' style='BACKGROUND-COLOR:" BACKGROUND_COLOR "'>"+VSTitles[index]+"</FONT>"  HTML_HREF_END;
                            //                            [Xref.append(str.sprintf( HTML_NBSP "<FONT style='bold' color='"FOREGROUND_COLOR"' style='BACKGROUND-COLOR:"BACKGROUND_COLOR"'>%s</FONT>"  ,  VSTitles[index]]);
                            Xref.append(str);

                        }else {
                            titleString = str.sprintf( "<a id='c%u' href=\"http://" COPYRIGHT_INDICATOR_SHOW_WINDOW "_%d_%d_c%u" QUOTE ">",copyrightID, bIndex, BTU->Type,copyrightID);
                            titleString = titleString +  bbld->getTitle()+  HTML_HREF_END;
                            Hdr =str.sprintf( "<th style=" QUOTE "font-size:%dpx;" QUOTE ">",Preferences->getFontSize()*2);
                            Hdr = Hdr + titleString + HTML_TABLE_HEADER_END;
                        }

                        copyrightID++;

                        if (!ShowThisInfo->doStackVerses) {
                            viewpage.append(Hdr);
                        }
                        bbld->ResetFootNote();// reset the footnote index counter for all the bibles
                    }
                        break;

                    case TYPE_DICTIONARY_TYPE:

                        break;
                }
            }
        }
    }

    // if we are stacking the versions, then the header is one cell, stacked.
    if (ShowThisInfo->doStackVerses) {
        viewpage.append(HTML_TABLE_HEADER_END );
    }


    //-------------------------------------------------------------------------------
    // go build the verses
    //-------------------------------------------------------------------------------
//DoBuildTheVerses:
    limit2 = VerseListArray.count();
    int VerseNumberCounter =0; // start out with first item in the list

    // always add in the ABP stuff
    // if we are printing, then add in the abp style stuff
    //if (EnableHTMLHeader){// Then
    viewpage.append( TABLE_STYLE_ABP);
    //}
  if (limit2){
        do{
            viewpage.append(  HTML_ROW_START HTML_TABLE_CELL_START );
            //------------------------------
            // build the book/chapter header info
            //-------------------------------
            versetxt = "<B>"+VerseListArray[VerseNumberCounter]+"</B>";
            viewpage.append( versetxt ); // BibleInfo.BookName+" Chapter "+Str(Chapter)
                                             //viewpage = viewpage + HTML_HEADER2_END

            QString bookMarkID = gBookMarkDB->getVariableByName(VerseListArray[VerseNumberCounter ], /*andCategoryOrTopicName:*/topic);
            int lbook,lchapter,lverse;
            QStringList bcvArray;

            bcvArray = mySplit(bookMarkID,":");

            if (bcvArray.count() > 2) {
                lbook =bcvArray[0].toInt();
                lchapter =bcvArray[1].toInt();
                lverse =bcvArray[2].toInt();
            } else {
                lbook =1; lchapter=1;lverse=1;
            }

            sit->VerseNumber = lverse;
            sit->BookNumber = lbook;
            sit->ChapterNumber = lchapter;


            sit->BookTableName =getBibleBookInfo(lbook).TableName;


            //============================
            if (!ShowThisInfo->doStackVerses) {
                viewpage.append(   HTML_TABLE_CELL_END HTML_ROW_END );

                // the following is for jump forward a verse
                viewpage.append(   HTML_ROW_START HTML_TABLE_CELL_START  );
                viewpage.append(str.sprintf("<A id=\"f%d\"></a>" HTML_TABLE_CELL_END ,sit->VerseNumber ));

                viewpage.append(   HTML_ROW_END );

            }else {
                viewpage.append(   HTML_ROW_END );
                // the following is for jump forward a verse
                viewpage.append(   HTML_ROW_START HTML_TABLE_CELL_START  );
                viewpage.append(str.sprintf("<A id=\"f%d\"></a>" HTML_TABLE_CELL_END ,sit->VerseNumber ));

                viewpage.append(   HTML_ROW_END );
            }




            //-------------------------------------
            // go through the selected bibles
            //--------------------------------------
            // if (!ShowThisInfo->doStackVerses) {
            //         viewpage.append( @  HTML_ROW_START);
            //     }
            //============================
            for (index =0; index < NumBooks; index++){
                BTU=&BooksToUse[index];

                DisplayBible = BTU->Index;

                if (DisplayBible < 0 )// Then
                    continue;

                ShowThisInfo->IndexIntoSelectedBooksArray=index; // used in subsequent calls
                if (ShowThisInfo->doStackVerses) {
                    viewpage.append(   HTML_ROW_START);
                    viewpage.append(HTML_TABLE_CELL_START);
                    viewpage.append(str.sprintf("<A id=\"v%d\"></a>"  ,sit->VerseNumber+1 ));
                }


                switch (BTU->Type){
                    case TYPE_BBL_TYPE:
                    case TYPE_ABP_TYPE:
                    case TYPE_MORPHOLOGY_TYPE:
                        bd = Books[DisplayBible];
                        bbld =(BibleDatabaseClass*) bd->Database;


                        if (EnableHTMLHeader ){//Then
                            SHOW_THIS_INFO_TYPE mySti=*ShowThisInfo;
                            mySti.ShowStrongs=NO;

                            // MENUS--> GetVerse() has the magic for each word for a menu.
                            versetxt = bbld->GetVerse(&mySti, /* SearchParmsStructPointer:*/sit);
                        }
                        else {
                            versetxt = bbld->GetVerse(ShowThisInfo,/* SearchParmsStructPointer:*/sit);
                        }
                        break;

                    case TYPE_COMMENTARY_TYPE:
                    {
                        bd = nil; // keep the bd tests from failing

                        CommentaryDatabaseClass *nd =(CommentaryDatabaseClass *)((BookHandlerClass *) Books[BTU->Index]->Database);
                        if (nd) {
                            versetxt = nd->GetVerse(ShowThisInfo,/* SearchParmsStructPointer:*/sit );
                        }
                    }  break;


                    case TYPE_NOTES_TYPE:
                    {
                        extern NotesDBClass *gNotes[];
                        //nd = nil; // keep the bd tests from failing

                        NotesDBClass *nd =  gNotes[BTU->Index];
                        sit->indexIntoArray = BTU->Index; // update this index for note identification
                        if (nd) {
                            versetxt = nd->GetVerse(ShowThisInfo,/* SearchParmsStructPointer:*/sit );
                        }
                    }  break;
                }//switch


                //-------------------------------------------------------------------------------
                // if we should, mark the verse to scroll to
                //-------------------------------------------------------------------------------
                if (DoMetaTag){// Then
                    if  (index == 0){// then
                        if (sit->VerseNumber == MTag){// Then
                            versetxt =  "<A NAME=" QUOTE JavaBookMark QUOTE "></a>" + versetxt;
                        }
                    }
                }

                // wsm -- side by side verse tag
                if (index==0 && !ShowThisInfo->doStackVerses) {
                    versetxt = str.sprintf("<A id=\"v%d\"></a>"  ,sit->VerseNumber+1 )+ versetxt;
                    //   viewpage.append(versetxt);
                }

                if  (EnableHTMLHeader){// Then
                    bmt =str.sprintf(SUPstart "%d" SUPend, sit->VerseNumber);
                }
                else {
                    extern QString CreateBookmarkTag(SEARCH_INFO_TYPE *bcv,
                                                     SHOW_THIS_INFO_TYPE *ShowThisInfo,
                                                     int BookToUse,
                                                     BookInfoStructType *BooksToUse);

                    bmt= CreateBookmarkTag(sit,ShowThisInfo,index,BooksToUse);//was BooksToUse[index].Index);
                }

                static int BType;// As integer;
                if (bd){// Then
                    if (bd->Database) {
                        bbld =(BibleDatabaseClass*) bd->Database;
                        BType = bbld->getDbType();
                    }
                    else
                        BType = TYPE_BBL_TYPE;// generic
                }
                else{
                    BType =TYPE_BBL_TYPE;
                }

                //-------------------------------------------------------------------------------
                // if we are stacking verses, then do so
                //-------------------------------------------------------------------------------
                if (ShowThisInfo->doStackVerses) {
                    if (index < Xref.count()) {
                        bmt =Xref[index]+" "+bmt;
                    }

                    versetxt=HandleVerseTag(bmt,versetxt, BType,ShowThisInfo) ;// stitch the "1" and the verse together
                    viewpage.append( versetxt);

                }else {
                    versetxt =  HandleVerseTag(bmt,versetxt, BType,ShowThisInfo);
                    //--------------------------------
                    viewpage.append(versetxt );// stitch the "1" and the verse together
                }


                if (ShowThisInfo->doStackVerses) {
                    // below works, but is not a good solution
                    //  if (index == NumBooks-1) {
                    //    viewpage.append(HTML_BRStr HTML_BRStr HTML_BRStr );
                    // }
                    viewpage.append(HTML_TABLE_CELL_END );
                    viewpage.append(   HTML_ROW_END );
                }


            }
            //============================
            if (!ShowThisInfo->doStackVerses) {
                viewpage.append(   HTML_ROW_END );

                // the following is for jump forward a verse
                viewpage.append(   HTML_ROW_START HTML_TABLE_CELL_START  );
                viewpage.append(str.sprintf("<A id=\"f%d\"></a>" HTML_TABLE_CELL_END ,sit->VerseNumber ));

                viewpage.append(   HTML_ROW_END );

            }else {
                viewpage.append(   HTML_ROW_END );
                // the following is for jump forward a verse
                viewpage.append(  HTML_ROW_START HTML_TABLE_CELL_START  );
                viewpage.append(str.sprintf("<A id=\"f%d\"></a>" HTML_TABLE_CELL_END ,sit->VerseNumber ));

                viewpage.append(   HTML_ROW_END );
            }


        } while (++VerseNumberCounter < limit2);// go to count -1
    }


    // add some blank lines
    viewpage.append(  "</table>" BLANK_LINES_24 BLANK_LINES_24 BLANK_LINES_24 "</body></html>");


    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------


//FinishedBuildingVerses:

    versetxt = viewpage.join("");


    //   DurationToNanoSecs(mach_absolute_time()-myCurrentTime);

    return versetxt;//.DefineEncoding(Encodings.UTF8)


}

#endif





