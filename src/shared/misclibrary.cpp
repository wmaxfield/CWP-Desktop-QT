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
//  miscLibrary.m
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/17/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//converted

#include <QApplication>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDate>
#include <QStandardPaths>

#include "cwpdefines.h"
#include "misclibrary.h"
#include "strongsdatabaseclass.h"
#include "PreferencesLibrary.h"
#include "BookMarkDB.h"
#include "notesLibrary.h"
#include "notesdbclass.h"
#include "preferencesmodule.h"

// SYSTEM GLOBALS



QStringList Bibles;// the bibles we have found in the file directory
int                         gNumberOfBibles;// number of the bibles found in the file directory

QList<BookHandlerClass *>   Books;// the bibles, commentaries, and books

QString StartupWindow_BibleBookChapter;


StrongsDatabaseClass        *gStrongsDatabase;

extern BookMarkDB                  *gBookMarkDB; // the database holding bookmarks (one per system) (initialization.m)
extern BookMarkDB                  *gBookBookMarkDB; // the database holding bookmarks (one per system) (initialization.m)

dictionaryDB                *gDictionaryDB;// the database holding the english dictionary (one per system)
dictionaryDB                *gBibleDictionaryDB; // the database holding the bible dictionary (one per system)



extern QString getPrefDBFullPath(void);
extern QString getFullPathOfDirectoryInsideBundleDirectory(QString DirName);
extern QString getFullPathOfAppBundleCWPDictionaryDirectory(void);
extern QString GetPreferencesCWPDirectory();
extern int     prefDirectoryCreated ; // when YES, the code had to create a directory.  Assume starting over
extern int     userDirectoryCreated;

int     prefDirectoryCreated = NO; // when YES, the code had to create a directory.  Assume starting over
int     userDirectoryCreated=NO;

// ------------------------------------------------------------------------
// return the directory the application is in
// ------------------------------------------------------------------------
QString GetFullPathOfApplicationBundleDirectory() {
    QString n ="";

      if (n.length()==0){
          return QCoreApplication::applicationDirPath();;
      } else {
          return n;
      }

}
// ------------------------------------------------------------------------
// return the directory Preferences points to.
// If preferences is not set up, reuturn the directory the App is in.
// ------------------------------------------------------------------------
QString GetPreferencesCWPDirectory() {
    QString n ="";
     if (Preferences)
          n= Preferences->getCWPDirectory();

     // preferences directory defaults to AppBundle
      if (n.length()==0){
          // this one should always exist, and can be created
          return getFullPathOfUserDocumentsCWPDirectory();
      } else {
          return n;
      }

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getFullPathOfPreferencesBibleDirectory(){

    return GetPreferencesCWPDirectory()+"/"+CROSSWORD_BIBLES_DIRECTORY_NAME;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getFullPathOfPreferencesBooksDirectory(){

    return GetPreferencesCWPDirectory()+"/"+CROSSWORD_BOOKS_DIRECTORY_NAME;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getFullPathOfPreferencesCommentaryDirectory(){

    return GetPreferencesCWPDirectory()+"/"+CROSSWORD_COMMENTARY_DIRECTORY_NAME;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getFullPathOfPreferencesDictionaryDirectory(){

    return GetPreferencesCWPDirectory()+"/"+CROSSWORD_DICTIONARY_DIRECTORY_NAME;
}
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
QString getFullPathOfPreferencesMediaDirectory(void)
{
    return GetPreferencesCWPDirectory()+"/"+ CROSSWORD_MEDIA_DIRECTORY_NAME;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL AddBibleToSelectedArrayByIndexNumber(int BibleNumber, int selectedIndex)
{
    BibleDatabaseClass *bbl;
    BookHandlerClass *bk;

    bk = Books[BibleNumber];
    if (bk) {
        if (bk->Database) {
            bbl = (BibleDatabaseClass *)(bk->Database);
            if (!bbl->isOpen) {
                bbl->Open();
            }

            Preferences->SelectedBooks[selectedIndex].Name=bbl->_BibleName;
            Preferences->SelectedBooks[selectedIndex].Type=bbl->getDbType();
            Preferences->SelectedBooks[selectedIndex].Index = BibleNumber;
            bbl->BibleIndexNumber = selectedIndex; // the index number in the selectedBooks[] array
            // save the selected book in the proper slot
            if (SavePrefVariable(BibleKey(selectedIndex+1), bbl->_BibleName)==NO)
                return NO;
            if (SavePrefVariable(DocTypeKey(selectedIndex+1),QString::number(bbl->getDbType()))==NO)//   str.sprintf("%d",Preferences->SelectedBooks[index].Type));
                return NO;
            return YES;
        }
    }
    if (selectedIndex >0 ){
        Preferences->SelectedBooks[selectedIndex].Index=-1;
    }

    return NO;
}


//----------------------------------------------------------------------------------------------------------------------------------
// return -1 for s1 less than s2
// return 0 for equal
// return 1 for s1 greater than s2
//----------------------------------------------------------------------------------------------------------------------------------
int CompareStrings(QString s1,QString s2)
{
    int r;

    r = QString::compare(s1,s2,Qt::CaseInsensitive);
    return r;

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void copyItemsFromDirectoryToDocumentsDirectory(QString startDirectoryName,QString destDirectoryName,BOOL CopyIfTrueLinkIfFalse)
{
    QStringList resourceArray;
    int index;
    QString destPath,sourcePath;
    //int *error;
    QStringList destArray;
    QFile f;

    destArray = getDirectoryListing(destDirectoryName);


    resourceArray =  getDirectoryListing(startDirectoryName);

    if (resourceArray.count()) {

        for (index=0; index < resourceArray.count(); index++) {

            sourcePath =resourceArray [index];

            // if the item is not in the destination directory, copy or link it.  Otherwise leave alone
            // we are making copies rather than soft links so the user can modify the item if necessary
            if ( !isNameInArray(sourcePath, destArray)) {

                destPath = destDirectoryName +"/"+sourcePath;

                sourcePath =startDirectoryName +"/"+sourcePath;
                if (CopyIfTrueLinkIfFalse) {
                    f.copy(sourcePath,destPath);
                    //[fileMgr copyItemAtPath:sourcePath toPath:destPath error:&error];
                } else {
                    f.remove(destPath);
                    //[fileMgr removeItemAtPath:destPath error:&error];
                    f.link(sourcePath,destPath);
                    //[fileMgr createSymbolicLinkAtPath:destPath withDestinationPath:sourcePath  error:&error];
                }
            }

        }
    }


}
// ------------------------------------------------------------------------
// count the number of available bibles
// ------------------------------------------------------------------------
int CountBibles(void)
{
    QStringList dirArray =getDirectoryListing(getBiblesDirectory());

    return dirArray.count(); // the number of files in the bible director
}


// ------------------------------------------------------------------------
// escape a string that may have ' in it so that it can be inserted in
// a sqlite query that uses ' as a string delimiter.
// eg, suppose the name is O'malley
// "insert into foo(name) values ('O'malley')" will fail, so using
// dbEscapeSingleQuoteString("O'malley") makes it O''malley, so it becomes
// insert into foo(name) values ('O''malley')"
// ------------------------------------------------------------------------
QString dbEscapeSingleQuoteString(QString inputString) {
    QString str;
    str = myReplace(inputString, QString("'"),QString("''"));
    return str;
}
#ifdef Q_OS_MAC
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
uint64_t DurationToNanoSecs(uint64_t Duration)
{
    mach_timebase_info_data_t info;
    mach_timebase_info(&info);

    //uint64_t start = mach_absolute_time();
    /* Do some code */
    //uint64_t duration = mach_absolute_time() - start;

    /* Convert to nanoseconds */
    Duration *= info.numer;
    Duration /= info.denom;

    /* Log the time */
    //NSLog("My amazing code took %lld nanoseconds!", Duration);
    return Duration;
}
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
uint64_t DurationToMilliSecs(uint64_t Duration)
{
    return (DurationToNanoSecs(Duration)/1000000); // divide by 1 million
}
#endif




// ------------------------------------------------------------------------
// The preferences database will be created if necessary, and MUST
// live within the user's Document directory on his machine
// ------------------------------------------------------------------------
QString getPrefDBFullPath(void)
{
    userDirectoryCreated=NO; // reset

    QString prefsDir = getDirectoryInsideUsersCWPDocumentsDirectory( CROSSWORD_PREFERENCES_DIRECTORY_NAME);

    if (userDirectoryCreated) {
        userDirectoryCreated=NO;
        prefDirectoryCreated=YES;
    }

    QString prefDBName = prefsDir +"/"+ "Preferences.db";
    return prefDBName;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getBookPrefDBName(void)
{
    QString prefsDir = getDirectoryInsideUsersCWPDocumentsDirectory( "Prefs");
    QString prefDBName = prefsDir+"/"+"BookPrefs.db";
    return prefDBName;
}





// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int getIndexOfBibleInBooks(QString bible)
{
    int index;
    BookHandlerClass *bk;
    BibleDatabaseClass *b;

    for (index=0; index < gNumberOfBibles; index++) {
         {
            if (Books.count()>=index+1) {
                bk = Books [index];
                if (bk->Database) {
                    b=(BibleDatabaseClass*)bk->Database;
                    if (CompareStrings(bible, b->_BibleName)==0) {
                        return index;
                    }
                }
            }
        }
    }

    return -1;
}




// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void addBibleToSelectedArrayByName(QString bible)
{
    int index;


    index = getIndexOfBibleInBooks(bible);
    if ( AddBibleToSelectedArrayByIndexNumber(index, Preferences->getNumberOfSelectedBooks()))
        Preferences->incrementNumberOfSelectedBooks();

}
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
void removeBookFromSelectedArrayByName(QString book)
{
    if (removeBibleFromSelectedArrayByName(book))
        return;

    removeNoteFromSelectedArrayByName(book);


}
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
BOOL removeBibleFromSelectedArrayByName(QString bible)
{
    int index,index2;
    BOOL foundit=NO;
    int numBooks = Preferences->getNumberOfSelectedBooks();
    for (index=0; index < numBooks; index++) {
        if (Preferences->SelectedBooks[index].Name== bible) {

            BookHandlerClass *bk = Books [Preferences->SelectedBooks[index].Index];
            bk->selected = NO;

            for (index2=index; index2 < numBooks-1; index2++) {
                Preferences->SelectedBooks[index2] = Preferences->SelectedBooks[index2+1];

            }
            Preferences->setNumberOfSelectedBooks(numBooks-1); //back out the number of books
            foundit = YES;
            break;
        }
    }
    //  saveSelectedBooks();

    return foundit;

}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
BOOL isBookNameInSelectedArray(QString book)
{
    if (isBibleNameInSelectedArray(book)) {
        return YES;
    }

    return (isNoteNameInSelectedArray(book));
}
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
BOOL isBibleNameInSelectedArray(QString bible)
{
    int index;
    QString selectBible;
    int numBooks = Preferences->getNumberOfSelectedBooks();

    for (index=0; index < numBooks; index++) {
        selectBible = Preferences->SelectedBooks[index].Name;
        if (StringsAreEqual(selectBible, bible)) {
            return YES;
        }
    }

    return NO;
}



//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
void
MsgBox(QString Msg)
{
    /* open an alert with an OK button */
   /*( UIAlertView *alert = [[UIAlertView alloc] initWithTitle:"Cross Word Message"
                                                    message:Msg
                                                   delegate:nil
                                          cancelButtonTitle:"OK"
                                          otherButtonTitles: nil];
    [alert show];
    [alert release];
    */
    QMessageBox m;
    m.setText(Msg);
    m.exec();
}





// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString GetWidthPercentage(int NumBooks)
{
    if (NumBooks ==0) NumBooks=1;
    float per = 100/NumBooks;
    extern float gVersion;
    if (gVersion) {
        per = per - (float)(NumBooks-1)*PERCENTAGE_SPACE_BETWEEN_COLUMNS;
    }

    QString str ;
    return str.sprintf("%f",per);
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int         getDatatabaseSuffixTypeFromString(QString TypeStr)
{
    if (TypeStr == BIBLE_DATABASE_SUFFIX  ) {
        return BIBLE_DATABASE_SUFFIX_TYPE;
    }

    if (TypeStr == COMMENTARY_DATABASE_SUFFIX  ) {
        return COMMENTARY_DATABASE_SUFFIX_TYPE;
    }

    if (TypeStr == DICTIONARY_SUFFIX  ) {
        return DICTIONARY_DATABASE_SUFFIX_TYPE;
    }

    if (TypeStr == NOTE_DATABASE_SUFFIX  ) {
        return NOTES_DATABASE_SUFFIX_TYPE;
    }

    return 0; // nothing found


}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int getTypeFromFileName(QString FileName)
{
    QStringList n;
    QString TypeStr="";

    n = mySplit(FileName,".");

    if (n.count()>1) {
        TypeStr = n [1];
    }

    if (TypeStr == BIBLE_DATABASE_SUFFIX  ) {
        return TYPE_BBL_TYPE;
    }

    if (TypeStr == COMMENTARY_DATABASE_SUFFIX  ) {
        return TYPE_BBL_TYPE;
    }

    if (TypeStr == DICTIONARY_SUFFIX  ) {
        return TYPE_DICTIONARY_TYPE;
    }

    if (TypeStr == NOTE_DATABASE_SUFFIX  ) {
        return TYPE_NOTES_TYPE;
    }

    if (TypeStr == MEDIA_INFO_DATABASE_SUFFIX  ) {
        return TYPE_MEDIA_INFO_TYPE;
    }

    if (TypeStr == PICTURE_INFO_DATABASE_SUFFIX  ) {
        return TYPE_PICTURE_INFO_TYPE;
    }

    return TYPE_BBL_TYPE; // nothing found



}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getSuffixStringFromTypeInt(int Type)
{
    switch (Type) {
        case NOTES_DATABASE_SUFFIX_TYPE:
            return  NOTE_DATABASE_SUFFIX;
            //break;

        case DICTIONARY_DATABASE_SUFFIX_TYPE:
            return  DICTIONARY_SUFFIX;
           // break;

        case COMMENTARY_DATABASE_SUFFIX_TYPE:
            return  COMMENTARY_DATABASE_SUFFIX;
            //break;

        case MEDIA_INFO_DATABASE_SUFFIX_TYPE:
            return  MEDIA_INFO_DATABASE_SUFFIX;

        case PICTURE_INFO_SUFFIX_TYPE:
            return  PICTURE_INFO_DATABASE_SUFFIX;

        case BOOK_DATABASE_SUFFIX_TYPE:
            return  BOOK_DATABASE_SUFFIX;

        case BIBLE_DATABASE_SUFFIX_TYPE:
        default:
            break;
    }
    return  BIBLE_DATABASE_SUFFIX;

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getSuffixStringFromTypeString(QString Type)
{
    return (getSuffixStringFromTypeInt(Type.toInt()));
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void *OpenBible(QString BibleName)
{
    BibleDatabaseClass *b;
    BookHandlerClass *bk;

    if (Instr(BibleName, ".bbl")) {
        BibleName = Left(BibleName, BibleName.length()-4);
    }

    int index;

    for (index=0; index < gNumberOfBibles; index++) {
        bk = Books [index];
        if (bk->Database) {
            b=(BibleDatabaseClass*)bk->Database;
            if (CompareStrings(BibleName, b->_BibleName)==0) {
                if (b->Open()==SUCCESS)
                    return b;
            }
        }
    }

    return nil;
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString DatabaseDirectory(void)
{
    return getFullPathOfUserDocumentsCWPDirectory();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL        FileExists(QString FilePath)
{
    QFileInfo f(FilePath);

    return (f.exists());


}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL isNameInArray(QString sourcePath,QStringList destArray)
{
    int index;

    for (index=0; index < destArray.count(); index++) {
        if (StringsAreEqual(sourcePath, destArray [index]))
            return YES;
    }

    return NO;

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL isType(QString fileName, int suffixType)
{
    QString fileSuffix;
    QStringList fname;

    if (!Instr(fileName, ".")) {
        return NO;
    }

    fname = mySplit(fileName,".");
    fileSuffix = fname [1];

    switch (suffixType) {
        case BIBLE_DATABASE_SUFFIX_TYPE:
            if (StringsAreEqual(fileSuffix,  BIBLE_DATABASE_SUFFIX)) {
                return YES;
            }
            break;

        case COMMENTARY_DATABASE_SUFFIX_TYPE:
            if (StringsAreEqual(fileSuffix,  COMMENTARY_DATABASE_SUFFIX)) {
                return YES;
            }
            break;

        case DICTIONARY_DATABASE_SUFFIX_TYPE:
            if (StringsAreEqual(fileSuffix,  DICTIONARY_SUFFIX)) {
                return YES;
            }
            break;

        case NOTES_DATABASE_SUFFIX_TYPE:
            if (StringsAreEqual(fileSuffix,  NOTE_DATABASE_SUFFIX)) {
                return YES;
            }
            break;


        default:
            return NO;
            //break;
    }

    return NO;
}

// ------------------------------------------------------------------------
// create directory if not exists, return path to it
// ------------------------------------------------------------------------
QString CreateDirectoryIfNotExist(QString PathToCreateDirectoryIn, QString DirectoryName)
{

    QString tmpPath = PathToCreateDirectoryIn +"/"+DirectoryName;

    QDir d(tmpPath);

   if ( d.exists())
       return tmpPath;

   if (!d.mkdir(tmpPath))
       return "";

   return tmpPath;
}
// ------------------------------------------------------------------------
// create directory if not exists, return path to it
// ------------------------------------------------------------------------
QString ReturnDirectoryIfExists(QString PathToOpenDirectoryIn, QString DirectoryName)
{

    QString tmpPath = PathToOpenDirectoryIn +"/"+DirectoryName;

    QDir d(tmpPath);

   if ( d.exists())
       return tmpPath;


   return "";
}
// ------------------------------------------------------------------------
// cannot create directory in App bundle, just return value
// ------------------------------------------------------------------------
QString getFullPathOfAppBundleCWPDictionaryDirectory(void)
{
    QString bp= getFullPathOfAppBundleCWPDirectory();
    QString fp = bp +"/"+"Dictionaries";
    return fp;

}

//---------------------------------------------------------------------------------------------------------------------------------
// cannot create directory in App bundle, just return value
//---------------------------------------------------------------------------------------------------------------------------------
QString getFullPathOfAppBundleCWPMediaDirectory(void) {

    return getFullPathOfAppBundleCWPDirectory()+"/Media";
}
//---------------------------------------------------------------------------------------------------------------------------------
// can't create directory in app bundle
// cannot create directory in App bundle, just return value
//---------------------------------------------------------------------------------------------------------------------------------
QString getFullPathOfAppBundleCWPDirectory(){
#if defined(Q_OS_MAC)
    return GetFullPathOfApplicationBundleDirectory()+"/../Resources/"+CROSSWORD_PROJECT_APP_BUNDLE_FILES_DIRECTORY;
#elif defined (Q_OS_WIN)
    return GetFullPathOfApplicationBundleDirectory()+"/"+CROSSWORD_PROJECT_APP_BUNDLE_FILES_DIRECTORY;
#endif
    return GetFullPathOfApplicationBundleDirectory()+"/"+CROSSWORD_PROJECT_APP_BUNDLE_FILES_DIRECTORY;
}
//---------------------------------------------------------------------------------------------------------------------------------
// cannot create directory in App bundle, just return value
//---------------------------------------------------------------------------------------------------------------------------------
QString getFullPathOfAppBundleCommentaryDirectory(void)
{
    return getFullPathOfAppBundleCWPDirectory()+"/"+CROSSWORD_COMMENTARY_DIRECTORY_NAME;
}

// ------------------------------------------------------------------------
// by default, the CWP directory is in the same directory as the application
// return the directory the Application is in with Bibles post-pended.
// cannot create directory in App bundle, just return value
// ------------------------------------------------------------------------
QString getFullPathOfAppBundleCWPBibleDirectory(void)
{
    return getFullPathOfAppBundleCWPDirectory()+"/"+CROSSWORD_BIBLES_DIRECTORY_NAME;
}

//----------------------------------------------------------------------------------------------------------------------------------
// cannot create directory in App bundle, just return value
//----------------------------------------------------------------------------------------------------------------------------------
QString getFullPathOfAppBundleMediaDirectory(void)
{
    QString bp= GetFullPathOfApplicationBundleDirectory();
    QString fp = bp +"/"+"Media";
    return fp;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getFullPathOfUserDocumentsCWPMediaDirectory(void) {

    return CreateDirectoryIfNotExist(getFullPathOfUserDocumentsCWPDirectory(), CROSSWORD_MEDIA_DIRECTORY_NAME);

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getFullPathOfUserDocumentsCWPDictionaryDirectory(void) {
    return CreateDirectoryIfNotExist(getFullPathOfUserDocumentsCWPDirectory(), CROSSWORD_DICTIONARY_DIRECTORY_NAME);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getFullPathOfUserDocumentsCWPBibleDirectory(void) {

    return CreateDirectoryIfNotExist(getFullPathOfUserDocumentsCWPDirectory(),CROSSWORD_BIBLES_DIRECTORY_NAME);
}

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
QString getApplicationBundleBooksDirectory(void)
{
    QString bp= GetFullPathOfApplicationBundleDirectory();
    QString fp = bp +"/"+"Books";
    return fp;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getFullPathOfUserPrefsDictionaryDirectory(){
    return GetPreferencesCWPDirectory()+"/"+ CROSSWORD_DICTIONARY_DIRECTORY_NAME;
}
// ------------------------------------------------------------------------
// return the user's documents location prepended to the CWP base directory.
// This directory WILL ALWAYS be created and MUST exist.
// ------------------------------------------------------------------------
QString getFullPathOfUserDocumentsCWPDirectory (void)
{
     QString d1 = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

     return CreateDirectoryIfNotExist(d1,CROSSWORD_PROJECT_DIRECTORY);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getFullPathOfUserDocumentCWPCommentaryDirectory(void) {

    return CreateDirectoryIfNotExist(getFullPathOfUserDocumentsCWPDirectory(),CROSSWORD_COMMENTARY_DIRECTORY_NAME);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getFullPathOfDirectoryInsideCWPDirectory(QString directoryName){

    return CreateDirectoryIfNotExist(getFullPathOfUserDocumentsCWPDirectory(),directoryName);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getFullPathOfDirectoryInsideBundleDirectory(QString DirName)
{
    return GetFullPathOfApplicationBundleDirectory()+"/"+DirName;
}
// ------------------------------------------------------------------------
// return the location of the directory requested with the CWP directory.
// create it if it does not exist.
// ------------------------------------------------------------------------
QString getDirectoryInsideUsersCWPDocumentsDirectory(QString DirName)
{
     QString Directory= getFullPathOfUserDocumentsCWPDirectory() +"/"+DirName;

    QDir d(Directory);

    if ( d.exists())
        return Directory;

    if (!d.mkdir(Directory))
        return "";

    userDirectoryCreated = YES;
    return Directory; // return the created directory

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getImageDirectory(void) {
    return getFullPathOfDirectoryInsideCWPDirectory("images");
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getDictionaryDirectory(void)
{

    return getFullPathOfDirectoryInsideBundleDirectory(CROSSWORD_DICTIONARY_DIRECTORY_NAME);


}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getBiblesDirectory(void)
{
    return getFullPathOfDirectoryInsideBundleDirectory(CROSSWORD_BIBLES_DIRECTORY_NAME);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getCommentaryDirectory(void)
{
    return getFullPathOfDirectoryInsideBundleDirectory(CROSSWORD_COMMENTARY_DIRECTORY_NAME);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getNotesDirectory(void)
{
    return getFullPathOfUserDocumentsCWPDirectory();


}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString getResourceDirectory(void)
{

    QString resourcePath = QApplication::applicationDirPath();
    return resourcePath;
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QDateTime getFileModificationDate(QString path)
{
    //NSError *error;

    //fmInit();
   /* NSDictionary *attr = [fileMgr attributesOfItemAtPath:path error:&error];
    if (attr != nil) {
        return [attr fileModificationDate];
    }
    return [NSDate dateWithTimeIntervalSinceReferenceDate:0];// really old
    */
    QFileInfo f(path);
    return f.lastModified();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QStringList getDirectoryListing(QString path)
{
     QDir d(path);
     QStringList l = d.entryList(QStringList("*"),QDir::Files | QDir::NoSymLinks,QDir::Name);
    return l;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int Instr(QString InString, QString subString)
{
    /*NSRange loc;
    if (!subString || !InString) {
        return 0;
    }
    loc = [InString rangeOfString:subString options:(NSCaseInsensitiveSearch)];
    if (loc.location == NSNotFound) {
        return 0;
    }

    return loc.location+1; // return 1 more than position
   */
    return InString.indexOf(subString,0,Qt::CaseInsensitive)+1;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int Instrp(QString InString, QString subString,int Start)
{
   return InString.indexOf(subString,Start,Qt::CaseInsensitive)+1;
}




// ------------------------------------------------------------------------
// return  characters from InString from position 0, lenght Length
// ------------------------------------------------------------------------
QString Left(QString InString, int Length)
{
    QString newStr;

    if (Length > InString.length()) {
        Length = InString.length();
    }

    if (!Length) {
        return "";
    }

    newStr = InString.left(Length);
    return newStr;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// return middle of a string
QString Mid(QString Source, int Start, int length)
{
    return Source.mid(Start,length);
}

// ------------------------------------------------------------------------
// this is a public function, used by First and SecondView controllers.
// ------------------------------------------------------------------------
UIInterfaceOrientation normalizeOrientation(UIInterfaceOrientation fromInterfaceOrientation)
{
    Q_UNUSED(fromInterfaceOrientation);
    return 0;
    /*
    // normalize the orientations so we can key on only two orientations
    if (fromInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown) {
        fromInterfaceOrientation = UIInterfaceOrientationPortrait;
    }

    if (fromInterfaceOrientation == UIInterfaceOrientationLandscapeRight) {
        fromInterfaceOrientation = UIInterfaceOrientationLandscapeLeft;
    }


    return fromInterfaceOrientation;
*/
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString endOfLine(void)
{
    return "\n";
}




extern void GetBookChapterVerse(int BookNumber, BookChapterVerseStructType& bcv);


//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
int decrementChapter(int book,int chapter)
{
    chapter--;
    if (chapter < 1) {
        BookChapterVerseStructType bcv;
        GetBookChapterVerse(book,bcv);
        chapter = bcv.NumberOfChapters;
    }
    return chapter;

}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
int incrementChapter(int book,int chapter)
{
    BookChapterVerseStructType bcv;
    GetBookChapterVerse(book,bcv);

    chapter++;
    if (chapter >bcv.NumberOfChapters) {
        chapter = 1;
    }
    return chapter;

}
QString    UpperCaseString(QString s){
    s.replace(0,1,s[0].toUpper());
    return s;
}
QString LowerCaseString(QString s){
    s.replace(0,1,s[0].toLower());
  return s;
}
// ------------------------------------------------------------------------
// remove all characters between start string and end string
// ------------------------------------------------------------------------
QString RemoveAll(QString Source, QString StartString, QString EndString)
{
    QString NewString;
    int location;
    int length;

    int StartStringIndex = Instr(Source, StartString);

    if (StartStringIndex== 0)
        return Source;

    location = StartStringIndex-1; // the location of the start string

    int EndStringIndex = Instrp(Source, EndString, location);//(Source, EndString);

    if (EndStringIndex==0)
        return Source;

    length = EndStringIndex-StartStringIndex+EndString.length();
    NewString = Source.remove(location,length);// stringByReplacingCharactersInRange:loc withString:""];
    return NewString;
}
// remove the first Start characters from InString and return the new string
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString Right(QString InString, int Start)
{
    QString newStr;
    if (Start > InString.length()) {
        return "";
    }
    newStr = InString.right(InString.length()-Start);
    return newStr;
}
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
int StringsAreEqual(QString s1,QString s2)
{
    if (s1==nullptr || s2==nullptr) {
        return NO;
    }
    return CompareStrings(s1, s2)==0; // true if equal
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString SuffixFromType(int SuffixType)
{
    switch (SuffixType) {
        case BIBLE_DATABASE_SUFFIX_TYPE:
            return BIBLE_DATABASE_SUFFIX;
            break;

        case COMMENTARY_DATABASE_SUFFIX_TYPE:
            return COMMENTARY_DATABASE_SUFFIX;
            break;

        case DICTIONARY_DATABASE_SUFFIX_TYPE:
            return DICTIONARY_SUFFIX;
            break;

        case NOTES_DATABASE_SUFFIX_TYPE:
            return NOTE_DATABASE_SUFFIX;
            break;


        default:
            return BIBLE_DATABASE_SUFFIX;
            break;
    }

}


QString trim(QString s)
{
   return s.trimmed();
}
