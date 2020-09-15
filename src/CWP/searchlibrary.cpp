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
//  searchLibrary.m
//  Crossword-Try3
//
//  Created by WADE MAXFIELD on 10/14/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//converted

#include <QLabel>
#include <QList>
#include "searchlibrary.h"
#include "../shared/structs.h"
#include "../shared/RecordSet/recordset.h"
#include "../shared/chapterversestructclass.h"
#include "../shared/bibleinfo.h"
#include "../shared/BibleDatabaseClass/bibledatabaseclass.h"
//#include "notesLibrary.h"
#include "commentarymodule.h"
#include "biblemodule.h"
#include "notesdbclass.h"

extern NotesDBClass    *getNotesDB(int NoteDBIndex);

int resultsArrayNumItems;
//ChapterVerseStructType resultsArray[MAX_ENTRIES_IN_RESULTS_ARRAY+5];// give extra room

 QList<ChapterVerseStructType>resultsArray;


//---------------------------------------------------
//---------------------------------------------------
BOOL  DoSearch(SearchInfoStructType *s,QList<ChapterVerseStructType> *ResultList,int *numItems, SearchingWindowStructType *SearchingWindow)
{
    // return YES;  for (good search, NO; for (not. put results in ResultList
    //dim index As integer
    int index;
    //dim r1,r2 As Boolean // when YES; , found something
    BOOL r1=NO;
    BOOL r2=NO;


    // we have the translation index, do the search
    *numItems=0;

    //----------------------------------------------------
    // if the search is limited to a book,{ //then search only the book
    //----------------------------------------------------
    if (s->SearchThisBookOnly){ //then
        if (SearchBook(s,ResultList,numItems,SearchingWindow)){ //then
            return YES;
        } //end if
        return NO;
    } //end if

    //----------------------------------------------------
    // if the search allows the OT,{ //then search the OT
    //----------------------------------------------------
    if (s->OT){ //then
        for (index = 1; index <= LAST_BOOK_IN_OT; index++) {
            s->BookTableName = getBibleBookInfo(s->BookNumber).TableName;
            r1=SearchBook(s,ResultList,numItems,SearchingWindow); // perform the search and fill list box
        } //next
    } //end if

    //----------------------------------------------------
    // if the search allows the NT,{ //then search the NT
    //----------------------------------------------------
    if (s->NT){ //then
        for (index =  LAST_BOOK_IN_OT+1 ; index <=Preferences->NumberOfBooksInBible; index++) {
            s->BookTableName = getBibleBookInfo(s->BookNumber).TableName;
            r2=SearchBook(s,ResultList,numItems,SearchingWindow); // perform the search and fill list box
        } //next
    } //end if


    if (r1 || r2 ){ //then
        return YES;
    }else{
        return NO;
    } //end if


}

//---------------------------------------------------
//---------------------------------------------------
void fillLabelsWithData(QString VerseNumberLabel, QString VerseTextLabel, int itemNumber,int BibleNumber)
{
    QString str;
   // BookHandlerClass *bk;
    BibleDatabaseClass *bbld=nil;
    //CommentaryDatabaseClass *cmtd=nil;
    ChapterVerseStructType cvs=resultsArray[itemNumber];

    //extern NSMutableArray *Books;
    extern QList<BookHandlerClass *>      Books;
    BibleBookInfoStructType bbis = getBibleBookInfo(cvs.BookNumber);


    SEARCH_INFO_TYPE sit;
    sit.BookNumber = cvs.BookNumber;
    sit.ChapterNumber = cvs.Chapter;
    sit.VerseNumber = cvs.Verse;
     sit.BookTableName= bbis.TableName;

    int bblidx =Preferences->SelectedBooks[BibleNumber].Index;
    if (bblidx<0) {
        bblidx=0;
    }
    bbld =(BibleDatabaseClass*)((BookHandlerClass*)Books[bblidx])->Database;
    VerseTextLabel= bbld->GetPlainVerse(&globalSTI,/* SearchParmsStructPointer:*/&sit ,/*Type:*/bbld->getDbType() );

    VerseNumberLabel=bbis.BookName +". "+QString::number( cvs.Chapter) +":"+QString::number( cvs.Verse);





}




//---------------------------------------------------
//---------------------------------------------------

RecordSet *runSelectQuery(SearchInfoStructType *s,QString Query)
{
    BookHandlerClass *bk;

    if (s->isBibleSearch) {

        bk = Books[s->indexIntoArray]; // (s->TranslationNumber).DB.SQLSelect(Query);

       return ((SQliteDatabase *) bk->Database)->SQLSelect(Query);

    }

    if (s->isCommentarySearch) {
        CommentaryDatabaseClass *cdb;
        bk = Books [s->indexIntoArray];
        cdb = (CommentaryDatabaseClass*)bk->Database;
        return cdb->SQLSelect(Query);
    }

    if (s->isNoteSearch) {
        NotesDBClass *ndb;
        ndb = getNotesDB(s->indexIntoArray);
        return ndb->SQLSelect(Query);

    }
    return nil;
}

//---------------------------------------------------
//---------------------------------------------------
ChapterVerseStructType FindWord(SearchInfoStructType *s)
{
    // s.WordToSearchFor MUST be a single word

    //dim rs As RecordSet
    RecordSet *rs;
    //dim Words(-1) As String
    QStringList Words;
    //dim Query As String
    //dim index as integer
  //  int index;
    //dim cvs As ChapterVerseStruct
    //dim st As String
    // break the query up into words to look for
    QString Query;
    QString str;
    //BookHandlerClass *bk;
    //BibleDatabaseClass *bbld=nil;
    //CommentaryDatabaseClass *cmtd=nil;
    ChapterVerseStructType cvs;

    //extern QList<BookHandlerClass *>      Books;

    Words = mySplit( s->WordToSearchFor," ");

    cvs.BookNumber = -1;// trigger for a failed search
    cvs.Verse = -1;
    cvs.Posn = -1;
    cvs.Chapter = -1;

    // fail on multiple words
    if (Words.count()>1)
        return cvs;

  //  index = 0;

    Query = "Select " WORD_COLUMN " from "+s->BookTableName +" where word like '%%"+s->WordToSearchFor+"%%' and " CHAPTER_COLUMN " = "+QString::number(s->Chapter);
    Query = Query +  " and " VERSE_COLUMN " = "+QString::number(s->Verse)+" and " WORD_SEQ_COLUMN " = "+QString::number(s->WordSequenceNumber);

    rs = runSelectQuery(s,Query);


    if (rs  != Nil ){ //then
        if (! rs->atEOF() ){ //then
            cvs.Chapter = s->Chapter; //CType(s.Chapter,Int16)
            cvs.Verse = s->Verse ; //CType(s.Verse,Int16)
            cvs.BookNumber = s->BookNumber;//CType( s.BookNumber,Int16)
            cvs.Posn = s->WordSequenceNumber;// CType(s.WordSequenceNumber ,Int16)// word sequence
        } //end if

    } //end if


//SearchFinished:
    return cvs;

}

//---------------------------------------------------
//---------------------------------------------------
BOOL HandlePhraseSearch(SearchInfoStructType *s,QList<ChapterVerseStructType> *ResultList ,int *numItems,SearchingWindowStructType *SearchingWindow) //As boolean
{
    Q_UNUSED(SearchingWindow);
    RecordSet *rs;

    QStringList Words;
    QString s1,str;
    QString Query;

    int index;
  //  int loopCount=0;

    BOOL r=NO;

  //  int index2;

    ChapterVerseStructType sr;
    ChapterVerseStructType sr1={-1,-1,-1,-1,-1};// = *ResultList;
    SearchInfoStructType sis;

  //  ChapterVerseStructType cvs=*ResultList;

    QString phrase;

    phrase = s->PhraseToSearchFor;

    // break the query up into words to look for
    Words = mySplit(phrase," ");

    if (Words.count() == 0 )
        return NO;

    index = 0;

    s1 = Words [index];
    s1 = s1.replace("'","\'");

    if (s->SearchForAWholeWord){ //then // search for a whole word, ! a "like" word
        Query =   "Select " CHAPTER_COLUMN "," VERSE_COLUMN ", " WORD_SEQ_COLUMN  " from "+s->BookTableName+" where word = '"+s1+"' limit "+QString::number(Preferences->MaximumSearches);

        //Query = "Select chapter,verse from " s.BookTableName " where word = '" Words(index) "' limit " Str(MaximumSearches)
    }else{  // search for a like
        Query = "Select " CHAPTER_COLUMN ", " VERSE_COLUMN ", " WORD_SEQ_COLUMN  " from "+s->BookTableName+" where word like '%%"+s1+"%%' limit "+QString::number(Preferences->MaximumSearches);
    } //end if


    rs = runSelectQuery(s,Query);



    if (rs  != Nil ){ //then

        sis.BookTableName = s->BookTableName;
        while (! rs->atEOF() ) {
            // we have a candidate word for the phrase.
            // see if the next word in the verse is the next word we are looking for

            // concatenate the chapter and verse using a ":"


            sr.Chapter = rs->IdxField(1).toInt();
            sr.Verse = rs->IdxField(2).toInt();
            sr.BookNumber = s->BookNumber;
            sr.Posn = rs->IdxField(3).toInt();

            if (!(sr1.Chapter == sr.Chapter && sr1.Verse == sr.Verse && sr1.BookNumber == sr.BookNumber)){ //then
                // we have the first word.  Get the sentence from the bible and see if the phrase is in there
                QString verse;
                BookHandlerClass *bk;
                BibleDatabaseClass *bbl;
                SHOW_THIS_INFO_TYPE sti;
                SEARCH_INFO_TYPE sit;
                sti=globalSTI;

                sit.BookNumber = sr.BookNumber;
                sit.ChapterNumber = sr.Chapter;
                sit.VerseNumber = sr.Verse;
                sit.BookTableName= s->BookTableName;

                bk = Books [s->indexIntoArray]; // (s->TranslationNumber).DB.SQLSelect(Query);
                bbl = (BibleDatabaseClass*)bk->Database;
                if (bbl) {
                    verse = bbl->GetPlainVerse(&sti,/* SearchParmsStructPointer:*/&sit, /*Type:*/TYPE_BBL_TYPE);
                } else {
                    verse = "";
                }


                if (Instr(verse, phrase)) {
                   // ResultList[(*numItems)++]=sr;
                    ResultList->append(sr);
                    *numItems = ResultList->count();

                    sr1 = sr;

                    if (*numItems >Preferences->MaximumSearches){ //then
                        goto SearchFinished;
                    } //end if

                    r =YES;; // found a match

                } //end if
            }  //if (!(sr1.Chapter...))
            // this is bail point if duplicate verse
        //SkipThisSearch:

            rs->MoveNext();
        } //wend
    } //end if

SearchFinished:
    if (rs) {
        rs->done();// release sqlite table NOW
        delete rs;
    }

    return r;
}

//---------------------------------------------------
//---------------------------------------------------
BOOL HandleStrongsSearch(SearchInfoStructType *s ,
                         QList<ChapterVerseStructType> *ResultList,
                         int *numItems,
                         SearchingWindowStructType *SearchingWindow ) //As boolean
{
    RecordSet *rs;

    QString Query;
    QString str;

    BOOL r=NO;
    Q_UNUSED(SearchingWindow);

    ChapterVerseStructType sr;

   ChapterVerseStructType sr1={-1,-1,-1,-1,-1};//=ResultList[0];




    Query =   "Select chapter,verse from "+s->BookTableName+" where " ORIG_WORD_TYPE_COLUMN " = '"+s->Language+"' and " DICTIONARY_ENTRY_COLUMN " = '"+QString::number(s->StrongsNumber)+"' limit "+QString::number(Preferences->MaximumSearches);

    rs = runSelectQuery(s, Query);

    if (rs  != Nil ){ //then

        while (! rs->atEOF() ) {
            // concatenate the chapter and verse using a ":"
            sr.Chapter = rs->IdxField(1).toInt();// IntegerValue,int16)
            sr.Verse = rs->IdxField(2).toInt();// .IntegerValue,int16)
            sr.BookNumber = s->BookNumber;//,int16)
            if ( sr.Verse == sr1.Verse && sr.Chapter == sr1.Chapter&& sr.BookNumber == sr1.BookNumber){ //then
                goto skipsave;
            } //end if
            sr1=sr; // save for later comparison

            ResultList->append(sr);
            *numItems = ResultList->count();

            if (*numItems >Preferences->MaximumSearches){ //then
                goto SearchFinished;
            } //end if

            r =YES; // found a match

        skipsave:
            rs->MoveNext();
        } //wend
    } //end if


SearchFinished:

    if (rs) {
        rs->done();// release sqlite table NOW
        delete rs;//[rs release];
    }
    return r;
}

//---------------------------------------------------
//---------------------------------------------------
BOOL HandleWordSearch(SearchInfoStructType      *s,
                      QList<ChapterVerseStructType> *ResultList,
                      int                       *numItems,
                      SearchingWindowStructType *SearchingWindow ) //As boolean
{
    Q_UNUSED(SearchingWindow);
    RecordSet *rs=0;
    QStringList Words;

    QString Query,s1,str;

    int index;

    BOOL r=YES;

    ChapterVerseStructType sr;

    ChapterVerseStructType sr1={-1,-1,-1,-1,-1};//=ResultList[0];

   // sr1.Chapter =-1;  // prime the pump
    //sr1.Verse=-1;
    // break the query up into words to look for
    Words = mySplit( s->WordToSearchFor ," ");


    if (Words.count()==0 ) //then
        return r;

    for (index = 0 ; index < Words.count(); index++ ) {//to Words.Ubound


        s1 = Words [index];
        s1 = s1.replace("'","\'");


        if (s->SearchForAWholeWord){ //then // search for a whole word, ! a "like" word
            Query = "Select chapter,verse from "+s->BookTableName +" where word = '"+s1+"' limit "+ QString::number(Preferences->MaximumSearches);
         }else{  // search for a like;
            Query =  "Select chapter,verse from "+s->BookTableName +"  where word like '%"+s1+"%' limit "+ QString::number(Preferences->MaximumSearches);
        } //end if


        rs = runSelectQuery(s, Query) ;


        if (rs  != Nil ){ //then
            //[rs retain];
            while (! rs->atEOF() ){
                // concatenate the chapter and verse using a ":"
                sr.Chapter = rs->IdxField(1).toInt();// .IntegerValue,int16)
                sr.Verse = rs->IdxField(2).toInt();//.IntegerValue,int16)
                sr.BookNumber = s->BookNumber;//,int16)
                if ( sr.Verse == sr1.Verse&& sr.Chapter == sr1.Chapter&& sr.BookNumber == sr1.BookNumber){ //then
                    goto skipsave;
                } //end if

                sr1=sr; // save for later comparison
                //ResultList[(*numItems)++]= sr;
                ResultList->append(sr);
                *numItems = ResultList->count();

                if (*numItems >=Preferences->MaximumSearches){ //then
                    goto SearchFinished;
                } //end if

                r =YES; // found a match
            skipsave:
                rs->MoveNext();
            } //wend
        } //end if
    } //next

SearchFinished:
    if (rs) {
        rs->done();// release sqlite table NOW
        delete rs;//[rs release];
    }

    return r;
}

//---------------------------------------------------
//---------------------------------------------------
RecordSet    *SearchBookGetResult(SearchInfoStructType *s,BOOL *result ,SearchingWindowStructType *SearchingWindow)// As recordSet
{
    //---------------------------------------------------
    //search a book in bookname
    //---------------------------------------------------
    Q_UNUSED(SearchingWindow);

    //dim rs As RecordSet
    RecordSet *rs=0;
    //dim Query As String
    QString Query,s1,s2;


    *result = NO;
    s2 = s->WordToSearchFor;
    s1=s2.replace("'","\'");
    //s1 = s2;

    Query ="Select " WORD_COLUMN "," WORD_SEQ_COLUMN " from "+ QString( s->BookTableName) +" where word like '%"+s1+ "%' limit 1";// only find one word

    rs = runSelectQuery(s, Query);

    if (rs  != Nil ){ //then
        *result =YES; // found a match
    } //end if

    return rs;

}

//---------------------------------------------------
//search a book in bookname
//---------------------------------------------------
BOOL SearchBook(SearchInfoStructType *s,
                QList<ChapterVerseStructType> *ResultList,
                int *numItems,
                SearchingWindowStructType *SearchingWindow )
{

    BOOL r;

    if (s->SearchForStrongs){ //then
        // search for the strong's number
        return (HandleStrongsSearch(s,ResultList,numItems,SearchingWindow));
    } //end if


    if (s->SearchForAWord){ //then // ! a phrase search
        s->SearchForAWholeWord=NO;
        return HandleWordSearch(s,ResultList,numItems,SearchingWindow);

    } //end if

    if (s->PhraseSearch){ //then
        s->SearchForAWholeWord=YES;
        r= HandlePhraseSearch(s,ResultList,numItems, SearchingWindow);
        return r;
    } //end if

    return YES;
}

//---------------------------------------------------
//---------------------------------------------------
RecordSet   *SearchBookChapter(SearchInfoStructType *s,BOOL *result)
{
    //---------------------------------------------------
    //search a book in bookname
    //---------------------------------------------------

    //dim rs As RecordSet
    RecordSet *rs;
    //dim Query As String
    QString Query,s1;

    *result = NO;

    s1 =s->WordToSearchFor.replace("'","\'");

    Query ="Select " WORD_COLUMN "," WORD_SEQ_COLUMN " from "+QString(s->BookTableName) +" where word like '%"+s1+"%' limit 1";;// only find one word

    //Query = "Select " WORD_COLUMN "," WORD_SEQ_COLUMN " from " s.BookTableName " where word like '%" s.WordToSearchFor "%' "
    //" and chapter =" str(s.chapter)
   // " limit 1" // only find one word

    rs = runSelectQuery(s, Query);

    if (rs  != Nil ){ //then
        *result =YES; // found a mat
    } //end if


    return rs;

}
//---------------------------------------------------
//---------------------------------------------------
RecordSet *SearchBookChapterVerse(SearchInfoStructType *s ,BOOL *result)
{
    //---------------------------------------------------
    //search a book in bookname
    //---------------------------------------------------

    //dim rs As RecordSet
    RecordSet *rs;
    //dim Query As String
    QString Query;
    //dim SeqLimit As String
    char *SeqLimit;

    *result = NO;

    if (s->WordSequenceNumber == 0){ //then
        SeqLimit = (char*)" >= ";
    }else{
        SeqLimit = (char*)" > ";
    } //end if

    QString s1 =s->WordToSearchFor.replace("'",/* withString:*/"\'");

    Query = "Select " WORD_COLUMN "," WORD_SEQ_COLUMN " from "+QString(s->BookTableName) +" where word like '%"+s1+"%' and ";
    Query = Query + WORD_SEQ_COLUMN+" "+SeqLimit+" "+QString::number(s->WordSequenceNumber)+ " and chapter = "+QString::number(s->Chapter)+
            " and verse = "+QString::number(s->Verse)+" limit 1 ";


    rs = runSelectQuery(s, Query);

    if (rs  != Nil ){ //then
        *result =YES; // found a match
    } //end if



    return rs;

}
//#tag EndMethod

