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
//  dictionarydb.m
//  CWP-Student
//
//  Created by WADE MAXFIELD on 1/21/11.
//  Copyright 2011 Wade Maxfield All rights reserved.
//converted

#include "../shared/HTMLModule.h"
#include "../shared/misclibrary.h"
#include "../shared/bibleinfo.h"
#include "dictionarydb.h"
#include <QObject>
#include <QStringList>

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
dictionaryDB::dictionaryDB() : BibleDatabaseClass()
{
    iStartDataIndex = 0;
    iCurrentDataIndex=0;
    dictionaryType = 0;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
dictionaryDB::dictionaryDB(/*initWithDBName:*/QString DictionaryDBName,
                               /* andType:*/int dtype,
                                 /*andDictionaryType:*/int dtypeOfDictionary)
 : BibleDatabaseClass()
{
    initWithDBName(DictionaryDBName , /*andType*/ dtype  , /*andDictionaryType*/  dtypeOfDictionary);

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
dictionaryDB::~dictionaryDB()
{
       Close();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString dictionaryDB::preProcessWord(QString word)
{
    QString sWord = word;
    int index;
    QString Query;

    while (Instr(sWord, "%3C")) {
        sWord = RemoveAll(sWord, "%3C", "%3E");
    }


    static char *RemoveItems[] = {
   (char*)"\"",
   (char*)"(",
   (char*)")",
   (char*)",",
   (char*)".",
   (char*)":",
   (char*)"?",
   (char*)"!",
   (char*)";",
   (char*)"%5B",
   (char*)"%5D",
   (char*)"<i>",
   (char*)"</i>",
   (char*)"%3CI",
   (char*)"%3E",
        NULL
    };



    index=0;
    while (RemoveItems[index]!= NULL) {
        Query =RemoveItems[index];
        if (Instr(sWord,Query)) {
            sWord = myReplace(sWord,Query,"");
        }
        index++;
    }
    return sWord;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void dictionaryDB::initWithDBName(QString dbName , /*andType*/ int Type  , /*andDictionaryType*/ int DicType)
{
    Q_UNUSED(Type);
    QString str;

        databaseName = dbName;
        openWithType(DicType);// open the database

        if (!isOpen)
            return;

        QString Query = "Select * from " DICTIONARY_TABLE " limit 1" ;

        /*rs=*/  SQLSelect(Query);

        if (Error()) {
#if DEBUG_VERSION
            MsgBox(str.sprintf("initWithDBName:%ls, %",dbName, ErrorMessage());
#endif
            isOpen = NO; // we failed
        } else {
           // if (getDbType() == TYPE_BBL_TYPE) {
           //     setDbType( Type);
           // }
        }

    dictionaryType = DicType;

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL dictionaryDB:: isWordInBibleDictionary( QString word)
{
    QString sWord;
    QString Query;
    RecordSet *result;
    QString str;

    sWord = preProcessWord(word);

    Query = "select " DICTIONARY_ID_COLUMN ", " DICTIONARY_DEFINITION ", " DICTIONARY_WORD_INFO " from " DICTIONARY_TABLE " where " DICTIONARY_WORD " LIKE " QUOTE +sWord+ "%%" QUOTE;

    result = SQLSelect(Query);


    if (result != Nil ){ // then

        if ( result->NumberOfRows == 0) {
            return NO;
        }

        return YES;
    }

    return  NO;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString dictionaryDB:: getDefinitionsForWord(QString word, /*limitedToNumberOfDefinitions*/ int NumberOfDefinitions)
{
    if (dictionaryType == DICTIONARY_TYPE_ENGLISH_DICTIONARY) {
        return getEnglishDictionaryDefinitionsForWord(word , /*limitedToNumberOfDefinitions:*/NumberOfDefinitions);
    } else {
        return getBibleDictionaryDefinitionsForWord(word , /*limitedToNumberOfDefinitions:*/NumberOfDefinitions);
    }

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString dictionaryDB:: getBibleDictionaryDefinitionsForWord(QString word , /*limitedToNumberOfDefinitions*/ int NumberOfDefinitions)
{
    RecordSet *result;

    QString  Query;// as String

    QString sWord;



    QChar c,d;


    sWord = preProcessWord(word);

RetrySearch:

    //CREATE TABLE Dictionary (ID integer NOT NULL PRIMARY KEY,Word varchar, info varchar, Def varchar);
    //#define  DICTIONARY_WORD "Word"
    //#define  DICTIONARY_WORD_INFO "info"

    //#define  DICTIONARY_DEFINITION "Def"

    //!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!!!!
    // the following requires the native order of the database to
    // be alphabetical, so don't delete and re-insert fields in the database, always recreate it

    // use LIKE rather than = to get case insensitive match
    Query = "select " DICTIONARY_ID_COLUMN ", " DICTIONARY_DEFINITION ", " DICTIONARY_WORD_INFO " from " DICTIONARY_TABLE " where " DICTIONARY_WORD " LIKE " QUOTE +sWord+"%%" QUOTE;

    result = SQLSelect(Query);


    if (result != Nil ){ // then

        if ( result->NumberOfRows == 0) {
            c = (sWord.mid(sWord.length()-1,1)).at(0);
            d = (sWord.mid(sWord.length()-2,1)).at(0);

            if (c=='d' && d=='e') {
                sWord = sWord.left(sWord.length()-2);//up to but not including index
                goto RetrySearch;
            }

            if (c =='s') {
                sWord = sWord.left(sWord.length()-1);// up to but not including index value
                goto RetrySearch;
            }
        }

        int did,did2;

        did = result->IdxField(1).toInt();// the id field

        // on the ipad, we tried to show the word in the middle of the screen
        // show it at the beginning for other platforms
        //did2 = did -NumberOfDefinitions/2;
        did2 = did;

        if (did2 < 1) {
            did2 = 1;
            NumberOfDefinitions = 1; // fix this since we are going to be at first definition
        }

        if (did2 <= 3) {
            if (did >= 4) {
                did2 = 4;
            }

        }



        return getBibleDictionaryDefinitionsForIndex(did2, /* andNumberOfDefinitions:*/NumberOfDefinitions , /*thenScrollToFieldID*/did);
    }


//failSearch:
    return Query.sprintf( "<i>No definition found for </i>\"<b>%ls\"!</b>",(wchar_t*) sWord.utf16());

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString dictionaryDB:: getEnglishDictionaryDefinitionsForWord(QString word , /*limitedToNumberOfDefinitions*/ int NumberOfDefinitions)
{
    // usage  Variable = GetVariable(TableName,DB,result,Category,VariableName
    // if result = false{ // then bail
    RecordSet *result;
    //dim k as integer
    QString  Query;// as String
    QString  myVar;// as Variant
    // myDB As REALSQLDatabase
    QStringList DS;// As String
    QString Phonic;
    //  int index;
    QString sWord;
    QString str;


    QChar c=0,d=0;//,e=0;

    //c = [sWord characterAtIndex:sWord.length()-1);
    sWord = preProcessWord(word);

    if (sWord.length()==0)
        return "";


    int compoundTryNumber=0;
    QString lastWord;

RetrySearch:

    // use LIKE rather than = to get case insensitive match
    Query = str.sprintf("select " DICTIONARY_DEFINITION "," DICTIONARY_WORD_INFO " from " DICTIONARY_TABLE " where " DICTIONARY_WORD " LIKE " QUOTE "%ls" QUOTE, (wchar_t*)sWord.utf16());

    result = SQLSelect(Query);


    if (result != Nil ){ // then

        if ( result->NumberOfRows == 0) {
            if (sWord.length())
                c = (sWord.mid(sWord.length()-1,1)).at(0);
            if (sWord.length()>2)
                d = (sWord.mid(sWord.length()-2,1)).at(0);


            if (c=='d' && d=='e') {
                sWord = sWord.left(sWord.length()-2);
                goto RetrySearch;
            }

            if (c =='s') {
                sWord = sWord.left(sWord.length()-1);
                goto RetrySearch;
            }

            // eth
            if (c=='h' && d=='t' && compoundTryNumber==0){
                compoundTryNumber =1;
            }

            switch (compoundTryNumber) {
                case 0:
                // nothing here, move on
                break;

                case 1:
                    compoundTryNumber++;
                    lastWord = sWord;
                    sWord = sWord.left(sWord.length()-2);
                    goto RetrySearch;
                break;

                case 2:
                    sWord = lastWord;
                    compoundTryNumber=0;// don't try again
                    sWord = sWord.left(sWord.length()-3);
                    goto RetrySearch;
                break;
            }
        }




        int foundType=0;

        while (!result->atEOF()) {

            NumberOfDefinitions = NumberOfDefinitions-1;


            if (!result->atEOF() ){ // then exit // bail if the records give out

                if (result->FieldCount() != 0 ){ // then
                    myVar = result->IdxField(1);//Field*/ @ DICTIONARY_DEFINITION));
                    myVar = ParseForDictionary(myVar,/* withPreviouslyFoundType:*/&foundType);

                    Phonic = result->IdxField(2); //Field*/ @ DICTIONARY_WORD_INFO) );
                    //Phonic = replace(Phonic,@ HTML_ITALIC_START,"");
                    //Phonic = replace(Phonic,@ HTML_ITALIC_END,"");
                    DS.append("<b>"+sWord+"</b>&nbsp;&nbsp;("+Phonic+")&nbsp;&nbsp;"+myVar);
                   // DS.append(str.sprintf("<b>%ls</b>&nbsp;&nbsp;(%ls)&nbsp;&nbsp;%ls"
                   //                       , (wchar_t*)sWord.utf16(), (wchar_t*)Phonic.utf16(),(wchar_t*)myVar.utf16()));

                } // end if

                if (NumberOfDefinitions == 0){ // then exit  // bail from this loop
                   delete result;
                    return DS.join( HTML_BRStr);
                } // end if

            }

            result->MoveNext();
        }

       delete result;
    }

    if (DS.count()>0) {
        return DS.join( HTML_BRStr);
    }
    return  QObject::tr("<i>No definition found for </i>\"<b>")+sWord+QObject::tr("\"!</b>");
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString dictionaryDB:: getBibleDictionaryDefinitionsForIndex(int dataIndex, /* andNumberOfDefinitions*/ int NumberOfDefinitions ,
                                                             /*thenScrollToFieldID*/ int scrollToFieldID)
{
    RecordSet *result;

    QString  Query;// as String
    QString  myVar;// as Variant

    QStringList DS;// As String
    QString Phonic;
    QString sWord;
    QString str;

    int foundType = FoundNormal;

#define PRE_FIELD 1
#define WORD_FIELD 2
#define POST_FIELD 3
#define LANG_FIELD 4
#define STRONG_FIELD 5


   // DS = [NSMutableArray arrayWithCapacity:NumberOfDefinitions);

    if (dataIndex<=0) {
        dataIndex=1;
        NumberOfDefinitions = 3; // if we go back to cover page, make sure we only show cover
    }


     iStartDataIndex = dataIndex;// save beginning

    // pick up the ID and select the next NumItems from the ID - numItems/2 spot
    Query = str.sprintf("select " DICTIONARY_WORD ", " DICTIONARY_DEFINITION ", " DICTIONARY_WORD_INFO ", " DICTIONARY_ID_COLUMN " from " DICTIONARY_TABLE " where " DICTIONARY_ID_COLUMN " >= "  "%d limit %d", dataIndex, NumberOfDefinitions);

    result = SQLSelect(Query);

    if (result == Nil) {
        goto failSearch;
    }
   // [result retain);

    DS.append(HTML_TABLE_START);

    while (!result->atEOF()) {

        NumberOfDefinitions = NumberOfDefinitions-1;


        if (!result->atEOF() ){ // then exit // bail if the records give out

            if (result->FieldCount() != 0 ){ // then
                sWord = result->IdxField(1);
                myVar = result->IdxField(2);//Field*/ @ DICTIONARY_DEFINITION));


                myVar = ParseForDictionary(myVar,/* withPreviouslyFoundType:*/&foundType);

                myVar = myReplace(myVar, " )", ")");
                myVar = myReplace(myVar, "( ", "(");
                myVar = myReplace(myVar, " .", ".");
                myVar = myReplace(myVar, " ,", ",");




                Phonic = result->IdxField(3); //Field*/ @ DICTIONARY_WORD_INFO) );
                //Phonic = replace(Phonic,@ HTML_ITALIC_START,"");
                //Phonic = replace(Phonic,@ HTML_ITALIC_END,"");

                dataIndex = result->IdxField(4).toInt();// intValue);
                int fontSize = Preferences->getFontSize()/4;

               /* if (fontSize<10) {
                    fontSize +=2;
                    if (fontSize<9) {
                        fontSize=9;
                    }
                }
                */
                if (dataIndex== scrollToFieldID) {
                    sWord =str.sprintf("<A NAME=" QUOTE JavaBookMark QUOTE "></a><p><FONT SIZE=%d><b>%ls</b> %ls</FONT></p>\n"
                                       ,fontSize, (wchar_t*)Phonic.utf16(),(wchar_t*)myVar.utf16());// tag for auto scroll
                } else {
                    sWord =str.sprintf("<p><FONT SIZE=%d><b>%ls</b> %ls</FONT></p>\n"
                                       , fontSize, (wchar_t*)Phonic.utf16(),(wchar_t*)myVar.utf16());
                }


                if (Instr(sWord, "<br")) {
                    sWord = myReplace(sWord,"<br/>","<br>");
                    sWord = myReplace(sWord,"<br />","<br>");
                   // sWord = replace(sWord,"<br>","");
                }
                DS.append(HTML_ROW_START);
                DS.append(HTML_TABLE_CELL_START_LEFT_PAD);
                DS.append(sWord);
                DS.append(HTML_TABLE_CELL_END);
                DS.append(HTML_ROW_END);

            } // end if

            if (NumberOfDefinitions == 0){ // then exit  // bail from this loop
               delete result;
                iCurrentDataIndex = dataIndex; // pick up current data index (end)
                DS.append(HTML_TABLE_END);
                DS.append( BLANK_LINES_24 );

                return DS.join( "");
            } // end if

        }

        result->MoveNext();
    }
    delete result;


    iCurrentDataIndex = scrollToFieldID; // pick up current data index (end)

    if (DS.count()>0) {
        DS.append( BLANK_LINES_24 );

        return DS.join("");
    }

failSearch:
    return  "<i>No definition found!</i>";


}

