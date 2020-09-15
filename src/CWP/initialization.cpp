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
*/// ------------------------------------------------------------------------
//
//  Initialization.m
//  CWP-Teacher
//
//  Created by WADE MAXFIELD on 4/14/11.
//  Copyright 2011 Wade Maxfield. All rights reserved.
//converted
// ------------------------------------------------------------------------
#include <QObject>
#include <QStringList>
#include <QList>

#include "initialization.h"
#include "bookpreferencesdbclass.h"
#include "userhistoryclass.h"
#include "MediaModuleDatabase.h"

#include "BookMarkDB.h"

extern QStringList                 speakingList;
extern int16_t gShowSQLExecError;
extern float                       gVersion;
extern BookMarkDB                  *gBookBookMarkDB;

//--------
extern BookPreferencesDBClass *gTextBookPreferences;
int16_t gShowSQLExecError=YES; // when TRUE, show the exec errors
int16_t gMediaNotFound;

//UserHistoryClass    *userHistory;  in userhistoryclass.cpp
BookMarkDB                  *gBookMarkDB;
BookMarkDB                  *gBookBookMarkDB;

extern MediaModuleDatabase  *gMediaModuleDatabase;

// speakinglist format:
// index 0 -- verse
// index 1 -- bible number, book, chapter, verse  "2:66:3:23"
QStringList                 speakingList;
QList<BookHandlerClass *>   gTextBooks;
float                       gVersion;
int gNumberOfBooks;

BOOL isRunningInSimulator;


#include "bookdbclass.h"

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void checkDirArray(QStringList dirArray, /*withPathComponent:*/QString pathComponent, /*ForType:*/int typeOfBook, /*andReplaceInBooks*/BOOL replaceBook)
{
    // now load from the chosen directory, replacing objects in the Books array with the matching new object
    int i=0;
    int NumberOfFiles=0;
    int index=0;
    QString FileName="";
    BibleDatabaseClass *bd=nullptr;
    BookHandlerClass *bh=nullptr;


    BookDBClass *bh1=nullptr;

    QString suffix="";

    suffix = getSuffixStringFromTypeInt(typeOfBook);
    //[suffix retain];

    if (dirArray.count()) {
        NumberOfFiles = dirArray.count();

        for (index=0; index<NumberOfFiles; index++) {
            FileName = dirArray [index];
            if (Instr(FileName,suffix)> 0) {
                switch (typeOfBook) {
                        default:
                    case TYPE_BBL_TYPE:
                        bd = new BibleDatabaseClass(pathComponent+"/"+FileName);
                        // wsm, don't do this-->bd->setDbType(typeOfBook);
                         bd->pathComponent = pathComponent;
                         //bd->_BibleName =FileName;
                         bd->objectType = BOOK_TYPE_BIBLE;
                    break;

                    case TYPE_BOOK_TYPE: {
                        bh1 = new BookDBClass();
                        //bh1->setDbType( typeOfBook);

                        bh1->_BibleName =FileName;
                        bh1->pathComponent = pathComponent;
                        bh1->objectType = BOOK_TYPE_BOOK;

                        bd = reinterpret_cast<BibleDatabaseClass*>(bh1);
                }
                    break;
                }
                   // bd = new BibleDatabaseClass();;


                bh = new BookHandlerClass();
                bh->Database = bd;

                // save our bacon
                bh->name =FileName;

                if (replaceBook) {
                    for (i=0 ; i < Books.count(); i++) {
                        QString n = ((BookHandlerClass*)Books[i])->name ;
                        if (StringsAreEqual(FileName,n )) {
                            delete Books[i];
                            Books[i] =bh;
                            break; // finished with this loop
                        }
                    }

                    if (i == Books.count()) {
                        Books.append(bh);
                    }
                } else {
                    // if we aren't replacing, then we are only adding
                    Books.append(bh);
                }
            }
        }
    }
}
// ------------------------------------------------------------------------
// test for sort routine
// ------------------------------------------------------------------------
bool BookHandlerClassLessThan(const BookHandlerClass *b1, const BookHandlerClass *b2) {
    //return b1->name < b2->name ;<--this suddenly broke
    return b1->name.toLower() < b2->name.toLower();// <--had to replace with this
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void initialization::getSelectedNotes() {

    int NumSelectedBooks = Preferences->getNumberOfSelectedBooks();

    int index;
    QString DocType;
    QString BibleName;
    int selectIndex=0;// having this as a separate index allows skipping of malformed database entries
    //int r;

    for (index = 0 ; index < NumSelectedBooks ; index++) {
        // get the bible from the database
        BibleName =GetPrefVariable(BibleKey(index+1)) ;
        if (BibleName =="") {
            // the database is broken
            // don't open this one
            continue;
        }
        DocType = getSuffixStringFromTypeString( GetPrefVariable(DocTypeKey(index+1)));


        if (StringsAreEqual(DocType , NOTE_DATABASE_SUFFIX)) { //then
          // get the notes from the PersonalCommentary
           // r = FAIL;
        } //end if
        else {
           // r =
                  OpenBookAndPutInSelectedArray(DocType,BibleName,selectIndex);

        } //end if

        selectIndex++;
    }//next

}
extern int enable_debug; // set in MainWindow.cpp
// ------------------------------------------------------------------------
// initialize the system, find where the documents are.
// To be cross-platform, the preferences file is in the
// user's documents directory, which is inside the CWP directory.
// ------------------------------------------------------------------------
initialization::initialization() {
    failed =0;
    //--------------------------------------------------------------------
    // preferences must be loaded early in the game, but after books are
    // known
    // now we have the CWP Directory inside the Preferences class.
    //--------------------------------------------------------------------
    Preferences = new PreferencesModuleClass() ;

   //int NumSelectedBooks = Preferences->getNumberOfSelectedBooks();

    initBibles();

    initCommentaries();
    initNotes (); // set up for user notes.
    initMedia();

    initLibraryStuff ();       // set up for the bibles

    QStringList Book2;

    qStableSort( Books.begin(),Books.end(),BookHandlerClassLessThan);


    getSelectedNotes();


    getSelectedCommentaries();

    // set the number of selected books as the number found
    //Preferences->setNumberOfSelectedBooks(selectIndex);
    //--------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // NOTES
    //--------------------------------------------------------------------------
    getSelectedNotes();// notesLibrary.m

    extern void  initHistory();
    initHistory();
    //--------------------------------------------------------------------
   //--------------------------------------------------------------------
    gVersion = 1;
    //--------------------------------------------------------------------

    if (Preferences->getSpokenHelp() || Preferences->getSpeechEnabled()) {
        // always start this, just in case.
#if 0
        gFliteEngine = [[FliteTTS alloc] init];
        if (gFliteEngine) {
            [gFliteEngine setPitch:Preferences.voicePitch   variance:Preferences.voiceVariance speed:Preferences.voiceSpeed];
            [gFliteEngine setVoice:Preferences.voice];
        }
#endif
    }

    initTextBooks();

    gTextBookPreferences =new BookPreferencesDBClass();//

    return ;

}

// ------------------------------------------------------------------------
// set up the media if it is available
// ------------------------------------------------------------------------
void  initialization::initMedia()
{
    QString dirName;// = getFullPathOfAppBundleMediaDirectory();
    QStringList dirArray;// =getDirectoryListing(dirName);
    int         index;
    QString  FileName;
    int         NumberOfFiles;

    // user documents overrides everything.  (user must copy entire media directory)
    dirName = getFullPathOfUserDocumentsCWPMediaDirectory();
    dirArray = getDirectoryListing(dirName);


    if (dirArray.count()==0){
        // Nothing in app bundle.  Check Preferences directory
        dirName = getFullPathOfPreferencesMediaDirectory();
        dirArray = getDirectoryListing(dirName);
    }

    // if nothing in the preferences, check app bundle
    if (dirArray.count()==0){
        dirName = getFullPathOfAppBundleMediaDirectory();
        dirArray = getDirectoryListing(dirName);
    }


    if (dirArray.count() >0) {
        NumberOfFiles = dirArray.count(); // the number of files in the bible director
        for (index=0; index<NumberOfFiles; index++) {
            FileName = dirArray [index];

            if (Instr(FileName, getSuffixStringFromTypeInt(TYPE_MEDIA_INFO_TYPE))> 0) {
                //gMediaModuleDatabase = [[MediaModuleDatabase alloc] init];
                gMediaModuleDatabase = new MediaModuleDatabase();
                gMediaModuleDatabase->databaseName = FileName;
                gMediaModuleDatabase->pathComponent = dirName+"/";
                gMediaModuleDatabase->Open();
                break;// finished
            }
        }
    }

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QList<BookHandlerClass *> sortBooksArray(QList<BookHandlerClass *>booksArray) {

    qStableSort( booksArray.begin(),booksArray.end(),BookHandlerClassLessThan);

    return booksArray;
}
// ------------------------------------------------------------------------
// commentary search path
// 1) application directory
// 2) preferences directory location
// 3) user Documents/CWP/Commentary directory
// 4) user Documents/CWP directory
// todo: explain in dialog
// ------------------------------------------------------------------------
void  initialization::initCommentaries()
{

    // from the app install location
    QStringList dirArray =getDirectoryListing(getFullPathOfAppBundleCommentaryDirectory());

    if (dirArray.count()>0)
      checkDirArray(dirArray, /*withPathComponent:*/getFullPathOfAppBundleCommentaryDirectory() , /*ForType:*/COMMENTARY_DATABASE_SUFFIX_TYPE , /*andReplaceInBooks:*/NO);

    // from preferences
    dirArray = getDirectoryListing(getFullPathOfPreferencesCommentaryDirectory());

    if (dirArray.count()>0)
      checkDirArray(dirArray, /*withPathComponent:*/getFullPathOfPreferencesCommentaryDirectory() , /*ForType:*/COMMENTARY_DATABASE_SUFFIX_TYPE , /*andReplaceInBooks:*/YES);

    // from user documents
    dirArray = getDirectoryListing(getFullPathOfUserDocumentCWPCommentaryDirectory());

    if (dirArray.count()>0)
        checkDirArray(dirArray , /*withPathComponent:*/getFullPathOfUserDocumentCWPCommentaryDirectory() , /*ForType:*/COMMENTARY_DATABASE_SUFFIX_TYPE , /*andReplaceInBooks:*/YES);

    // from Documents/CWP directory
    // the user can create notes in his CWP directory, and those can be read as commentaries
    dirArray = getDirectoryListing(getFullPathOfUserDocumentsCWPDirectory());
    if (dirArray.count()>0)
        checkDirArray(dirArray , /*withPathComponent:*/getFullPathOfUserDocumentsCWPDirectory() , /*ForType:*/COMMENTARY_DATABASE_SUFFIX_TYPE , /*andReplaceInBooks:*/YES);



    gNumberOfCommentaries  =0;
    QString FileName;
    int index;
    for (index=0; index < Books.count(); index++) {
        FileName = Books [index]->name;
            if ( Instr(FileName, getSuffixStringFromTypeInt(COMMENTARY_DATABASE_SUFFIX_TYPE))> 0) {
            gNumberOfCommentaries++;
        }
    }
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void initialization::initStrongsDatabase(){

        QStringList dirArray;
        QString DictionaryDBName="";
        BOOL found=NO;
        int index;

        gStrongsDatabase = new StrongsDatabaseClass() ;


        // Check the user Documents directory.  This one overrides all the others
        dirArray =getDirectoryListing(getFullPathOfUserDocumentsCWPDictionaryDirectory());
        if (dirArray.count()>0) {
            // Strongs Dictionary
            for (index=0; index < dirArray.count();index++) {
                if (dirArray[index].compare(gStrongsDatabase->databaseName)==0) {
                    found = YES;
                    gStrongsDatabase->databaseName = getFullPathOfUserDocumentsCWPDictionaryDirectory()+"/"+ gStrongsDatabase->databaseName;
                    break;
                }
            }
        }

        // if not found, check in the Preferences directory
        if (!found){
            // if user has chosen a preferences directory, should not be equal
            if (GetPreferencesCWPDirectory().compare(GetFullPathOfApplicationBundleDirectory())){
                dirArray =getDirectoryListing(getFullPathOfPreferencesDictionaryDirectory());
                if (dirArray.count()>0) {
                    // Strongs Dictionary
                    for (index=0; index < dirArray.count();index++) {
                        if (dirArray[index].compare(gStrongsDatabase->databaseName)==0) {
                            found = YES;
                            gStrongsDatabase->databaseName = getFullPathOfPreferencesDictionaryDirectory()+"/"+ gStrongsDatabase->databaseName;
                            break;
                        }
                    }
                }
            }
        }


        // if not found, check in the app bundle
        if (!found){
            // check in the App Bundle Directory
            dirArray =getDirectoryListing(getFullPathOfAppBundleCWPDictionaryDirectory());
            if (dirArray.count()>0) {
                // Strongs Dictionary
                for (index=0; index < dirArray.count();index++) {
                    if (dirArray[index].compare(gStrongsDatabase->databaseName)==0) {
                        found = YES;
                        gStrongsDatabase->databaseName = getFullPathOfAppBundleCWPDictionaryDirectory()+"/"+ gStrongsDatabase->databaseName;
                        break;
                    }
                }
            }
        }


        if (!found){
            QString StorageLoc;
            StorageLoc = getFullPathOfUserDocumentsCWPDirectory()+"/"+DICTIONARIES_DIRECTORY;
           MsgBox( QObject::tr("Could not find ")+ gStrongsDatabase->databaseName+ QObject::tr(".  Should be in \n'")+StorageLoc+"'");
           return;
        }

        gStrongsDatabase->Open();

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void initialization::initEnglishDictionaryDatabase() {

    QStringList dirArray;
    QString DictionaryDBName=ENGLISH_DICTIONARY_DB_NAME;
    BOOL found=NO;
    int index;


    gDictionaryDB = new dictionaryDB();

    // Check the user Documents directory.  This one overrides all the others
    dirArray =getDirectoryListing(getFullPathOfUserDocumentsCWPDictionaryDirectory());
    if (dirArray.count()>0) {
        // Strongs Dictionary
        for (index=0; index < dirArray.count();index++) {
            if (dirArray[index].compare(DictionaryDBName)==0) {
                found = YES;
                gDictionaryDB->initWithDBName(
                            getFullPathOfUserDocumentsCWPDictionaryDirectory()+"/"+DictionaryDBName,
                            /* andType*/TYPE_DICTIONARY_TYPE,
                            /*andDictionaryType:*/DICTIONARY_TYPE_ENGLISH_DICTIONARY);
                break;
            }
        }
    }

    // if not found, check in the Preferences directory
    if (!found){
        // if user has chosen a preferences directory, should not be equal
        if (GetPreferencesCWPDirectory().compare(GetFullPathOfApplicationBundleDirectory())){
            dirArray =getDirectoryListing(getFullPathOfPreferencesDictionaryDirectory());
            if (dirArray.count()>0) {
                for (index=0; index < dirArray.count();index++) {
                    if (dirArray[index].compare(DictionaryDBName)==0) {
                        found = YES;
                        gDictionaryDB->initWithDBName(
                                    getFullPathOfPreferencesDictionaryDirectory()+"/"+DictionaryDBName,
                                    /* andType*/TYPE_DICTIONARY_TYPE,
                                    /*andDictionaryType:*/DICTIONARY_TYPE_ENGLISH_DICTIONARY);
                        break;
                    }
                }
            }
        }
    }

    // if not found, check in the app bundle
    if (!found){
        // check in the App Bundle Directory
        dirArray =getDirectoryListing(getFullPathOfAppBundleCWPDictionaryDirectory());
        if (dirArray.count()>0) {
            // Strongs Dictionary
            for (index=0; index < dirArray.count();index++) {
                if (dirArray[index].compare(ENGLISH_DICTIONARY_DB_NAME)==0) {
                    found = YES;
                    gDictionaryDB->initWithDBName(
                                getFullPathOfAppBundleCWPDictionaryDirectory()+"/"+DictionaryDBName,
                                /* andType*/TYPE_DICTIONARY_TYPE,
                                /*andDictionaryType:*/DICTIONARY_TYPE_ENGLISH_DICTIONARY);
                    break;
                }
            }
        }
    }


    if (!found){
        QString StorageLoc;
        StorageLoc = getFullPathOfUserDocumentsCWPDictionaryDirectory();
       MsgBox( QObject::tr("Could not find ")+ DictionaryDBName+ QObject::tr(".  Should be in \n'")+StorageLoc+"'");
       return;
    }
    // ------------------------------------------------------------------------
    // end of websters
    // ------------------------------------------------------------------------
}
// ------------------------------------------------------------------------
// bookmarks only available in user documents directory
// todo: explain in dialog
// ------------------------------------------------------------------------
void initialization::initLibraryStuff() {

    initStrongsDatabase();

    StartupWindow_BibleBookChapter = "" ; // init the string

    // ------------------------------------------------------------------------
    // BOOKMARKS
    // only in User Documents CWP Directory
    // ------------------------------------------------------------------------
    if (!gBookMarkDB) {
        QString BibleBookMarkDBName= getFullPathOfUserDocumentsCWPDirectory() +"/"+ BIBLE_BOOK_MARKS_DB_NAME ;

        gBookMarkDB = new BookMarkDB();
        gBookMarkDB->initWithDBName( BibleBookMarkDBName,/* andType:*/TYPE_PREFERENCES_TYPE);
    }

    // ------------------------------------------------------------------------
    // WEBSTER's DICTIONARY
    // ------------------------------------------------------------------------
    initEnglishDictionaryDatabase();


}


// ------------------------------------------------------------------------
//  Notes are only in the user document's directory
// todo: explain in dialog
// ------------------------------------------------------------------------
void  initialization::initNotes()
{
    gNumberOfNotes =0;


    QStringList dirArray =getDirectoryListing(getNotesDirectory());
    int         index;
    NotesDBClass *bd;
    QString  FileName;
    int         NumberOfFiles;
    // BookHandlerClass *bhc;

    if (gNumberOfNotes > 0) {
        return; // we don't have to re-initialize the stuff.  we can release, then re-call this routine when memory gets tough
    }


    if (!dirArray.empty()) {
        NumberOfFiles = dirArray.count(); // the number of files in the bible director


        for (index=0; index<NumberOfFiles; index++) {
            FileName = dirArray [index];

            if (Instr(FileName, getSuffixStringFromTypeInt(TYPE_NOTES_TYPE))> 0) {
                bd = new NotesDBClass( );
                bd->pathComponent = getNotesDirectory();
                bd->Open(FileName);
                gNotes[gNumberOfNotes++]=bd;
                // [bd release]; // global gNotes still has it.
            }
        }
    }


    // create the note file if it does not exist
    if (gNumberOfNotes==0) {
        NotesDBClass *note = new NotesDBClass(DEFAULT_NOTES_FILE NOTE_DATABASE_SUFFIX);
        gNotes[gNumberOfNotes++]=note;
    }


}
//--------------------------------------------------------------------------
// three places for bibles and such
// todo: explain in dialog
//--------------------------------------------------------------------------
void  initialization::initBibles()
{

    QStringList dirArray;

    dirArray=getDirectoryListing(getFullPathOfAppBundleCWPBibleDirectory());
    if (dirArray.count()>0)
        checkDirArray(dirArray, /*withPathComponent:*/getFullPathOfAppBundleCWPBibleDirectory() , /*ForType:*/TYPE_BBL_TYPE , /*andReplaceInBooks:*/NO);

    //----------------------------------------
    // did user change where bibles are stored?
    //----------------------------------------
    dirArray = getDirectoryListing(getFullPathOfPreferencesBibleDirectory());
    // check the user's preferences for bibles.
    if (dirArray.count()>0)
        checkDirArray(dirArray, /*withPathComponent:*/getFullPathOfPreferencesBibleDirectory() , /*ForType:*/TYPE_BBL_TYPE , /*andReplaceInBooks:*/YES);


    // check the user's documents bible directory
    // Any Bibles here *will* *replace* the bibles in the previous locations
    dirArray = getDirectoryListing(getFullPathOfUserDocumentsCWPBibleDirectory());
    if (dirArray.count()>0)
        checkDirArray(dirArray , /*withPathComponent:*/getFullPathOfUserDocumentsCWPBibleDirectory() , /*ForType:*/TYPE_BBL_TYPE , /*andReplaceInBooks:*/YES);



    gNumberOfBibles  =0;
    QString FileName;
    int index;
    for (index=0; index < Books.count(); index++) {
        BibleDatabaseClass *bdcp ;
        bdcp =((BibleDatabaseClass*)((BookHandlerClass *)Books [index])->Database);
        FileName = bdcp->getFileName();
        if ( Instr(FileName, getSuffixStringFromTypeInt(BIBLE_DATABASE_SUFFIX_TYPE))> 0) {
            gNumberOfBibles++;
        }
    }
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void  initialization::initTextBooks() {
    if (!bmc)
        bmc = new BookManipulationClass();


    QStringList dirArray =getDirectoryListing(getApplicationBundleBooksDirectory());
    if (dirArray.count()>0)
        checkDirArray(dirArray,/* withPathComponent:*/getApplicationBundleBooksDirectory(), /*ForType:*/BOOK_DATABASE_SUFFIX_TYPE, /*andReplaceInBooks:*/NO);


    //----------------------------------------
    // did user change where bibles are stored?
    //----------------------------------------
    dirArray = getDirectoryListing(getFullPathOfPreferencesBooksDirectory());
    // check the user's preferences for bibles.
    if (dirArray.count()>0)
        checkDirArray(dirArray, /*withPathComponent:*/getFullPathOfPreferencesBooksDirectory() , /*ForType:*/BOOK_DATABASE_SUFFIX_TYPE , /*andReplaceInBooks:*/YES);

    dirArray = getDirectoryListing(getFullPathOfUserDocumentsCWPDirectory());
    if (dirArray.count()>0)
        checkDirArray(dirArray, /*withPathComponent:*/getFullPathOfUserDocumentsCWPDirectory(), /*ForType:*/BOOK_DATABASE_SUFFIX_TYPE, /*andReplaceInBooks*/YES);

    gNumberOfBooks  =0;
    QString FileName;
    int index;
    QString suffix =getSuffixStringFromTypeInt(BOOK_DATABASE_SUFFIX_TYPE);

    for (index=0; index < Books.count(); index++) {
        FileName = ((BibleDatabaseClass*)((BookHandlerClass *)Books [index])->Database)->_BibleName;
        if ( Instr(FileName, suffix)> 0) {
            ((BibleDatabaseClass*)((BookHandlerClass *)Books [index])->Database)->setDbType( TYPE_BOOK_TYPE);
            gNumberOfBooks++;
            gTextBooks.append(Books [index]);
        }
    }

}

