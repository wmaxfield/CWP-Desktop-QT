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
//  PreferencesLibrary.m
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 10/4/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//converted

#include "PreferencesLibrary.h"
//#include "misclibrary.h"

#include "biblemodule.h"
#include "HTMLModule.h"
//#include "PreferencesLibrary.h"
#include "structs.h"
#include "commentarymodule.h"
#include "WebDefinitionModule.h"
#include "searchlibrary.h"
#include "bookhandlerclass.h"
#include "notesLibrary.h"
#include "preferencesmodule.h"

#include <QList>

PreferencesModuleClass      *Preferences;
SHOW_THIS_INFO_TYPE globalSTI;
SearchInfoStructType globalSIT;


extern QList<BookHandlerClass *>      Books;


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString BibleKey(int BibleNumber)
{
    return "Book"+QString::number(BibleNumber);
    //QString str;

    //return str.sprintf("%s%d","Book",BibleNumber);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString CommentaryKey(int CommentaryNumber)
{
    QString str;
    return str.sprintf("%s%d","Commentary",CommentaryNumber);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString CommentaryTypeKey(int CommentaryNumber)
{
    QString str;
    return str.sprintf("%s%d",COMMENTARY_DATABASE_SUFFIX,CommentaryNumber);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString DocTypeKey(int Number)
{
    //QString str;
    //return str.sprintf("%s%d",BIBLE_DATABASE_SUFFIX,Number);
    return BIBLE_DATABASE_SUFFIX+QString::number(Number);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString NoteKey(int Number)
{
    QString str;
    return str.sprintf("%s%d","Note",Number);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString NoteTypeKey(int Number)
{
    QString str;
    return str.sprintf("%s%d",NOTE_DATABASE_SUFFIX  ,Number);
}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
QString GetPrefVariable(QString VarName)
{
    BOOL r;

    return Preferences->getPrefVariableByName(VarName, /*Category:@*/ CATEGORY_PREFERENCE, /*Result:*/&r);

}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
BOOL      SavePrefVariable(QString VarName,QString Value)
{
    //BOOL r;

   return Preferences->savePrefVariable(CATEGORY_PREFERENCE, /*VarName:*/VarName, /*Value:*/Value);
}
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
int OpenBibleByNameAndPutInSelectedArray(QString Name, int index2)
{
    // return success or fail
    int index;
    BibleDatabaseClass *bbld;
    BookHandlerClass *bk;
    QString Name2;

    //extern BOOL AddBibleToSelectedArrayByIndexNumber(int BibleNumber);//miscLibrary.m

    if (!Instr(Name,  BIBLE_DATABASE_SUFFIX )) {
        Name2 = Name + BIBLE_DATABASE_SUFFIX ;
    } else {
        Name2 = Name;
    }

    for (index=0; index < Books.count(); index++) {
        bk = Books[index];
        if (bk->Database) {
            bbld=(BibleDatabaseClass*)bk->Database;

            if (!Instr(bbld->_BibleName, BIBLE_DATABASE_SUFFIX) ) {
                if (StringsAreEqual(bbld->_BibleName , Name)) {
                    goto match;
                }
            }

            if (StringsAreEqual(bbld->_BibleName, Name2)) {
            match:
                //bbld->openWithFileName(Name);
                bk->selected = YES;

                if (AddBibleToSelectedArrayByIndexNumber(index,index2)==NO)
                    return FAIL;

                return SUCCESS;
            }
        }
    }

    return FAIL;

}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
int OpenCommentaryByNameAndPutInSelectedArray(QString Name)
{
     int index;
   // CommentaryDatabaseClass *bbld;
    BookHandlerClass *bk;
    QString Name2;
    QString n;

    if (!Instr(Name,  COMMENTARY_DATABASE_SUFFIX )) {
        Name2 = Name + COMMENTARY_DATABASE_SUFFIX ;
    } else {
        Name2 = Name;
    }
   for (index=0; index < Books.count(); index++) {

        bk = Books[index];
        if  (bk->Database) {

            n = bk->name;
            if (StringsAreEqual(n, Name2)) {
               // bbld = (CommentaryDatabaseClass*) bk->Database;
                //bbld->openWithType(TYPE_COMMENTARY_TYPE );
                bk->selected = YES;
                if (AddCommentaryToSelectedArrayByIndexNumber(index)==NO)
                    return FAIL;

                return SUCCESS;
            }
        }
    }
    return FAIL;

}


//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
extern int OpenPersonalNotesByNameAndPutInSelectedArray(QString Name);


//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
int OpenBookAndPutInSelectedArray(QString Type, QString Name, int index)
{


    if (StringsAreEqual(Type,  BIBLE_DATABASE_SUFFIX)) {
        return OpenBibleByNameAndPutInSelectedArray(Name,index);
    }

    if (StringsAreEqual(Type, COMMENTARY_DATABASE_SUFFIX)) {
        return OpenCommentaryByNameAndPutInSelectedArray(Name);
    }

    if (StringsAreEqual(Type,  NOTE_DATABASE_SUFFIX)) {
        return OpenPersonalNotesByNameAndPutInSelectedArray(Name);
    }

    return FAIL ;
}

#if 0
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
BOOL      LoadDefaultPrefs(void)
{
    QString str;

    // BOOL r;//dim r As Boolean
    //-------------------
    //Default Prefs Template
    //-------------------
    Preferences->RememberWindowPositions = YES;
    Preferences->enableDisplayStrongs = YES;
    Preferences->BlendStrongsIntoWord = YES;
    Preferences->bEnableMultimediaTag = NO;

    //--------------------
    //Load into Bible Prefs DB
    //--------------------
    Preferences->savePrefVariable( CATEGORY_PREFERENCE, /*VarName:*/ INI_REMEMBER_WINDOWS,
                                  /*Value:*/str.sprintf("%d", Preferences->RememberWindowPositions));
    Preferences->savePrefVariable( CATEGORY_PREFERENCE, /*VarName:*/ INI_SHOW_STRONGS, /*Value:*/ str.sprintf("%d", Preferences->enableDisplayStrongs));
    Preferences->savePrefVariable( CATEGORY_PREFERENCE, /*VarName:*/ INI_HIDE_STRONGS_NUMBERS, /*Value:*/ str.sprintf("%d", Preferences->BlendStrongsIntoWord));
    Preferences->savePrefVariable( CATEGORY_PREFERENCE, /*VarName:*/ INI_CHECK_VERSION, /*Value:*/str.sprintf("%d",  Preferences->bEnableVersionCheck));
    Preferences->savePrefVariable( CATEGORY_PREFERENCE, /*VarName:*/ INI_MULTIMEDIA_ENABLED, /*Value:*/ str.sprintf("%d", Preferences->bEnableMultimediaTag));
    SavePrefVariable( INI_PREF_VERSION, "1.0");

    Preferences->searchNT = YES;
    Preferences->searchOT = YES;
    Preferences->searchThisBookOnly = NO;
    //    SavePrefVariable(@ SEARCH_NT,@ "1");
    //SavePrefVariable(@ SEARCH_OT, @"1");
    //SavePrefVariable(@ SEARCH_BOOK, @"0");
    Preferences->getBlue();
    Preferences->getRed();
    Preferences->getGreen();


    // r=LocateaAndUpdateMediaVariable() // find the media prefs folder

    //-------------------------------------------------------------
    // first, see if it is right next to the application
    //dim f As FolderItem
    //f = App.CurWorkingFolder()
    //
    //r = FindFile(f,MEDIA_INFO_DB_NAME, 1000,f,YES)
    //MediaFolder = f
    //-----------------------------------------------------------



    Preferences->voicePitch = 125.0;
    Preferences->voiceVariance = 11.0;
    Preferences->voiceSpeed = 1.24;
    Preferences->voice = "cmu_us_slt";//woman
    Preferences->setSpeechEnabled(NO);
    Preferences->setSpokenHelp( NO);
    Preferences->setEnableDisplayPictures(YES);
    return YES;

}
#endif

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
BOOL      _LoadPreferences(void)
{
    // load in the preferences from the database
    QString BibleName; //dim BibleName As String
    QString DocType; //dim DocType As String
    //int     index; //dim index As Integer
    int r=YES; //dim r As Integer


    Preferences->loadingPrefs = YES;

    // determine if we should show the dictionary
    globalSTI.enableEnglishDictionary= Preferences->getShowEnglishDictionaryWindow();// = GetPrefVariable( INI_ENABLE_ENGLISH_DICTIONARY).toInt();


    // get the advanced html flag.  Might use this later in other versions, so pick up in other than linux
    Preferences->EnableAdvancedHTML =  GetPrefVariable( INI_ADVANCED_HTML).toInt();

    // fix the preferences to work
    Preferences->bEnableVersionCheck = GetPrefVariable( INI_CHECK_VERSION).toInt();
    //--------------------------------------------------------------------------
    // pick up the books location
    //--------------------------------------------------------------------------
    Preferences->UseAlternateDBPath = GetPrefVariable( INI_USE_ALTERNATE_DATABASE_PATH).toInt();

    if ( Preferences->UseAlternateDBPath) { //then
        QString path;// dim path As string
        path = GetPrefVariable( INI_DATABASE_PATH);

        if (path.length()!=0) { //then
            Preferences->DatabaseFolder = path;
        } else {
            SavePrefVariable( INI_DATABASE_PATH,getFullPathOfUserDocumentsCWPDirectory());
            return NO;
        } //end if

    } else {

    } //end if



    globalSTI.BlendStrongsIntoWord= Preferences->getBlendStrongs(); // FOR NOW.... GetPrefVariable( INI_HIDE_STRONGS_NUMBERS).toInt();
    globalSTI.ShowStrongs = Preferences->getEnableDisplayStrongs();

    globalSTI.EnableMultimediaTag = Preferences->getSpeechEnabled();
    Preferences->bSPeakFullChapter =  GetPrefVariable( INI_SPEAK_FULL_CHAPTER).toInt();

    Preferences->bHideNotes = GetPrefVariable( INI_NOTES_PREF).toInt();
    //--------------------------------------------------------------------------
    //Preferences->bHideNavigationButtons = GetPrefVariable( INI_HIDE_NAV_BUTTONS);
    //--------------------------------------------------------------------------

    Preferences->getFontSize();//force load GetPrefVariable( INI_FONT_SIZE).toInt();

    if (Preferences->getFontSize() == 0 ) { //then
        Preferences->setFontSize( 16); // correct for no value
    } //end if

    Preferences->HeaderSize = CalcHeaderSize();


    Preferences->getRed();// = GetPrefVariable( INI_R_COLOR).toInt();
    Preferences->getGreen();// = GetPrefVariable( INI_G_COLOR).toInt();// intValue );
    Preferences->getBlue();// = GetPrefVariable( INI_B_COLOR).toInt();

    Preferences->getSearchNT();// force load
    Preferences->getSearchOT();
    //Preferences->searchNT = GetPrefVariable( SEARCH_NT).toInt();
    //Preferences->searchOT = GetPrefVariable( SEARCH_OT).toInt();

    globalSTI.HilightSearch = NO;

    Preferences->loadingPrefs = NO;

    //globalSTI.doStackVerses = NO; // temporarily
    Preferences->doStackVerses = GetPrefVariable( INI_DO_STACK_VERSES).toInt();
    globalSTI.doStackVerses = Preferences->doStackVerses;

    if (Preferences->doStackVerses) {
        Preferences->PreferredTableStyle =  TABLE_STYLE_SINGLE_LINE_BORDERS;
    }else {
        Preferences->PreferredTableStyle =  TABLE_STYLE_NO_LINES;//TABLE_STYLE_SINGLE_LINE_BORDERS;
    }

    Preferences->firstTimeRun = NO;


    Preferences->voiceSpeed = GetPrefVariable( INI_VOICE_SPEED).toFloat();
    Preferences->voicePitch = GetPrefVariable( INI_VOICE_PITCH).toFloat();
    Preferences->voiceVariance = GetPrefVariable( INI_VOICE_VARIANCE).toFloat();
    Preferences->voice = GetPrefVariable( INI_VOICE );

    return r;

}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void saveSelectedBooks(void)
{
    int index;
    int NumSelectedBooks=Preferences->getNumberOfSelectedBooks();
    QString BibleName;
    QString str;

    for (index=0; index < NumSelectedBooks; index++) {
        BibleName = Preferences->SelectedBooks[index].Name;
        SavePrefVariable(BibleKey(index+1), BibleName);
        SavePrefVariable(DocTypeKey(index+1), str.sprintf("%d",Preferences->SelectedBooks[index].Type));
    }


}



