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
*/
//
//  PreferencesModule.m
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/24/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
// converted

#include <QString>
#include <QTextStream>


#include "preferencesmodule.h"
#include "HTMLModule.h"
#include "WebDefinitionModule.h"
#include "../shared/RecordSet/recordset.h"
#include "misclibrary.h"
#include "PreferencesLibrary.h"
//#include "FirstViewController.h"
#include "structs.h"
#include "biblemaxvalues.h"

//extern BookInfoStructType    gSelectedBooks[MAX_NUM_SELECTED_BOOKS];
extern BookInfoStructType    gSelectedNotes[MAX_NUM_SELECTED_NOTES];
extern BookInfoStructType    gSelectedCommentaries[MAX_NUM_SELECTED_COMMENTARIES];


//BookInfoStructType    gSelectedBooks[MAX_NUM_SELECTED_BOOKS];
BookInfoStructType    gSelectedNotes[MAX_NUM_SELECTED_NOTES];
BookInfoStructType    gSelectedCommentaries[MAX_NUM_SELECTED_COMMENTARIES];

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

PreferencesModuleClass::PreferencesModuleClass() : SQliteDatabase()
{


        loadingPrefs = YES;
        searchPrefChanged=NO;

        PreferredTableStyle =  TABLE_STYLE_NO_LINES;//TABLE_STYLE_SINGLE_LINE_BORDERS;
                                                     //LinkColor = @ COLOR_LIGHT_BLUE;
        blue=255; // color blue
        FontSize = 16;
        CreateIndexes = NO;
        //SelectedBooks = gSelectedBooks;
        numberOfSelectedBooks=0;
        SelectedNotes = gSelectedNotes;
        SelectedCommentaries = gSelectedCommentaries;
        numberOfSelectedNotes=0;

        numberOfSelectedCommentaries=0;

        numberOfNotesInSelectedNotesArray=0; // this is used to keep track of notes put into preferences array

        currentBook=1;
        currentChapter=1;
        currentVerse=1;
        currentPosition=0;

        CWPDirectory ="";
        CWPDirectoryLoaded=false;

        StrongsOffset=0;
        MaximumSearches=250;// was 100
        loadingPrefs = NO;
        NumberOfBooksInBible = 66; // may set in preferences on disk later
        voicePitch = 125.0;
        voiceVariance = 11;
        voiceSpeed = 1.1f;
        //doStackVerses=1;
    firstTimeRun=NO;

    databaseName = getPrefDBFullPath();// get the preferences database name and location
    Open();// open the database

    QString Query = "Select * from " BIBLE_PREFERENCES_TABLE " limit 1" ;
    if (SQLSelect(Query)==nil || (queryError.type() != QSqlError::NoError)) {
            CreateTables();// just in case
            setUpDefaults();
            firstTimeRun = YES;
        }

    doStackVerses=NO;
    enableEnglishDictionary=NO;
    enableVerseEdit=NO;
    blendStrongsLoaded=NO;
    bSPeakFullChapter=NO;
    bHideNotes=NO;
    bEnableVersionCheck=NO;
    _spokenHelp=NO;
    EnableAdvancedHTML=NO;
    green=0;
    greenLoaded=NO;
    red=0;
    redLoaded=NO;
    searchNT=YES;
    showBookMarkLoaded=NO;


    blueLoaded=NO;

    currentChapterLoaded=NO;
    currentVerseLoaded=NO;
    currentBookLoaded=NO;

    displayStrongsLoaded=NO;
    debugLoaded=NO;
    Debug = NO;

    enableEnglishDictionaryLoaded=NO;

    greenLoaded=NO;

    numberOfSelectedBooksLoaded=NO;
    numberOfSelectedNotesLoaded=NO;

    redLoaded=NO;

    spokenHelpLoaded = NO;
    speechEnableLoaded=NO;
    speechEnabled=NO;

    showBookMarkList = NO;
    searchNTLoaded=NO;
    searchOTLoaded=NO;
    fontSizeLoaded=NO;
    enableVerseEditLoaded=NO;
    //                     x y  w  h
    mainWindowGeometry = "10,10,800,600";
    lastBibleDictionarySearchText="";

    showCopyTagLoaded=false;
    showCopyTag=true;

}
// ------------------------------------------------------------------------
// this is called when first creating the Preferences database.
// ------------------------------------------------------------------------
void PreferencesModuleClass::setUpDefaults(){
    setBlue(158);
    setEnableDisplayStrongs(YES);
    setBlendStrongs(YES);// don't show numbers
    setEnableEnglishDictionary(YES);
    setCurrentBook(1);
    setCurrentChapter(1);
    setSearchNT(YES);
    setSearchOT(YES);
    setSearchThisBookOnly(NO);
    setShowBookMarkTag(YES);
    setShowCopyTag(YES);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void        PreferencesModuleClass::setCWPDirectory(QString directory){
  CWPDirectory=directory;
  QString str;
  if (!loadingPrefs) {
      savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_CWP_DIRECTORY , /*Value:*/CWPDirectory);
  }
    CWPDirectoryLoaded=true;
}
// ------------------------------------------------------------------------
// return the directory where the CWP bibles, etc. are stored.
// ------------------------------------------------------------------------
QString     PreferencesModuleClass::getCWPDirectory(){
    QString str;
    BOOL result=NO;
    if (!CWPDirectoryLoaded) {
        str = getPrefVariableByName(INI_CWP_DIRECTORY, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        CWPDirectory=str;
        CWPDirectoryLoaded = YES;
    }
    return CWPDirectory;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::Close() {
    isOpen=NO;
    return SQliteDatabase::Close();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setEnableDisplayPictures(BOOL _enableDisplayPicturesl ){
    QString str;
    _enableDisplayPictures = _enableDisplayPicturesl;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_ENABLE_SHOW_PICTURES , /*Value:*/str.sprintf("%d", _enableDisplayPicturesl));
    }
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getEnableDisplayPictures() {
#if CWP_TEACHER // for now
    return 1;
#else
    return _enableDisplayPictures;
#endif
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setVoice(QString voicel)
{
    voice = voicel;
    if (!loadingPrefs) {
       savePrefVariable(  CATEGORY_PREFERENCE , /*VarName:*/ INI_VOICE , /*Value:*/dbEscapeSingleQuoteString( voice));
    }
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setVoiceVariance(float voiceVariancel)
{
    QString str;
    voiceVariance = voiceVariancel;
    if (!loadingPrefs) {
       savePrefVariable(  CATEGORY_PREFERENCE , /*VarName:*/ INI_VOICE_VARIANCE , /*Value:*/str.sprintf( "%f",voiceVariancel));
    }

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setVoiceSpeed(float voiceSpeedl)
{
    QString str;
    voiceSpeed = voiceSpeedl;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_VOICE_SPEED , /*Value:*/str.sprintf("%f",voiceSpeedl));
    }

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setVoicePitch(float voicePitchl)
{
    QString str;
    voicePitch = voicePitchl;
    if (!loadingPrefs) {
       savePrefVariable(  CATEGORY_PREFERENCE , /*VarName:*/ INI_VOICE_PITCH , /*Value:*/str.sprintf("%f",voicePitchl));
    }
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString PreferencesModuleClass::getMainWindowGeometry()
{
    QString str;
    BOOL result=NO;
    str = getPrefVariableByName(INI_MAIN_WINDOW_XYWH, /*Category*/CATEGORY_PARAMS, /*Result:*/&result);
    if (result){
        mainWindowGeometry=str;
    }
    return mainWindowGeometry;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setMainWindowGeometry(const QString &value)
{
    QString str;

    if (!loadingPrefs) {
       savePrefVariable(  CATEGORY_PARAMS , /*VarName:*/ INI_MAIN_WINDOW_XYWH , /*Value:*/value);
    }

    mainWindowGeometry = value;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString PreferencesModuleClass::getBibleDictionaryGeometry()
{
    QString str;
    BOOL result=NO;
    str = getPrefVariableByName(INI_BIBLE_DICTIONARY_XYWH, /*Category*/CATEGORY_PARAMS, /*Result:*/&result);
    if (result){
        bibleDictionaryGeometry=str;
    }

    return bibleDictionaryGeometry;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setBibleDictionaryGeometry(const QString &value)
{
    QString str;

    if (!loadingPrefs) {
       savePrefVariable(  CATEGORY_PARAMS , /*VarName:*/ INI_BIBLE_DICTIONARY_XYWH , /*Value:*/value);
    }

    bibleDictionaryGeometry = value;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString PreferencesModuleClass::getLastBibleDictionarySearchText()
{
    QString str;
    BOOL result=NO;
    str = getPrefVariableByName(INI_BIBLE_DICTIONARY_SEARCH_TEXT, /*Category*/CATEGORY_PARAMS, /*Result:*/&result);
    if (result){
        lastBibleDictionarySearchText=str;
    }


    return lastBibleDictionarySearchText;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setLastBibleDictionarySearchText(const QString &value)
{
    QString str;

    if (!loadingPrefs) {
       savePrefVariable(  CATEGORY_PARAMS , /*VarName:*/ INI_BIBLE_DICTIONARY_SEARCH_TEXT , /*Value:*/value);
    }

    lastBibleDictionarySearchText = value;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setSpokenHelp(BOOL n)
{
    _spokenHelp = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_SPOKEN_HELP_ENABLED , /*Value:*/QString::number(n));
    }
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
bool PreferencesModuleClass::getEnableVerseEdit()
{

    QString str;
    BOOL result=NO;
    if (!enableVerseEditLoaded) {
        str = getPrefVariableByName(INI_ENABLE_VERSE_EDIT, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        enableVerseEdit = str.toInt();// convert to BOOL
        enableVerseEditLoaded = YES;
    }


    return enableVerseEdit;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setEnableVerseEdit(bool value)
{
    enableVerseEdit = value;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_ENABLE_VERSE_EDIT , /*Value:*/QString::number(value));
    }
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getSpokenHelp()
{
    QString str;
    BOOL result=NO;
    if (!spokenHelpLoaded) {
        str = getPrefVariableByName(INI_SPOKEN_HELP_ENABLED, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        _spokenHelp = str.toInt();// convert to BOOL
        spokenHelpLoaded = YES;
    }


    return _spokenHelp;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::incrementNumberOfSelectedNotes() {
    setNumberOfSelectedNotes(numberOfSelectedNotes+1);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::decrementNumberOfSelectedNotes() {
    if (numberOfSelectedNotes)
        setNumberOfSelectedNotes(numberOfSelectedNotes-1);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int PreferencesModuleClass::getNumberOfSelectedNotes()
{
    QString str;
    BOOL result=YES;
    if (!numberOfSelectedNotesLoaded) {
        str = getPrefVariableByName(INI_NUM_SELECTED_NOTES, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        numberOfSelectedNotesLoaded = YES;
        numberOfSelectedNotes = str.toInt();// convert to BOOL
    }

    return numberOfSelectedNotes;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setNumberOfSelectedNotes(int n)
{
    QString str;
    numberOfSelectedNotes= n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_NUM_SELECTED_NOTES , /*Value:*/QString::number(n));
    }
    numberOfSelectedNotesLoaded=YES;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int PreferencesModuleClass::getCurrentChapter()
{

    QString str;
    BOOL result=YES;
    if (!currentChapterLoaded) {
        str = getPrefVariableByName(INI_CHAPTER, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        currentChapterLoaded = YES;
        currentChapter = str.toInt();// convert to BOOL
    }

    if (currentChapter ==0)
        setCurrentChapter(1);

    return currentChapter;


}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setCurrentChapter(int n)
{
    QString str;
    currentChapter= n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_CHAPTER , /*Value:*/str.sprintf("%d",n));
    }
    currentChapterLoaded=YES;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getSearchNT()
{
    QString str="0";
    BOOL result=YES;
    if (!searchNTLoaded) {
        str = getPrefVariableByName(SEARCH_NT, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        searchNT = str.toInt();// convert to BOOL
        searchNTLoaded = YES;
    }


    return searchNT;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setSearchNT(BOOL n)
{
    QString str;
    searchNT = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ SEARCH_NT , QString::number(n));
        searchPrefChanged = YES;
    }
    searchNTLoaded=YES;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getSearchOT()
{
    QString str="0";
    BOOL result=YES;
    if (!searchOTLoaded) {
        str = getPrefVariableByName(SEARCH_OT, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        searchOT = str.toInt();// convert to BOOL
        searchOTLoaded = YES;
        searchPrefChanged = YES;
    }


    return searchOT;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setSearchOT(BOOL n)
{
    QString str;
    searchOT = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ SEARCH_OT , QString::number(n));
        searchPrefChanged = YES;
    }
    searchOTLoaded=YES;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getSearchThisBookOnly()
{
    QString str="0";
    BOOL result=YES;
    if (!searchThisBookOnlyLoaded) {
        str = getPrefVariableByName(SEARCH_BOOK, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        searchThisBookOnly = str.toInt();// convert to BOOL
        searchThisBookOnlyLoaded = YES;
    }


    return searchThisBookOnly;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setSearchThisBookOnly(BOOL n)
{
    QString str;
    searchThisBookOnly = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ SEARCH_BOOK , QString::number(n));
    }
    searchThisBookOnlyLoaded=YES;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int PreferencesModuleClass::getNumberOfSelectedBooks()
{
    QString str="0";
    BOOL result=YES;
    if (!numberOfSelectedBooksLoaded) {
        str = getPrefVariableByName(INI_NUM_SELECTED_BOOKS, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        numberOfSelectedBooksLoaded = YES;
        numberOfSelectedBooks = str.toInt();
    }

    return numberOfSelectedBooks;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setNumberOfSelectedBooks(int n)
{
    QString str;
    numberOfSelectedBooks = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_NUM_SELECTED_BOOKS , /*Value:*/str.sprintf("%d",n));
    }
    numberOfSelectedBooksLoaded=YES;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int PreferencesModuleClass::getNumberOfSelectedCommentaries() const
{
    return numberOfSelectedCommentaries;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setNumberOfSelectedCommentaries(int value)
{
    numberOfSelectedCommentaries = value;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::incrementNumberOfSelectedBooks() {
    setNumberOfSelectedBooks(numberOfSelectedBooks+1);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::decrementNumberOfSelectedBooks() {
    if (numberOfSelectedBooks>0)
        setNumberOfSelectedBooks(numberOfSelectedBooks-1);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getShowBookMarkTag() {
    QString str;
    BOOL result=YES;
    if (!showBookMark) {
        str = getPrefVariableByName(INI_ENABLE_BOOKMARK_TAG, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        showBookMark = str.toInt();// convert to BOOL
        showBookMarkLoaded = YES;
    }


    return showBookMark;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void  PreferencesModuleClass::setShowBookMarkTag(BOOL n){
    QString str;
    showBookMark = n;
    //globalSTI.enableEnglishDictionary=n;
    if (!loadingPrefs) {
      savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_ENABLE_BOOKMARK_TAG , /*Value:*/str.sprintf("%d",n));
    }

    showBookMarkLoaded=YES;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getShowCopyTag() {
    QString str;
    BOOL result=YES;
    if (!showCopyTagLoaded) {
        str = getPrefVariableByName(INI_SHOW_COPY_TAG, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        showCopyTag = str.toInt();// convert to BOOL
        showCopyTagLoaded = YES;
    }


    return showCopyTag;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void  PreferencesModuleClass::setShowCopyTag(BOOL n){
    QString str;
    showCopyTag = n;
    //globalSTI.enableEnglishDictionary=n;
    if (!loadingPrefs) {
      savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_SHOW_COPY_TAG , /*Value:*/str.sprintf("%d",n));
    }

    showCopyTagLoaded=YES;

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getShowEnglishDictionaryWindow() {
    QString str;
    BOOL result=NO;
    if (!enableEnglishDictionaryLoaded) {
        str = getPrefVariableByName(INI_ENABLE_ENGLISH_DICTIONARY, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        enableEnglishDictionary = str.toInt();// convert to BOOL
        enableEnglishDictionaryLoaded = YES;
    }


    return enableEnglishDictionary;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void  PreferencesModuleClass::setShowEnglishDictionaryWindow(BOOL n){
   setEnableEnglishDictionary(n);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getDisplayStrongs() {
    QString str;
    BOOL result=NO;
    if (!displayStrongsLoaded) {
        str = getPrefVariableByName(INI_SHOW_STRONGS, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        enableDisplayStrongs = str.toInt();// convert to BOOL
        displayStrongsLoaded = YES;
    }


    return enableDisplayStrongs;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void  PreferencesModuleClass::setDisplayStrongs(BOOL n){
   // QString str;
    enableDisplayStrongs = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_SHOW_STRONGS , /*Value:*/QString::number(n));
    }
    displayStrongsLoaded=YES;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getBlendStrongs() {
    QString str;
    BOOL result=NO;
    if (!blendStrongsLoaded) {
        str = getPrefVariableByName(INI_HIDE_STRONGS_NUMBERS, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        BlendStrongsIntoWord = str.toInt();// convert to BOOL
        blendStrongsLoaded = YES;
    }


    return BlendStrongsIntoWord;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void  PreferencesModuleClass::setBlendStrongs(BOOL n){
   // QString str;
    BlendStrongsIntoWord = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_HIDE_STRONGS_NUMBERS , /*Value:*/QString::number(n));
    }
    blendStrongsLoaded=YES;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int16_t PreferencesModuleClass::getCurrentBook()
{
    QString str;
    BOOL result=NO;
    if (!currentBookLoaded) {
        str = getPrefVariableByName(INI_BOOK, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        currentBook = str.toInt();// convert to BOOL
        currentBookLoaded = YES;
    }

    if (currentBook <= 0 || currentBook >MAX_NUMBER_OF_BOOK_IN_BIBLE)
        setCurrentBook(1);// fix database at the same time
    return currentBook;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setCurrentBook(int16_t n)
{
    if (n < 1 || n > MAX_NUMBER_OF_BOOK_IN_BIBLE)
        return; // do not allow bogus value in
    //QString str;
    currentBook = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_BOOK , /*Value:*/QString::number(n));
    }
    currentBookLoaded=YES;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int PreferencesModuleClass::getDebug()
{
#if ENABLE_DEBUG
    QString str;
    BOOL result=NO;
    if (!debugLoaded) {
        str = getPrefVariableByName(INI_DEBUG, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        Debug = str.toInt();// convert to BOOL
        debugLoaded = YES;
    }

    return Debug;
#else
    return NO;
#endif
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setDebug(int n)
{
#if ENABLE_DEBUG
    //QString str;
    Debug = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_DEBUG , /*Value:*/QString::number(n));
    }
    debugLoaded=YES;
#else
    Q_UNUSED(n);
    Debug=NO;
    debugLoaded=YES;
#endif
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int16_t PreferencesModuleClass::getCurrentVerse()
{
    QString str;
    BOOL result=NO;
    if (!currentVerseLoaded) {
        str = getPrefVariableByName(INI_VERSE, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        currentVerse = str.toInt();// convert to BOOL
        currentVerseLoaded = YES;
    }
    if (currentVerse <= 0)
        currentVerse = 1;

    return currentVerse;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setCurrentVerse(int16_t n)
{
    // don't write to disk more than we have to
    if (currentVerse==n)
        return;

    //QString str;
    currentVerse = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_VERSE , /*Value:*/QString::number(n));
    }
    currentVerseLoaded=YES;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getSpeechEnabled()
{
    QString str;
    BOOL result=NO;
    if (!speechEnableLoaded) {
        str = getPrefVariableByName(INI_SPEECH_ENABLED, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        speechEnabled = str.toInt();// convert to BOOL
        speechEnableLoaded = YES;
    }

#if !ENABLE_SPEECH
    speechEnabled=NO;
#endif
    return speechEnabled;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setSpeechEnabled(BOOL n)
{
   // QString str;
    speechEnabled = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_SPEECH_ENABLED , /*Value:*/QString::number(n));
    }
    speechEnableLoaded=YES;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setEnableDisplayStrongs(BOOL n)
{
    setDisplayStrongs(n);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getEnableDisplayStrongs()
{
    return getDisplayStrongs();
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::getEnableEnglishDictionary() {
    return getShowEnglishDictionaryWindow();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass:: setEnableEnglishDictionary(BOOL n)
{
    QString str;
    enableEnglishDictionary = n;
    //globalSTI.enableEnglishDictionary=n;
    if (!loadingPrefs) {
      savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_ENABLE_ENGLISH_DICTIONARY , /*Value:*/str.sprintf("%d",n));
    }

    enableEnglishDictionaryLoaded=YES;

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setDoStackVerses(BOOL n)
{
    QString str;
    doStackVerses = n;
    if (!loadingPrefs) {
      savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_DO_STACK_VERSES , /*Value:*/str.sprintf( "%d",n));
    }
}




// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int PreferencesModuleClass::getFontSize() {
    QString str;
    BOOL result=NO;
    if (!fontSizeLoaded) {
        str = getPrefVariableByName(INI_FONT_SIZE, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        FontSize = str.toInt();// convert to BOOL
        fontSizeLoaded = YES;
    }

    if (FontSize==0)
        setFontSize(16);// default font size
    return FontSize;
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setFontSize(int lFontSize)
{
   // QString str;
    FontSize = lFontSize;
    if (!loadingPrefs) {
      savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_FONT_SIZE , /*Value:*/QString::number(lFontSize));
    }
    fontSizeLoaded=YES;

}






// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString PreferencesModuleClass::getPrefVariableByName(QString varName, /*Category*/QString category, /*Result:*/BOOL *result)
{
    // usage  Variable = GetVariable(TableName,DB,Result,Category,VariableName
    // if result = false then bail
    //dim Result as RecordSet
    RecordSet *rs=0;
    //dim k as integer
    QString Query="";//Dim Query as String
    QString myVar="";//Dim myVar as Variant
                    //dim myDB As REALSQLDatabase


    *result = NO;
    QTextStream(&Query) << "select Value from " BIBLE_PREFERENCES_TABLE " where Category = '" << category <<"' and Name = '" << varName << "' limit 1";

    rs =SQLSelect(Query);
    // NSLog(@"rs retain count=%d",[rs retainCount]);
    if (rs != Nil ) {//Then
        if (rs->FieldCount()  > 0 ){// Then
            myVar = rs->Field("Value");//.StringValue // wsm .Value returns nil
            if (!myVar.isEmpty() ) {//Then
                *result = YES;
            }//end if
        }//end if
      delete rs;
    } else {// end if
        return "";
    }
  return myVar;
}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::savePrefVariable(QString Category,/* VarName*/QString VariableName, /* Value*/QString myVar)
{
    //return True if success, False if error
    QString Query;// dim Query as String


    //SQLExecute("BEGIN");

    myVar = myReplace(myVar, QUOTE,"\\\"");
    myVar = myReplace(myVar,"'","\'");


    QTextStream(&Query) << "delete from " BIBLE_PREFERENCES_TABLE " where Category = '" << Category << "' and Name = '" << VariableName << "'";
    BOOL t = showSQLExecError;
    extern int16_t gShowSQLExecError;
    showSQLExecError = gShowSQLExecError;
    SQLExecute(Query);

    if (queryError.type() != QSqlError::NoError) {
        if (showSQLExecError) {
            MsgBox( "An error occured saving a preference.  The error was '"+queryError.databaseText());
            extern int emergencyExit;
            emergencyExit=true;
            return NO;
        }
       // return NO;
    }



    // the variable does not exist, insert it
    Query="insert into " BIBLE_PREFERENCES_TABLE "(ID,Category,Name,Value) values (NULL,'"+Category+"','"+VariableName+"','"+myVar+"')";
    SQLExecute(Query);

    if (queryError.type() != QSqlError::NoError) {
        if (showSQLExecError)
            MsgBox( "An error occured saving a preference.  The error was '"+queryError.databaseText());
        return NO;
    }
    showSQLExecError = t;

   // SQLExecute("COMMIT");


    return YES;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::CreateTables() // create the database tables (assuming they don't yet exist)
{
    QString Query;//Dim Query as String


    Query="create table " BIBLE_PREFERENCES_TABLE " (ID integer NOT NULL PRIMARY KEY,Category varchar, Name varchar, Value varchar)";
    SQLExecute(Query);

    if (queryError.type() != QSqlError::NoError) {
        if (Instr( queryError.databaseText(),"already exists")>0)
            return YES;
        else
            goto ProjectError;
    }

    return YES;

ProjectError:
    MsgBox ( "There was an error creating the User Preferences.");
    return (NO);

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL PreferencesModuleClass::DropTables()
{
    QString Query;//Dim Query as String

    Query = "drop table if exists " BIBLE_PREFERENCES_TABLE;

    SQLExecute(Query);

    return Error();

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass:: LoadPreferences() // calls the external c function
{
    _LoadPreferences();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int PreferencesModuleClass::getRed()
{
    QString str;
    BOOL result=NO;
    if (!redLoaded) {
        str = getPrefVariableByName(INI_G_COLOR, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        red = str.toInt();// convert to BOOL
        redLoaded = YES;
    }

    return red;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setRed(int n)
{
    //QString str;
    red = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_G_COLOR , /*Value:*/QString::number(n));
    }
    redLoaded=YES;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int PreferencesModuleClass::getBlue()
{
    QString str;
    BOOL result=NO;
    if (!blueLoaded) {
        str = getPrefVariableByName(INI_B_COLOR, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        blue = str.toInt();// convert to BOOL
        blueLoaded = YES;
    }


    return blue;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setBlue(int n)
{
    //QString str;
    blue = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_B_COLOR , /*Value:*/QString::number(n));
    }
    blueLoaded=YES;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int PreferencesModuleClass::getGreen()
{
    QString str;
    BOOL result=NO;
    if (!greenLoaded) {
        str = getPrefVariableByName(INI_G_COLOR, /*Category*/CATEGORY_PREFERENCE, /*Result:*/&result);
        green = str.toInt();// convert to BOOL
        greenLoaded = YES;
    }

    return green;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void PreferencesModuleClass::setGreen(int n)
{
    //QString str;
    green = n;
    if (!loadingPrefs) {
        savePrefVariable( CATEGORY_PREFERENCE , /*VarName:*/ INI_G_COLOR , /*Value:*/QString::number(n));
    }
    greenLoaded=YES;
}
