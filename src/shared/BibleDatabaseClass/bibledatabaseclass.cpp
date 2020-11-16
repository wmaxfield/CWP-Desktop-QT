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
//  BibleDatabase.m
//  Crossword iPad
//
//  Created by WADE MAXFIELD on 9/21/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//converted

#include <QFileInfo>
//#include "defineStudent.h"
#include "cwpdefines.h"
#include "biblemodule.h"
#include "HTMLModule.h"
#include "misclibrary.h"
#include "structs.h"

#include "RecordSet/recordset.h"

#include <QStringList>
#include "BibleDatabaseClass/bibledatabaseclass.h"
#include <QSqlError>
#include "../shared/bibleinfo.h"
#include "MediaModuleDatabase.h"

static unsigned int dictionaryIDNumber;




static QString str;

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BibleDatabaseClass::BibleDatabaseClass() : SQliteDatabase()
{
    _BibleName="";
    databaseName="";
    QueryEnd="";
    QueryBegin="";
    isOpen=0;
    objectType=0;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BibleDatabaseClass::BibleDatabaseClass(QString FileName) : SQliteDatabase() {
    isOpen=NO;
    objectType =0;
   openWithFileName(FileName);

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BibleDatabaseClass::BibleDatabaseClass(QString FileName, int type) : SQliteDatabase() {
    isOpen=NO;
    objectType =0;
    setDbType(type);
   openWithFileName(FileName);

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDatabaseClass::setName(QString lname) {
    _BibleName = lname;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleDatabaseClass::getName() {
    return _BibleName;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int BibleDatabaseClass::getObjectType()
{
    return BOOK_TYPE_BIBLE;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
RecordSet * BibleDatabaseClass::GetVerseRecords(/*forParms*/SEARCH_INFO_TYPE *SIT)
{
    RecordSet *rs;
    QString Query;

   // Q_UNUSED(STI);

    if (!isOpen) {
        Open();
    }

    //                              Select * from BookTableName where Chapter = 1 and Verse =1 QueryEnd
    Query = SIT->BookTableName+" where " CHAPTER_COLUMN " = "+QString::number(SIT->ChapterNumber)+" and " VERSE_COLUMN " = "+QString::number(SIT->VerseNumber);

    Query = QueryBegin + Query + QueryEnd;
    rs =SQLSelect(Query);  // the statement to run for the select.

    if (rs == nil) {
        return nil;
    }

    return rs;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BibleDatabaseClass::updateWordRecordInfo(SEARCH_INFO_TYPE*SIT,
                                              /*usingWord*/QString Word, /*andPre*/QString Pre,
                                               /*andPost*/QString Post, /*andLang*/QString Lang,
                                              /*andNumber*/ QString Number, QString Format) {

    QString Query;
    myReplace(Word,"'","''");
       myReplace(Pre,"'","''");
       myReplace(Post,"'","''");
       myReplace(Lang,"'","''");
       myReplace(Number,"'","''");
       myReplace(Format,"'","''");

   // Query = str.sprintf("Update %s set " WORD_COLUMN "='%ls', " WORD_PRE "='%ls', " WORD_POST "='%ls', " ORIG_WORD_TYPE_COLUMN "='%ls', " DICTIONARY_ENTRY_COLUMN "='%ls' where " CHAPTER_COLUMN "=%d and " VERSE_COLUMN "=%d and " WORD_SEQ_COLUMN"=%d  "
   //                     ,SIT->BookTableName, Word,Pre,Post,Lang,Number,

    Query = QString("Update ")+QString(SIT->BookTableName)+QString(" set " WORD_COLUMN "='") +Word +QString("', " WORD_PRE "='")+Pre+QString("', " WORD_POST "='")+Post+QString("', " ORIG_WORD_TYPE_COLUMN) ;

    Query = Query  + QString("='")+Lang+QString("', " DICTIONARY_ENTRY_COLUMN "='")+Number+QString("', " WORD_FORMAT "='")+Format+
            QString("' where " CHAPTER_COLUMN) ;
    str = str.sprintf("=%d and " VERSE_COLUMN "=%d and " WORD_SEQ_COLUMN"=%d  ",SIT->ChapterNumber, SIT->VerseNumber, SIT->WordSequenceNumber);
    Query = Query + str;



    SQLExecute(Query);

    if (queryError.type() != QSqlError::NoError) {
        MsgBox(ErrorMessage());
    }

    return  queryError.type() != QSqlError::NoError;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDatabaseClass::SaveNoteToDatabase( QString noteText,/* BookNumber*/int bookNumber,/* Chapter*/int chapter,/* Verse*/int verse)
{
   // MsgBox(noteText);
    QString newNote;
    BibleBookInfoStructType bbis = getBibleBookInfo(bookNumber);

    //newNote = myReplace(noteText,"\n", "<br/>\r"); // make this work in an html page, don't let space get into the brstr
    newNote = myReplace(noteText,"'", "''");

    int index=0;

    DeleteFromDatabaseByBook(bookNumber, /*Chapter:*/chapter,/* Verse:*/verse);


    InsertWordIntoDatabase(&bbis, /*Chapter:*/chapter,/* Verse:*/verse, /*WordSequence:*/index,
                           /*Word:*/newNote, /*OrigWordType:*/ "",/* DictionaryEntry:*/0, /*PreWord:*/"",/* PostWord:*/"");

    /*
     for (index = 0; index < [word count);index++){
     if ([[word objectAtIndex:index] length]) {
     [self InsertWordIntoDatabase:&bbis Chapter:chapter Verse:verse WordSequence:index Word:[word objectAtIndex:index] OrigWordType:@ " " DictionaryEntry:0 PreWord:" PostWord:");
     if ([self Error ]) {
     MsgBox(ErrorMessage());
     }
     }

     }
     */

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDatabaseClass::DeleteFromDatabaseByBook(int bookNumber , /*Chapter*/ int chapter , /*Verse*/ int verse )
{
    QString Query;
    BibleBookInfoStructType bbis = getBibleBookInfo(bookNumber);


    // this must be a "modern" database
    Query = "delete from "+bbis.TableName+" where " CHAPTER_COLUMN "= "+QString::number(chapter)+" and "  VERSE_COLUMN "= "+QString::number( verse) ;
    SQLExecute(Query);


}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleDatabaseClass::GetTextFromDatabaseByBook(int bookNumber , /*Chapter*/ int chapter , /*Verse*/ int verse )
{
    // BIBLE_BCV_TYPE bcv;
    SHOW_THIS_INFO_TYPE sti;
    SEARCH_INFO_TYPE sit;

    sti = globalSTI;
    sti.ShowStrongs = NO;

#if STUDENT_OR_TEACHER
    sti.enableEnglishDictionary=NO;
#endif
    sit.BookNumber=bookNumber;
    sit.ChapterNumber=chapter;
    sit.VerseNumber = verse;




    BibleBookInfoStructType bbis = getBibleBookInfo(bookNumber);


    sit.BookTableName =bbis.TableName;
    QString note=  GetVerse(&sti,/* SearchParmsStructPointer:*/&sit );

    //note = myReplace(note , HTML_BRStr , "\n");
    //note = myReplace(note , "<br/>","\n");
    return note;
}





// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDatabaseClass:: createIndex( QString TableName)
{

    QString Query;// As String

    Query ="create  index if not exists "+TableName+"IDX on "+TableName+"(Chapter ASC ,Verse ASC ,WordSeqNumber ASC)";
    SQLExecute(Query); // create the index


    if (Error()){//then
        if (errorCode() != 8){//then // only show non-write errors
            MsgBox (ErrorMessage());
        }//end if
    }

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BibleDatabaseClass::createTables()
{

    QString Query;// as String
    BibleBookInfoStructType BibleInfo;

    //--------------------------------------------
    // create 66 books

    int BookNumber;// As integer

    showSQLExecError=true;

    for (BookNumber = 1 ; BookNumber<= NUM_BOOKS_IN_BIBLE; BookNumber++){
        BibleInfo = getBibleBookInfo(BookNumber);

        Query = "create table "+BibleInfo.TableName +"  (" CHAPTER_COLUMN " integer KEY," VERSE_COLUMN " integer KEY," WORD_SEQ_COLUMN " integer,"  WORD_COLUMN " varchar," WORD_PRE " varchar," WORD_POST " varchar," WORD_FORMAT " integer, "  ORIG_WORD_TYPE_COLUMN " char(1), " DICTIONARY_ENTRY_COLUMN " integer)";

        SQLExecute(Query) ;

        if (queryError.type() != QSqlError::NoError){ //Then
            MsgBox(_BibleName+":BibleDatabaseClass:createTables table create error: "+ ErrorMessage());
            return NO;
        }

        createIndex(BibleInfo.TableName);
    }

    // create short name to full name links

    Query = "create table "+QString( TESTAMENT_TYPE_TABLE)+ " ("  BOOK_SNAME_COLUMN " text primary key not null," BOOK_COLUMN " text key," TESTAMENT_COLUMN " text)";
    SQLExecute(Query);



    Query = "create table " TITLE_TABLE "(" TITLE_COLUMN " varchar )" ;
    SQLExecute(Query);

    Query = "create table " BIBLE_TYPE_TABLE "(" BIBLE_TYPE_FIELD " integer )" ;
    SQLExecute(Query);
    // fix our type so it will be correct
    Query = str.sprintf( "insert into " BIBLE_TYPE_TABLE "(" BIBLE_TYPE_FIELD ") values (%d) ",getDbType() );
    SQLExecute(Query);
    if (Error()) { // then
        MsgBox( "An error.  The error was "+ErrorMessage());
        return NO;
    } // end if


    showSQLExecError=false;

    return YES;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDatabaseClass::setTitle( QString ttl)
{
    extern QString   dbEscapeSingleQuoteString(QString inputString); // misclibrary.h
    QString Query;

    // this must be a "modern" database
    Query =  "delete from " TITLE_TABLE  ;
    SQLExecute(Query);
    ttl = dbEscapeSingleQuoteString(ttl);
    Query = "insert into " TITLE_TABLE " values ('"+ttl+"')";
    SQLExecute(Query);

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleDatabaseClass::getTitle()
{
    RecordSet *rs;
    QString Query;
    QString TitleText;
    BOOL StripFormatting = YES;
    QString str;

    if (!isOpen) {
        Open();
    }

    // this must be a "modern" database
    Query =  "select * from " TITLE_TABLE  ;

    rs = SQLSelect(Query);

    if (rs != Nil ) {//then

        //----------------------------------------------------------------
        // IF LINUX, then font style does not work
        //add <font> tag into the  text
        //------------------------------------------------------------------
        TitleText = rs->IdxField(1);
        if (!StripFormatting){// Then
                              // do it with style
            str = str.sprintf( "<p style=" QUOTE "font-size:%dpt" QUOTE HTML_GENERIC_END_TAG ,Preferences->getFontSize()*2);
            TitleText = str + TitleText;
        }

        //rs->done();// deallocate the sqlite stuff
        delete rs;
        return  TitleText;

    }//end if

    QFileInfo f(_BibleName);

    return  f.baseName(); //[self._BibleName stringByDeletingPathExtension]  ; // the bible name as the default


}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleDatabaseClass::getSTitle() {
    RecordSet *rs;
    QString Query;
    QString TitleText;
    //  BOOL StripFormatting = YES;

    // this must be a "modern" database
    Query =  "select " STITLE_COLUMN " from " STITLE_TABLE ;

    rs = SQLSelect(Query);

    if (rs != Nil ) {//then

        //----------------------------------------------------------------
        // IF LINUX, then font style does not work
        //add <font> tag into the  text
        //------------------------------------------------------------------
        TitleText = rs->IdxField(1);

        rs->done();// deallocate the sqlite stuff

        return  TitleText;

    }//end if

    return "";

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleDatabaseClass::getBibleName()
{
    return _BibleName;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDatabaseClass::setBibleName( QString bn)
{
    if (Instr(bn, ".bbl")|| Instr(bn, ".cmt")) {
        bn =  bn.left(bn.length()-4); //Left(bn, [bn length]-4);
    }
    _BibleName=bn;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
long BibleDatabaseClass::Close()
{
    SQliteDatabase::Close();
    isOpen = NO;
    //return lastReturnFromSqlite3;
    return 0;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BibleDatabaseClass::Open() {
    if (_BibleName.isEmpty()) {
        return NO;
    }
    if (getDbType()==0) {
        return  openWithType(TYPE_BBL_TYPE);
    }

    return openWithType(getDbType());
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int BibleDatabaseClass::openWithFileName( QString BibleFileName)
{
    QFileInfo f(BibleFileName);

    _BibleName = f.baseName();

    QueryBegin = "SELECT pre, Word, post, OrigWordLanguage, DictEntryNumber, WordSeqNumber, WordFormat from " ;
    QueryEnd = " ORDER BY WordSeqNumber ";


    if (!Instr(BibleFileName, ".")) {

        databaseName="" ;


        if (openWithType(TYPE_BBL_TYPE)== SUCCESS) {
            initDBType();// pick up the odd bible type.
            return SUCCESS;
        }

    } else {
        // this is expected to be a FULL database name, path included
        databaseName =BibleFileName;
        // databaseName = [BibleFileName copy);
        return Open();// pick up other code benefits
    }

    return FAIL;

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL BibleDatabaseClass::openWithType(int lType)
{
    BOOL openResult;

    if (isOpen) {
        Close();
    }

    if (!QueryBegin.length()) {
        //                 1    2     3         4                5               6              7
        QueryBegin =" SELECT pre, Word, post, OrigWordLanguage, DictEntryNumber, WordSeqNumber, WordFormat from " ;
    }
    if (!QueryEnd.length()) {
        QueryEnd =  "ORDER BY WordSeqNumber ";
    }
    // DictionaryStart = <a style=\"color:black\" href=\"D+";

    if (databaseName.length()==0) {
        // if the name does not have ".bbl" appended, then add it
        if (Instr(_BibleName, getSuffixStringFromTypeInt(lType))== 0) {
            databaseName = _BibleName + getSuffixStringFromTypeInt(lType);
        } else {
            databaseName = _BibleName;
        }
    }

    // if we have an "/" in the database name, assume that the path component is
    // already there
    if (!Instr(databaseName , "/")) {

        if (pathComponent.length()) {
            databaseName = pathComponent + "/"+databaseName;
//            databaseName = pathComponent + databaseName;
        } else {

            // the type of file changes where the database will be stored.  since this is a bible database type (which includes notes),
            // then we only have to look at a few changes.  The user notes are stored in the documents directory proper.
            switch (lType) {
                default:
                case TYPE_BBL_TYPE:
                case TYPE_ABP_TYPE:
                case TYPE_MORPHOLOGY_TYPE:
                    databaseName = getFullPathOfAppBundleCWPBibleDirectory() +"/"+ databaseName;// /documents/bibles/ASV.bbl
                    break;

                case TYPE_NOTES_TYPE:
                    databaseName = getNotesDirectory() +"/"+ databaseName;// /documents/UserNotes.note
                    break;

                case TYPE_COMMENTARY_TYPE:
                    databaseName = getFullPathOfAppBundleCommentaryDirectory() +"/"+ databaseName;// /documents/Commentaries/Gill.cmt
                    break;

                case TYPE_BOOK_TYPE:
#if STUDENT || STUDENT_OR_TEACHER
                    databaseName = getApplicationBundleBooksDirectory() +"/"+ databaseName;//
                   // [databaseName retain);
#endif
                    break;
            }
        }
    }


    openResult =SQliteDatabase::Open();


    if (openResult==YES) {
        if (getDbType() != lType) {
            setDbType( lType);
        }
        isOpen = YES;
        return SUCCESS;
    }
    else {
        isOpen = NO;
    }

    return FAIL;

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleDatabaseClass::HandleABPWord(RecordSet *rs, SHOW_THIS_INFO_TYPE *sti, SEARCH_INFO_TYPE *sit)
{
    QStringList TheWord;
    QString s1;
    QString s2;
    QString s3,s4;
    QStringList s;
    // NSMutableArray *sone;
    int index;

   // TheWord = [NSMutableArray arrayWithCapacity:7);


    TheWord.append( "");//0

    TheWord.append( SPAN_CLASS_UNIT);//1
                                          //NSLog(%ls",@ SPAN_CLASS_UNIT);


    TheWord.append(rs->IdxField(3).trimmed());//2 strongs
                                        //NSLog(%ls",rs->IdxField(3]);

    s1 =  SPAN_CLASS_GREEK +rs->IdxField(1)+ UN_SPAN +endOfLine();
    //s1 = [[[@ SPAN_CLASS_GREEK stringByAppendingString:rs->IdxField(1]] stringByAppendingString:@UN_SPAN] stringByAppendingString:endOfLine());
    TheWord.append(s1);//3
                           //NSLog(%ls",s1);


    TheWord.append(rs->IdxField(2).trimmed());//4 word field
                                        //NSLog(%ls",rs->IdxField(2]);

    TheWord.append( UN_SPAN);//5
                                  //NSLog(%ls",@ UN_SPAN);

    TheWord.append(" ");//6


    s1 = TheWord[4];

    if (Instr(s1, "lord")>0) {
        if (Instr(s1, "<span")>0) {
            s1 = s1.replace("lord","LORD");
            TheWord[4]=s1;
            //TheWord[4 ]=[s1 stringByReplacingOccurrencesOfString:"lord" withString:"LORD"]);
        }

        s1 = RemoveAll(TheWord[4],"<span", HTML_GENERIC_END_TAG);
        s3 = s1.replace( UN_SPAN,"");
    }
    else {
        if ( StringsAreEqual(s1," ") || s1.length()==0) {
            TheWord[4] = "asterisk" ;
        }

        s3 = TheWord[4].trimmed();
    }


    if (Instr(s3, HTML_SUPSTART)) {
        s3 = RemoveAll(s3, HTML_SUPSTART, HTML_SUPEND);
        s3 = s3.replace("[","");
        s3 = s3.replace("]","");
    }

    QString Language=rs->Field( ORIG_WORD_TYPE_COLUMN);

    if (Language.length()==0) {
        Language = "G";
    }

    if (Instr(TheWord[2], " ")){
        s =  mySplit(TheWord[2], " ");


        for (index=0; index <= s.count()-1; index++) {
            if (s[index]=="") {
                continue;
            }

            s3 = myReplace(s3," ","ZZZ");// wsm hide spaces
            //the new webkit can't stand ",","!", "'", or ";" in http url.
            s4 = myReplace(s3,";","");
            s4 = myReplace(s4,",","");
            s4 = myReplace(s4,"'","");            // if there is a "," in the text, remove it, it causes
            s4 = myReplace(s4,":","");            // if there is a "," in the text, remove it, it causes
            s4 = myReplace(s4,"!","");            // if there is a "!" in the text, remove it, it causes
            // the link to fail
            s3 = myReplace(s3,",","");
            s1 = "<a id='t"+QString::number(sti->IDNumber)+"' href=\"http://";

            s1 = s1 + Language + s[index] +"_t" +QString::number(sti->IDNumber) + "_"+ s3 +"\"" HTML_GENERIC_END_TAG +s[index]+ HTML_HREF_ENDS ;

            sti->IDNumber++;
            s[index ]=s1;
        }

        s1 =  SPAN_CLASS_STRONGS +s.join(" ")+ UN_SPAN +endOfLine();
        TheWord[2 ]=s1;
    }
    else {     
        s3 = myReplace(s3," ","ZZZ");// wsm hide spaces
        //s3 = myReplace(s3,"asterisk","*");

        //the new webkit can't stand ",", "'", or ";" in http url.
        s4 = myReplace(s3,";","");
        s4 = myReplace(s4,",","");
        s4 = myReplace(s4,"'","");
        s4 = myReplace(s4,":","");            // if there is a "," in the text, remove it, it causes
        s4 = myReplace(s4,"!","");            // if there is a "!" in the text, remove it, it causes
        s1 = "<a id='t"+QString::number(sti->IDNumber)+"' href=\"http://";
        s1 = s1+ Language +TheWord[2]  +"_t" +QString::number(sti->IDNumber) + "_"+ s4 +"\"" HTML_GENERIC_END_TAG +TheWord[2]+ HTML_HREF_ENDS UN_SPAN +endOfLine();
                         // , sti->IDNumber, Language,TheWord[2], sti->IDNumber, s3,TheWord[2],endOfLine());
        // s1 =[[[@ SPAN_CLASS_STRONGS stringByAppendingString:@ HTML_HREF_STARTQ] stringByAppendingString:G"] stringByAppendingString:TheWord[2] );
        // s1 = [[[s1 stringByAppendingString:+"] stringByAppendingString:s3] stringByAppendingString:@ QUOTE);
        // s1 = [[[s1 stringByAppendingString:@ HTML_GENERIC_END_TAG] stringByAppendingString:TheWord[2]] stringByAppendingString:@ HTML_HREF_ENDS);
        //  s1 = [[s1 stringByAppendingString:@ UN_SPAN] stringByAppendingString:endOfLine());
        TheWord[2 ]=SPAN_CLASS_STRONGS +s1;
    }

#if STUDENT_OR_TEACHER
    if (sti->enableEnglishDictionary) {
        if (s3.length() >0) {
           s = s3.split("ZZZ");// s3.split(" ");// [NSMutableArray arrayWithArray:[s3 componentsSeparatedByString:" "]);
        }
        else {
            s =  TheWord[4].split(" ");
        }

        if (s.count()) {
            for (index=0; index < s.count(); index++) {
                s2 =s[index];
                s2 = myReplace(s2,"'","");
                s2 = myReplace(s2,";","");
                s2 = myReplace(s2,",","");
                s2 = myReplace(s2,":","");            // if there is a "," in the text, remove it, it causes
                s2 = myReplace(s2,"!","");            // if there is a "!" in the text, remove it, it causes
                if (s2.length()) {
                    s2 = str.sprintf( "<a id='d%u' href=\"http://D_%ls_d%u" QUOTE HTML_GENERIC_END_TAG "%ls" HTML_HREF_ENDS
                                      ,dictionaryIDNumber,(wchar_t*) s2.utf16(),dictionaryIDNumber,(wchar_t*)s[index].utf16());
                    s[index]=s2;
                    dictionaryIDNumber++;
                }
            }
            TheWord[4]=s.join(" ");
        }
    }//for index
#endif

    if (sti->HilightSearch) {
        if (sit->SearchForAWord) {
            if (Instr(TheWord[4], sit->WordToSearchFor)>0 ) {
                // TheWord[0 ]=[TheWord[0] stringByAppendingString:HTML_BOLD_START]);
                s2 = "<a style=\"color:red\">";
                s2 = s2 +TheWord[4];
                TheWord[4 ]=s2 + HTML_HREF_END;
            }
        }
    }

    TheWord[4 ]= SPAN_CLASS_ENGLISH +myReplace(TheWord[4],"asterisk","*")+ UN_SPAN;
    // TheWord[4 ]=[[@SPAN_CLASS_ENGLISH stringByAppendingString:TheWord[4]] stringByAppendingString:@UN_SPAN]);

    // for (index=0; index<[TheWord count); index++) {
    //   NSLog(%ls",TheWord[index]);
    //}
    //s1 = str.sprintf(%ls%ls%ls%ls%ls%ls",TheWord[0],TheWord[1],TheWord[2],TheWord[3],TheWord[4],TheWord[5],TheWord[6]);

    s1= TheWord.join(" "); //crashes and burns sometimes
   // [TheWord release);
    //[DictionaryStart release);

    return s1;

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleDatabaseClass::HandleWord(RecordSet *rs, SHOW_THIS_INFO_TYPE *sti, SEARCH_INFO_TYPE *sit)
{
    QStringList TheWord;
    Q_UNUSED(sit);
    Q_UNUSED(sti);
    //TheWord = [NSMutableArray arrayWithCapacity:5);

    TheWord.append("");
    TheWord.append(rs->IdxField(1));// pre
    TheWord.append(rs->IdxField(WORD_FIELD));// word
    TheWord.append(rs->IdxField(3));// post
    TheWord.append("");


#if STUDENT_OR_TEACHER
     // wsm, don't show underline if preferences folder not set
    // if we found the media files, then can underline words for
    // bible picture hint
    if (Preferences->getEnableDisplayPictures() && gMediaModuleDatabase) {
        if (rs->IdxField(WORD_FORMAT_FIELD).toInt()) {
            TheWord[0 ]=HTML_UNDERLINE_START;
            TheWord[4 ]=HTML_UNDERLINE_END;
        }
        /*
    if (Preferences->MediaFolder &&  ShowStrongs ){// then
     if rs.IdxField(WORD_FORMAT_FIELD).IntegerValue = 1 then
     TheWord(0)= HTML_UNDERLINE_START //TheWord +
     TheWord(4)= HTML_UNDERLINE_END
     end if
     end if
     */

    }
#endif

    return TheWord.join("");

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int BibleDatabaseClass:: GetFootnoteIndex()
{
    return FootNote++;
    //return [[NSNumber numberWithInt:FootNote] stringValue);
}

// the following number rotates so we can tag each dictionary item

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleDatabaseClass::ParseForDictionary( QString newWord,/* withPreviouslyFoundType*/int * foundType)
{
    QStringList array;
    QStringList array2;
    int index, arrayCount;
    QString w, Word;
    // QString BibleDatabaseClass::DictionarySt = <a id=\"d%ud\" style=" QUOTE "color:black" QUOTE "href="QUOTE HTML_DICT_INDICATOR"+";
    QString CustomMenu="";

    w = myReplace(newWord,"<"," <");

    array = mySplit(w," "); // divide the paragraph into words
    arrayCount = array.count();
    for (index=0; index< arrayCount; index++) {
        w = array[index];

        if (w.length()==0) {
            continue;
        }

        if (*foundType != FoundNormal) {
            goto skip1;// process without touching until end tag reached.
        }

        if (Instr(w,"<span")) {
            *foundType = FoundSPAN;
            goto skip1;
        }

        if (Instr(w, "<img")) {
            *foundType = FoundIMG;
            goto skip1;
        }

        if (StringsAreEqual(w, "<font")) {
            *foundType = FoundFONT;
            goto skip1;
        }
        if (StringsAreEqual(w, "<table") || StringsAreEqual(w, "<TABLE")) {
            *foundType = FoundTABLE;
            goto skip1;
        }

        if (StringsAreEqual(w,"<br")){
            *foundType = FoundBR;
            goto skip1;
        }
        if (Instr(w, "<tr>") || Instr(w,"</tr>") ){
            *foundType = FoundTAG ;
            goto skip1;
        }

        if (Instr(w, "<td>")|| Instr(w, "<TD>")) {
            *foundType = FoundTAG;
            goto skip1;
        }
        if (Instr(w, "<a")) {
            *foundType = FoundXREF;
            goto skip1;
        }

        if (Instr(w, "<OL")) {
            *foundType = FoundOL;
            goto skip1;
        }


    skip1:

        switch (*foundType) {
            case FoundSPAN:
                array2.append(w);
                if (Instr(w,"</span>")) {
                    *foundType=FoundNormal;
                }
                break;

            case FoundOL:
                array2.append(w);
                if (Instr(w, ">")) {
                    *foundType = FoundNormal;
                }
                break;


            case FoundBR:
                array2.append(w);
                if (Instr(w, "/>")) {
                    *foundType = FoundNormal;
                }
                break;

            case FoundXREF:
                array2.append(w);
                if (Instr(w, "</a>")) {
                    *foundType = FoundNormal;
                }
                break;


            case FoundTAG:
                array2.append(w);
                *foundType= FoundNormal;
                break;

            case FoundFONT :
                array2.append(w);
                if (Instr(w, "</font>")) {
                    *foundType= FoundNormal;
                }
                break;

            case FoundTABLE:
                array2.append(w);
                if (Instr(w, "</table>") || Instr(w, "</TABLE>")) {
                    *foundType= FoundNormal;
                }
                break;

            case FoundIMG:
                array2.append(w);
                if (Instr(w, "png\">")) {
                    *foundType = FoundNormal;
                }
                break;

            case FoundTR:
                array2.append(w);
                if (Instr(w, ">")) {
                    *foundType = FoundNormal;
                }
#if FALLTHROUGH_ALLOWED == 1
[[fallthrough]];
#endif
            default:
            case FoundNormal:
                if (Instr(w, "\"")) {
                    Word = str.sprintf( "<a id=\"d%u\" style=" QUOTE "color:black" QUOTE " href=" QUOTE HTML_DICT_INDICATOR"_%ls_d%u\"%ls%s%ls%s"
                                        ,dictionaryIDNumber,(wchar_t *)myReplace(w,"\"","").utf16(),dictionaryIDNumber,(wchar_t *)CustomMenu.utf16(),
                                        HTML_GENERIC_END_TAG,(wchar_t *)w.utf16(), HTML_HREF_ENDS);
                } else {
                    Word = str.sprintf( "<a id=\"d%u\" style=" QUOTE "color:black" QUOTE " href=" QUOTE HTML_DICT_INDICATOR"_%ls_d%u\"%ls%s%ls%s"
                                        ,dictionaryIDNumber,(wchar_t *)w.utf16(),dictionaryIDNumber,(wchar_t *)CustomMenu.utf16(), HTML_GENERIC_END_TAG,(wchar_t *)w.utf16(), HTML_HREF_ENDS);
                }
                array2.append(Word);
                dictionaryIDNumber++;
                break;
        }
    }



    return array2.join(" ");

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleDatabaseClass::GetVerse(SHOW_THIS_INFO_TYPE *STI,/* SearchParmsStructPointer*/SEARCH_INFO_TYPE *SIT)
{
    RecordSet   *rs;
    QString CustomMenu="";
    //QString DictionarySt="";
    QStringList words;
    QString Word;
    QString tempword;
    QString s2;
    QString Query;
#if STUDENT_OR_TEACHER || STUDENT || STUDENT
    int         index;
    int arrayCount;
    QStringList PreAArray;
    int foundType = FoundNormal;
#endif


    if (!isOpen) {
        Open();
    }

    //                              Select * from BookTableName where Chapter = 1 and Verse =1 QueryEnd
    Query =QueryBegin+ SIT->BookTableName+ " where " CHAPTER_COLUMN " = "+QString::number(SIT->ChapterNumber)+" and " VERSE_COLUMN " = " +
            QString::number(SIT->VerseNumber)+" "+QueryEnd;

    rs =SQLSelect(Query);  // the statement to run for the select.

    if (rs == nullptr) {
        return  " ";
    }

    switch (getDbType()) {

        case TYPE_ABP_TYPE:
            while (!rs->atEOF()) {
                words.append(HandleABPWord(rs, STI, SIT));
                rs->MoveNext();
            }
            rs->done();// release sqlite table NOW
            delete rs;//[rs release);
            return words.join(" ");

            break;


        case TYPE_COMMENTARY_TYPE: {
#if STUDENT_OR_TEACHER || STUDENT
            QString w;
            QString w2;
            QString w3;
            int subIndex;
#endif
            //BOOL foundA=NO;

            while (!rs->atEOF()) {

                // we must handle the dictionary
                Word = HandleWord(rs, STI, SIT );
#if STUDENT_OR_TEACHER || STUDENT
                if (STI->enableEnglishDictionary) {

                    if (Instr(Word, "<a")) {
                        PreAArray = mySplit(Word,"<a");
                        arrayCount=PreAArray.count();

                        PreAArray[0 ]=ParseForDictionary(PreAArray[0],/* withPreviouslyFoundType:*/&foundType );//[self ParseForDictionary:PreAArray[0]  withPreviouslyFoundType:&foundType ]);

                        for (index=1; index< arrayCount; index++) {
                            w = PreAArray[index];
                            if (Instr(w, "</a>")) {
                                subIndex =Instr(w, "</a>")+3;
                                w2 = w.left(subIndex);
                                w3 = w.right(subIndex);
                                w = "<a "+ w2+ ParseForDictionary(w3,/* withPreviouslyFoundType:*/&foundType );

                                PreAArray[index ]=w;
                            } else {
                                PreAArray[index ]="<a "+PreAArray[index];
                            }
                        }
                        Word = PreAArray.join("");
                        words.append(Word);
                    } else {
                        words.append(ParseForDictionary(Word,/* withPreviouslyFoundType:*/&foundType ));// the word is fancied up already
                    }
                } else
#endif
                {
                    words.append(Word);
                }

                rs->MoveNext();
            }

            rs->done();// release sqlite table NOW
            delete rs;//[rs release);
           return words.join(" ");
        }
            break;



        default:
        case TYPE_BBL_TYPE:
        case TYPE_MORPHOLOGY_TYPE:

            if (STI->ShowStrongs) {
                goto DoShowStrongs;
            }
#if FALLTHROUGH_ALLOWED == 1
[[fallthrough]];
#endif

        case TYPE_NOTES_TYPE:

#if STUDENT_OR_TEACHER || STUDENT
            if (STI->enableEnglishDictionary) {

                while (!rs->atEOF()) { //                1  2   3 4 5 6    1            2              3           4                 5                         6
                    Word = HandleWord(rs, STI, SIT);

                    words.append(ParseForDictionary(Word,/* withPreviouslyFoundType:*/&foundType ));
                    rs->MoveNext();
                }

                //  rs->done();
                rs->done();// release sqlite table NOW
                delete rs;//[rs release);
                return words.join(" ");
            }
#endif

            while (!rs->atEOF()) {
                words.append(HandleWord(rs, STI, SIT));
                rs->MoveNext();
            }

            rs->done();// release sqlite table NOW
            delete rs;//[rs release);
            return words.join(" ");

            break;

    }




DoShowStrongs:


    while (!rs->atEOF()) {


        if (rs->IdxField(5).toInt() != 0) {
            static short footNoteID;
            //-----------------------------------------------------------
            // this could be a footnote.  It might be with a null tempword
            // if the letter is an 'F' then use that to index into the footnote database
            // if it exists, then we will append the footnote to the word on the right
            // if the word is blank, is a stand alone footnote (which will be most
            // of the footnotes)
            //-----------------------------------------------------------
            if ( StringsAreEqual(rs->IdxField(4).trimmed() ,  FOOTNOTE_INDICATOR)) {
                tempword = HandleWord(rs, STI, SIT);

                if (tempword.length()==0) {
                    tempword = str.sprintf( "<a id='F%u' href=\"http://" FOOTNOTE_INDICATOR "-%d-%d_%d_%d_%ls_F%u" HTML_GENERIC_END_TAGQ HTML_SUPSTART "%d" HTML_SUPEND HTML_HREF_ENDS
                                            ,footNoteID, BibleIndexNumber,SIT->BookNumber,SIT->ChapterNumber,SIT->VerseNumber,(wchar_t *)rs->IdxField(6).utf16(),footNoteID,GetFootnoteIndex());

                    words.append(tempword);
                }
                else {
#if STUDENT_OR_TEACHER || STUDENT
                    QString tword;

                    if (STI->enableEnglishDictionary) {
                        tword = ParseForDictionary(tempword,/* withPreviouslyFoundType:*/&foundType );
                        tempword = str.sprintf("%ls" "<a id='F%u' href=\"http://" FOOTNOTE_INDICATOR "-%d-%d_%d_%d_%ls_F%u" HTML_GENERIC_END_TAGQ HTML_SUPSTART  "%d" HTML_SUPEND HTML_HREF_ENDS
                                               ,(wchar_t *)tword.utf16(),footNoteID, BibleIndexNumber,SIT->BookNumber,SIT->ChapterNumber,SIT->VerseNumber,(wchar_t *)rs->IdxField(6).utf16(), footNoteID,  GetFootnoteIndex());
                    }else
#endif
                    {
                        tempword = str.sprintf("%ls" "<a id='F%u' href=\"http://" FOOTNOTE_INDICATOR "-%d-%d_%d_%d_%ls_F%u" HTML_GENERIC_END_TAGQ HTML_SUPSTART  "%d" HTML_SUPEND HTML_HREF_ENDS
                                ,(wchar_t *)tempword.utf16(),footNoteID, BibleIndexNumber,SIT->BookNumber,SIT->ChapterNumber,SIT->VerseNumber,(wchar_t *)rs->IdxField(6).utf16(), footNoteID, GetFootnoteIndex());
                    }



                    words.append(tempword);
                }// tempword length
                footNoteID++;
            }// if footnote indicator
             //--------------------
            else {// not footnote
                if (STI->BlendStrongsIntoWord) {
                    tempword = HandleWord(rs, STI, SIT);

                    if (tempword.length()==0) {
                        tempword = "<sup>*</sup>";
                    }

                    Query =rs->IdxField(WORD_FIELD);
                    // if the word begins with an &#, then is an html extra type of word.
                    // if it is &quot;, then strip that before testing
                    if (Query.length()>0 && Query.at(0) =='&'){
                        Query.remove("&quot;",Qt::CaseInsensitive);
                    }
                    //change the strongs to not include that part,
                    // since the word can't be looke up in the dictionary. use an xxxx instead
                    if (Query.length()>0 && Query.at(0) =='&') {
                        tempword = str.sprintf( HTML_HREF_STARTQ "%ls%ls_xxxx\"%ls" HTML_GENERIC_END_TAG "%ls" HTML_HREF_ENDS
                                                ,(wchar_t*)rs->IdxField(4).trimmed().utf16(),(wchar_t*)rs->IdxField(5).utf16(),(wchar_t*)CustomMenu.utf16(),(wchar_t*)tempword.utf16());
                    }else {
#if STUDENT_OR_TEACHER || STUDENT
                        QString s3;
                        if (STI->enableEnglishDictionary) {
                            if (Instr(tempword, "<a")==0 && Instr(tempword, ">")==0) {
                                goto NormalAdd;
                            }

                            // remove some things from the query'
                            s3 = Query;
                            s3.remove("&quot;");
                            s3.remove("'");
                            s3.remove(",");
                            s3.remove(";");
                            s3.remove(":");
                            s3.remove("!");
                            s3.remove(".");
                            tempword = str.sprintf( "<a id='t%d' href=\"http://"  "%ls%ls_t%u_%ls\"%ls" HTML_GENERIC_END_TAG "%ls" HTML_HREF_ENDS
                                                    ,STI->IDNumber,(wchar_t *)rs->IdxField(4).trimmed().utf16(),(wchar_t *)rs->IdxField(5).utf16(),STI->IDNumber,
                                                    (wchar_t *)s3.utf16(),(wchar_t *)CustomMenu.utf16(),(wchar_t *)tempword.utf16());
                            //tempword = str.sprintf(%ls%ls%ls",rs->IdxField(PRE_FIELD],s2,rs->IdxField(POST_FIELD]);

                        } else
#endif
                        {
                        NormalAdd:
                            QString s3;

                            s3 = rs->IdxField(WORD_FIELD);
                            s3.remove("&quot;");
                            s3.remove("'");
                            s3.remove(",");
                            s3.remove(";");
                            s3.remove(":");
                            s3.remove("!");
                            s3.remove(".");
//                            s3 = myReplace(s3,"'","");
//                            s3 = myReplace(s3,",","");
//                            s3 = myReplace(s3,";","");
//                            s3 = myReplace(s3,":","");
                            // add +WORD to allow dictionary to work with strongs
                            //                                       1 2 3  4   5 6 7 8     1                 2              3                4                     5               6                7          8
                            // NOTE: the t%d is for the tag number, for the ID of the word
                            tempword = "<a id='t"+QString::number(STI->IDNumber)+"' href=\"http://"+rs->IdxField(4).trimmed()+rs->IdxField(5) +"_t"+
                                    QString::number(STI->IDNumber)+"_"+s3+"\""+HTML_GENERIC_END_TAG+CustomMenu+tempword+HTML_HREF_ENDS;

                        }
                    }

                    STI->IDNumber++; // point to the next id.

                    words.append(tempword);
                }
                // dont blend strongs into words \/
                else {
                    tempword = HandleWord(rs, STI, SIT);
                    if (Instr(tempword, "<a")) {
                        words.append(tempword);
                    } else {
                        words.append(ParseForDictionary(tempword,/* withPreviouslyFoundType:*/&foundType ));// the word is fancied up already
                    }

                    QString s3;

                    s3 = rs->IdxField(WORD_FIELD);
                    s3.remove("'");
                    s3.remove(",");
                    s3.remove(";");
                    s3.remove(":");
                    s3.remove("!");
                    s3.remove(".");
                    tempword ="<a id='t"+QString::number(STI->IDNumber)+"' href=\"http://"+rs->IdxField(4).trimmed()+rs->IdxField(5) +"_t"+
                            QString::number(STI->IDNumber)+"_"+s3+"\""+HTML_GENERIC_END_TAG+CustomMenu;

                    tempword = tempword+HTML_LT+ rs->IdxField(4).trimmed()+rs->IdxField(5)+ HTML_GT HTML_HREF_ENDS;
                    words.append(tempword);
                }// blend strongs into word

            }// footnote indicator
        }// idxfield==0
        else {
#if STUDENT_OR_TEACHER || STUDENT
            if (STI->enableEnglishDictionary) {
                tempword =rs->IdxField(WORD_FIELD);
                if (Instr(tempword, "<") || Instr(tempword, ">")) {
                    goto NoDictionary;
                }
                tempword = HandleWord(rs, STI, SIT);
                if (Instr(tempword, "<a")) {
                    words.append(tempword);
                } else {
                    words.append(ParseForDictionary(tempword,/* withPreviouslyFoundType:*/&foundType ));// the word is fancied up already
                }


            }
            else
#endif
            {
            NoDictionary:
                words.append(HandleWord(rs, STI, SIT) + " ");
            }

        }

        rs->MoveNext();
    }

    rs->done();// release sqlite table NOW
    delete rs;
                          // [rs release);
   // words.append("\n\r");
    return words.join(" ");
    //return ";
}
// ------------------------------------------------------------------------
// sit fields
// BookTablename
// ChapterNumber
// VerseNumber
// ------------------------------------------------------------------------
QString BibleDatabaseClass::GetPlainVerse(SHOW_THIS_INFO_TYPE* STI, /*SearchParmsStructPointer*/SEARCH_INFO_TYPE* SIT, /* Type*/int BibleType)
{
    RecordSet *rs;
    QStringList  words;
    QString s2;

    QString Query;
    Q_UNUSED(BibleType);
    Q_UNUSED(STI);

    //                              Select * from BookTableName where Chapter = 1 and Verse =1 QueryEnd
    Query =SIT->BookTableName+"  where " CHAPTER_COLUMN " = "+QString::number( SIT->ChapterNumber)+" and " VERSE_COLUMN " = "+QString::number( SIT->VerseNumber);
    Query = QueryBegin + Query + QueryEnd;
    rs =SQLSelect(Query);  // the statement to run for the select.

    if (rs == nullptr) {
        return  "";
    }

    if (rs->NumberOfRows==0 || rs->NumberOfColumns==0) {
        return  "";
    }


    switch (getDbType()) {
        default:
        case TYPE_BBL_TYPE:
        case TYPE_NOTES_TYPE:
        case TYPE_MORPHOLOGY_TYPE:

        while (!rs->atEOF()) {
                s2 = str.sprintf("%ls%ls%ls",(wchar_t *)rs->IdxField(1).utf16(),(wchar_t *)rs->IdxField(WORD_FIELD).utf16(),(wchar_t *)rs->IdxField(3).utf16());
                words.append(s2);

                rs->MoveNext();
            }
            break;

        case TYPE_ABP_TYPE:
 //       DoABPBible: // label for this kind of bible

            while (!rs->atEOF()) {
                words.append(rs->IdxField(WORD_FIELD));
                rs->MoveNext();
            }
            break;

    }//switch


    rs->done();// release sqlite table NOW
    delete rs;

    s2= words.join(" ");// componentsJoinedByString: ");

    static const char *RemoveItems[] = {
  (const char*)"</span>",
   (const char*)      HTML_ITALIC_END,
   (const char*)     HTML_ITALIC_START,
  (const char*)"<I>",(const char*)"</I>",
   (const char*)     HTML_BRStr,
  (const char*)"<cite>",
  (const char*)"</cite>",
  (const char*)"</a>",
  (const char*)"</font>",
  (const char*)"<B>",
  (const char*)"</B>",
  (const char*)"&nbsp;",
  (const char*)"</b>",
  (const char*)"<b>",
  (const char*)"<br/>",
  (const char*)"<p>",
  (const char*)"</p>",
  (const char*)"<center>",
  (const char*)"<CENTER>",
  (const char*)"</DIV>",
  (const char*)"</div>",
        NULL
    };


    // following must occur in pairs
    static const char *RemoveAllItems[] = {
    (const char*)     HTML_SUPSTART,(const char*)HTML_SUPEND,
  (const char*)"<a",(const char*)">",
  (const char*)"<font",(const char*)">",
  (const char*)"<span",(const char*)">",
        NULL,NULL
    };

    int index=0;
    QString s;

    while (RemoveAllItems[index]) {
        s =RemoveAllItems[index++];
        Query =RemoveAllItems[index++];
        while (Instr(s2, s)) {
            s2 = RemoveAll(s2,s, Query);
        }
    }

    index=0;
    while (RemoveItems[index]!= NULL) {
        Query =RemoveItems[index];
        if (Instr(s2,Query)) {
            s2 = myReplace(s2,Query,"");
        }
        index++;
    }

    return s2;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDatabaseClass:: ResetFootNote()
{
    FootNote =1;
}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleDatabaseClass::getFootNoteTextFromBook(int Book,int Chapter, int Verse,int SeqNumber)
{
    RecordSet *rs;
    QString Query;
    QString FootNoteText;
    BOOL StripFormatting = NO;

    // this must be a "modern" database
    Query = str.sprintf( "select " WORD_COLUMN " from " FOOTNOTE_TABLENAME " where " CHAPTER_COLUMN "=%d and " VERSE_COLUMN "=%d and " BOOK_NUMBER_COLUMN  " = %d and " WORD_SEQ_COLUMN "=%d limit 1"
                         , Chapter,Verse,Book,SeqNumber);

    rs = SQLSelect(Query);

    if (rs != Nil ) {//then

        //----------------------------------------------------------------
        // IF LINUX, then font style does not work
        //add <font> tag into the  text
        //------------------------------------------------------------------
        FootNoteText = rs->IdxField(1);

        if (StripFormatting){// Then
            return FootNoteText;
        }else{
            // do it with style
            FootNoteText = HTML_STANDARD_HEADER HTML_NO_UNDERLINE_STYLE "<p style=" QUOTE "font-size:"+QString::number(Preferences->getFontSize()-4)+"pt" QUOTE HTML_GENERIC_END_TAG+FootNoteText+ "</p>" HTML_STANDARD_END;
        }
        return  FootNoteText;

    }//end if


    return ""; // empty string
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleDatabaseClass::getCopyrightText()
{
    RecordSet *rs;
    QString Query;
    QString CopyrightText;
    BOOL StripFormatting = NO;

    // this must be a "modern" database
    Query = "select " COPYRIGHT_INFO_COLUMN " from " COPYRIGHT_TABLE;

    rs = SQLSelect(Query);

    if (rs != nullptr ) {//then

        //----------------------------------------------------------------
        // IF LINUX, then font style does not work
        //add <font> tag into the  text
        //------------------------------------------------------------------
        CopyrightText = rs->IdxField(1);

        if (StripFormatting){// Then
            return CopyrightText;
        }else{
            // do it with style
            CopyrightText =  HTML_STANDARD_HEADER HTML_NO_UNDERLINE_STYLE "<p style=" QUOTE "font-size:"+QString::number(Preferences->getFontSize()-4)+"pt" QUOTE HTML_GENERIC_END_TAG+CopyrightText+ "</p>" HTML_STANDARD_END;
        }

        CopyrightText = myReplace(CopyrightText,"\r", HTML_BRStr);

        return  CopyrightText;

    }//end if


    return ""; // empty string

}
int catchthis;
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDatabaseClass::InsertWordIntoDatabase(BibleBookInfoStructType *bbis , /*Chapter*/ int chapter , /*Verse*/ int verse ,int wordSequence, QString word, QString origWordLanguage, QString dictionaryEntry, QString preWord,  QString postWord)
{

    QString Query;
    QString is,w;
    w = word;
    QChar c='\'';
    w.replace(c,"''",Qt::CaseInsensitive);
    //if (chapter==1 && verse==21)
    //    catchthis=true;
    //w = myReplace(word,"'","''");
    preWord = myReplace(preWord,"'","''");
   // preWord = myReplace(preWord,"\"","&quot;");
    postWord = myReplace(postWord,"'","''");
  //  postWord = myReplace(postWord,"\"","&quot;");

    Query ="insert into "+bbis->TableName+"(" CHAPTER_COLUMN "," VERSE_COLUMN "," WORD_SEQ_COLUMN "," WORD_COLUMN "," ORIG_WORD_TYPE_COLUMN "," DICTIONARY_ENTRY_COLUMN "," WORD_PRE ","  WORD_POST ") values (";
    Query = Query + "'"+ QString::number(chapter)+"', '"+  QString::number(verse)+"', '";
    is=   QString::number(wordSequence)+"', "+ "'"+w+"', '"+origWordLanguage+ "' , '" + dictionaryEntry +  "' , '" + preWord+  "', '"+postWord+"' )";
    Query = Query + is;

    SQLExecute(Query);


}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
/*void BibleDatabaseClass::insertSequenceNumberGapAtIndex(SEARCH_INFO_TYPE *sit, int index)
{
    QString Query,str;

    Query = QString("Update ")+QString(sit->BookTableName)+\
            QString(" set " WORD_SEQ_COLUMN "='") +\
            QString::number(sit->WordSequenceNumber+1) +\
            QString("' where " CHAPTER_COLUMN) ;
    str = str.sprintf("=%d and " VERSE_COLUMN "=%d and " WORD_SEQ_COLUMN"=%d  "
              ,sit->ChapterNumber, sit->VerseNumber, sit->WordSequenceNumber);
    Query = Query + str;



    SQLExecute(Query);


}*/
void BibleDatabaseClass::updateWordSeqNumber(SEARCH_INFO_TYPE *sit, int newSeqNumber){
    QString Query,str;

    Query = QString("Update ")+QString(sit->BookTableName)+\
            QString(" set " WORD_SEQ_COLUMN "='") +\
            QString::number(newSeqNumber) +\
            QString("' where " CHAPTER_COLUMN) ;
    str = str.sprintf("=%d and " VERSE_COLUMN "=%d and " WORD_SEQ_COLUMN"=%d  "
              ,sit->ChapterNumber, sit->VerseNumber, sit->WordSequenceNumber);
    Query = Query + str;



    SQLExecute(Query);
}
