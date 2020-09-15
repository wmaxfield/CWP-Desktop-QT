#ifndef PREFERENCESMODULE_H
#define PREFERENCESMODULE_H

// PREFERENCESMODULE_H
//
//  PreferencesModule.h
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/24/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//
#include <QString>
#include "cwpdefines.h"

#include "structs.h"


#include "../shared/Database/database.h"

#define BIBLE_PREFERENCES_TABLE "BiblePrefs"



class PreferencesModuleClass : public SQliteDatabase
{
public:
    PreferencesModuleClass();
    BOOL Close();

    QString getPrefVariableByName(QString varName, /*Category:*/ QString category, /*Result:*/BOOL *result);

    BOOL savePrefVariable(QString Category,/*VarName:*/QString VariableName, /*Value:*/QString myVar);

    void LoadPreferences(); // calls the external c function

    BOOL  CreateTables(); // create the database tables (assuming they don't yet exist)
    BOOL  DropTables(); // drop the table in preparation for recreating it.

    void  setUpDefaults();// initialize empty table

    void    setEnableDisplayPictures(BOOL enableDisplayPicturesl );
    BOOL    getEnableDisplayPictures(void);
    void    setVoice(QString voicel);
    void    setVoiceVariance(float voiceVariancel);
    void    setVoiceSpeed(float voiceSpeedl);
    void    setVoicePitch(float voicePitchl);


    BOOL    getShowCopyTag();
    void    setShowCopyTag(BOOL n);


    BOOL    getShowBookMarkTag();
    void    setShowBookMarkTag(BOOL n);

    BOOL    getShowEnglishDictionaryWindow();
    void    setShowEnglishDictionaryWindow(BOOL n);

    BOOL    getEnableEnglishDictionary();
    void    setEnableEnglishDictionary(BOOL n);
    void    setEnableDisplayStrongs(BOOL n);
    void    setSpeechEnabled(BOOL n);
    void    setDoStackVerses(BOOL n);


    void        setCurrentChapter(int lCurrentChapter);
    void        setCurrentVerse(int16_t lCurrentVerse);
    void        setCurrentBook(int16_t lCurrentBook);

    int         getFontSize();
    void        setFontSize(int lFontSize);
    void        setSearchThisBookOnly(BOOL lsearchBOOK);

    void        setCWPDirectory(QString directory);
    QString     getCWPDirectory();
    bool        CWPDirectoryLoaded;
    QString     PreferredTableStyle;
    QString     LinkColor;


    void        setBlendStrongs(BOOL n);
    BOOL        getBlendStrongs();

    int         StrongsOffset;              // the number of pixels to move the popup when it is shown

    BOOL        CreateIndexes;
    BOOL        bEnableMultimediaTag;
    BOOL        bEnableVersionCheck;
    BOOL        EnableAdvancedHTML;

    QString     DatabaseFolder;




    BOOL        getEnableDisplayStrongs();
    BOOL        getDisplayStrongs();
    void        setDisplayStrongs(BOOL b);
    BOOL        RememberWindowPositions;
    BOOL        UseAlternateDBPath;

    BookInfoStructType    SelectedBooks[MAX_NUM_SELECTED_BOOKS];

    BOOL        bSPeakFullChapter;

    QString     CurrentBookName;
    // ------------------------------------------------------------------------
    int16_t     getCurrentBook();
    int         getCurrentChapter();
    int16_t     getCurrentVerse();
    int         showBookMarkList;
    // ------------------------------------------------------------------------
    int         getDebug();
    void        setDebug(int d);

    int         getNumberOfSelectedBooks();
    void        setNumberOfSelectedBooks(int n);
    void        incrementNumberOfSelectedBooks();
    void        decrementNumberOfSelectedBooks();

    void        incrementNumberOfSelectedNotes();
    void        decrementNumberOfSelectedNotes();
    int         getNumberOfSelectedNotes();
    void        setNumberOfSelectedNotes(int n);
    int         numberOfNotesInSelectedNotesArray;
    int         HeaderSize;
    int         MaximumSearches;

    int         NumberOfBooksInBible;

    // ------------------------------------------------------------------------
    BOOL        getSearchOT();
    void        setSearchOT(BOOL n);

    BOOL        getSearchNT();
    void        setSearchNT(BOOL n);
    BOOL        getSearchThisBookOnly();


    // keep local, not saved in preferences db
    BOOL        searchPrefChanged;
    // ------------------------------------------------------------------------

    BOOL        loadingPrefs;       // when true, we are loading preferences, don't write the database at that time.

    int         getRed();
    void        setRed(int r);

    int         getGreen();
    void        setGreen(int g);

    int         getBlue();
    void        setBlue(int b);

    // ------------------------------------------------------------------------
    BOOL        bHideNotes;
    BookInfoStructType *SelectedNotes;
    // ------------------------------------------------------------------------

    QString     CWPDirectory;
    // ------------------------------------------------------------------------
    int getNumberOfSelectedCommentaries() const;
    int                 numberOfSelectedCommentaries;

    void setNumberOfSelectedCommentaries(int value);

    QString             commentaryFile;
    BookInfoStructType *SelectedCommentaries;
    // ------------------------------------------------------------------------

    BOOL         doStackVerses;

    BOOL         getSpeechEnabled();
    BOOL        getSpokenHelp();
    void        setSpokenHelp(BOOL sh);
    // ------------------------------------------------------------------------
    BOOL        firstTimeRun;
    float       voicePitch;
    float       voiceVariance;
    float       voiceSpeed;
    QString voice;
    // ------------------------------------------------------------------------
    // keep public for quick access, bad design, but needed for
    // speed in building the web page
    int         red;
    int         blue;
    int         green;


    int         currentPosition; // only used by history when going back and forth

    bool getEnableVerseEdit() ;
    void setEnableVerseEdit(bool value);


    QString getMainWindowGeometry() ;
    void setMainWindowGeometry(const QString &value);
    QString mainWindowGeometry;

    QString getBibleDictionaryGeometry() ;
    void setBibleDictionaryGeometry(const QString &value);

    QString getLastBibleDictionarySearchText() ;
    void setLastBibleDictionarySearchText(const QString &value);

private:

    int16_t         currentBook;
    int16_t         currentChapter;
    int16_t         currentVerse;
    int16_t         numberOfSelectedBooks;

    int16_t         redLoaded;
    int16_t         blueLoaded;
    int16_t         greenLoaded;


    BOOL         numberOfSelectedBooksLoaded;

    BOOL        _enableDisplayPictures;
    BOOL        _spokenHelp;
    BOOL        spokenHelpLoaded;
    BOOL        speechEnabled;
    BOOL       speechEnableLoaded;
    BOOL        currentChapterLoaded;
    BOOL       currentVerseLoaded;
    BOOL        currentBookLoaded;



    BOOL        blendStrongsLoaded;
    BOOL        BlendStrongsIntoWord;

    BOOL        showCopyTag;
    BOOL        showCopyTagLoaded;

    BOOL        showBookMark;
    BOOL        showBookMarkLoaded;

    BOOL         enableEnglishDictionary;
    BOOL         enableEnglishDictionaryLoaded;

    BOOL        enableDisplayStrongs;
    BOOL        displayStrongsLoaded;

    int         FontSize;
    BOOL        fontSizeLoaded;

    int         numberOfSelectedNotes;
    BOOL        numberOfSelectedNotesLoaded;

    BOOL        searchNT;
    BOOL        searchNTLoaded;
    BOOL        searchThisBookOnlyLoaded;
    BOOL        searchThisBookOnly;

    BOOL        searchOT;           // from the custom search button
    BOOL        searchOTLoaded;

    int          Debug;
    int         debugLoaded;

    bool        enableVerseEdit;
    bool        enableVerseEditLoaded;

    QString     bibleDictionaryGeometry;
    QString     lastBibleDictionarySearchText;
};

#if STUDENT_OR_TEACHER || STUDENT
extern PreferencesModuleClass      *Preferences; // in PreferencesLibrary.h
#endif


#endif

