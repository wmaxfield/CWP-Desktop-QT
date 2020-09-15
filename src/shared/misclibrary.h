#ifndef MISCLIBRARY_H
#define MISCLIBRARY_H
//
//  miscLibrary.h
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/17/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//
#include <QString>
//#include <QDateTime>
//#include <Qlist>

//#include "URLCacheAlert.h"

#include "preferencesmodule.h"
#include "../shared/bookhandlerclass.h"
#include "biblemodule.h"
#ifdef Q_OS_MAC
#include <mach/mach_time.h>
#endif
#include "commentarydatabaseclass.h"
#include "dictionarydb.h"

#include "cwpdefines.h"

#define NSDocumentDirectory "/"  /*!ALERT! we need to define the document directory*/

#define CROSSWORD_PROJECT_DIRECTORY "CWP"
#define CROSSWORD_PROJECT_APP_BUNDLE_FILES_DIRECTORY "CWPfiles"
#define CROSSWORD_BIBLES_DIRECTORY_NAME "Bibles"
#define CROSSWORD_COMMENTARY_DIRECTORY_NAME "Commentaries"
#define CROSSWORD_DICTIONARY_DIRECTORY_NAME "Dictionaries"
#define CROSSWORD_PREFERENCES_DIRECTORY_NAME "Prefs"
#define CROSSWORD_MEDIA_DIRECTORY_NAME   "Media"
#define CROSSWORD_BOOKS_DIRECTORY_NAME   "Books"

#define HEADING_TYPE 12345678

QString     SuffixFromType(int SuffixType);
BOOL        isType(QString fileName, int suffixType);


BOOL        AddBibleToSelectedArrayByIndexNumber(int BibleNumber,int SelectedBibleIndex);// and open it


QString     CreateDirectoryIfNotExist(QString PathToCreateDirectoryIn, QString DirectoryName);
QString     ReturnDirectoryIfExists(QString PathToOpenDirectoryIn, QString DirectoryName);
int         CompareStrings(QString s1,QString s2);

//------------------------------------
#define DOCOPY YES
#define DOLINK NO
void        copyItemsFromDirectoryToDocumentsDirectory(QString startDirectoryName,QString destDirectoryName,BOOL DoACopyIfTrueLinkIfFalse);
//------------------------------------


QString     DatabaseDirectory(void);
QString     dbEscapeSingleQuoteString(QString inputString);

#ifdef Q_OS_MAC
uint64_t    DurationToNanoSecs(uint64_t Duration);
uint64_t    DurationToMilliSecs(uint64_t Duration);
#endif

QString     endOfLine(void);

void        fmInit(void);
void        fmRelease(void);

BOOL        FileExists(QString FilePath);

QString     getFullPathOfAppBundleCommentaryDirectory(void);
int         getDatatabaseSuffixTypeFromString(QString TypeStr);

QString     getNotesDirectory(void);
QString     getResourceDirectory(void);

QString     getFullPathOfAppBundleCWPDictionaryDirectory(void);
QString     getApplicationBundleBooksDirectory(void);
QString     getBiblesDirectory(void);

QString     getCommentaryDirectory(void);


QString     getDictionaryDirectory(void);
QString     getFullPathOfUserDocumentsCWPDirectory (void);
QString     getFullPathOfUserDocumentCWPCommentaryDirectory();

QString     getFullPathOfUserDocumentsCWPDirectory(void);
QString     getFullPathOfUserPrefsDictionaryDirectory();
QString     getImageDirectory();

QString     GetFullPathOfApplicationBundleDirectory();
QString     getFullPathOfAppBundleCWPMediaDirectory(void);
QString     getFullPathOfAppBundleCWPDirectory();

QString     getPrefDBFullPath(void);
QString     getFullPathOfDirectoryInsideBundleDirectory(QString DirName);
QString     getFullPathOfDirectoryInsideCWPDirectory(QString DirName);
QString     getFullPathOfPreferencesBibleDirectory();
QString     getFullPathOfPreferencesDictionaryDirectory();
QString     getFullPathOfPreferencesBooksDirectory();
QString     getFullPathOfPreferencesMediaDirectory();
QString     getFullPathOfPreferencesCommentaryDirectory();
QString     GetPreferencesCWPDirectory();

QString     getFullPathOfUserDocumentsCWPDictionaryDirectory(void);
QString     getFullPathOfUserDocumentsCWPMediaDirectory(void);
void *      getTabBarController(void); // in Crossword_Try3AppDelegate.m
int         getTypeFromFileName(QString FileName);

QString     getDirectoryInsideUsersCWPDocumentsDirectory(QString DirName);


//BibleBookInfoStructType getBibleBookInfo(int BookNumber);
//BookChapterVerseStructType GetBookChapterVerse(int BookNumber);

QString     GetWidthPercentage(int NumBooks);

QStringList getDirectoryListing(QString path);

//#include <QDateTime>
class QDateTime; // prevent compile errors
QDateTime       getFileModificationDate(QString path);
QString     getSuffixStringFromTypeString(QString Type);
QString     getSuffixStringFromTypeInt(int Type);


int         incrementChapter(int book,int chapter);
int         decrementChapter(int book,int chapter);
BOOL        isBookNameInSelectedArray(QString book);
extern BOOL isRunningInSimulator; // initialization.m
BOOL        isNameInArray(QString sourcePath,QStringList destArray);


//QString     join(QStringList *array,QString JoinString);


void        MsgBox(QString Msg);
UIInterfaceOrientation normalizeOrientation(UIInterfaceOrientation fromInterfaceOrientation);

void          *OpenBible(QString BibleName);


//----------------------------------------------------------------------------

void        ReleaseLibraryStuff(void);
//----------------------------------------------------------------------------

QString Left(QString InString, int Offset);
QString Right(QString InString, int Start);
int     Instr(QString InString, QString subString);// returns position of text
//extern QString a;
//#define Instr(a,b)  a.indexOf(b,0,Qt::CaseInsensitive)

int     Instrp(QString InString, QString subString,int Start);// returns position of text after Start
extern
QString Mid(QString Source, int Start, int length);

extern
QString RemoveAll(QString Source, QString StartString, QString EndString);
extern
int     StringsAreEqual(QString s1,QString s2);
extern
QString    UpperCaseString(QString s);
extern
QString    LowerCaseString(QString s);
extern QString StartupWindow_BibleBookChapter;

QString trim(QString s);

int     DatatabaseSuffixTypeFromString(QString TypeStr);
QString getSuffixStringFromTypeString(QString Type);

QString getFullPathOfAppBundleCWPBibleDirectory(void);// get the bibles directory in the bundle
QString getFullPathOfAppBundleCWPDictionaryDirectory(void);
QString getFullPathOfAppBundleMediaDirectory(void);
QString getFullPathOfUserDocumentsCWPBibleDirectory(void) ; // get the bibles directory in the user's CWP directory
//----------------------------------------------------------------------------
void        TurnOffCocoaURLLoadingCache(void);
//----------------------------------------------------------------------------

//extern NSFileManager    *fileMgr;       // current global file manager
//extern QStringList      Bibles;        // array of possible bible names (for the popover)
extern int              gNumberOfBibles; //number of bibles available to the system
class BookHandlerClass;
extern QList<BookHandlerClass *>      Books;
extern dictionaryDB                *gDictionaryDB;// the database holding the english dictionary (one per system)
extern dictionaryDB                *gBibleDictionaryDB;// the database holding the bible dictionary (one per system)


//BibleBookInfoStructType getBibleBookInfo(int BookNumber);
extern PreferencesModuleClass *Preferences;

extern BOOL isBibleNameInSelectedArray(QString bible);
extern BOOL removeBibleFromSelectedArrayByName(QString bible);
extern void removeBookFromSelectedArrayByName(QString book);
extern void addBibleToSelectedArrayByName(QString bible);


#define mySplit( s1,s2) s1.split(s2,QString::KeepEmptyParts,Qt::CaseInsensitive)
// make case insensitive so only has to be done once
#define myReplace(s1,s2,s3)  s1.replace(s2,s3,Qt::CaseInsensitive)
//[s1 stringByReplacingOccurrencesOfString:s2 withString:s3]
#define str(a) QString("%d").arg(a) // only works for decimal

#define UBound(a) (a.count()-1)
#if __cplusplus <= 199711L
    #ifndef nullptr
    #define nullptr 0
    #endif
#endif


#endif // MISCLIBRARY_H
