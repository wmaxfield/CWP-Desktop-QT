#ifndef BIBLEINFO_H
#define BIBLEINFO_H

/*
 *  BibleInfo.h
 *  CrosswordTry2
 *
 *  Created by WADE MAXFIELD on 9/23/10.
 *  Copyright 2010 Wade Maxfield. All rights reserved.
 * converted
 */
#include "misclibrary.h"

#define b01 (const char*)"Genesis"
#define b02 (const char*)"Exodus"
#define b03 (const char*)"Leviticus"
#define b04 (const char*)"Numbers"
#define b05 (const char*)"Deuteronomy"
#define b06 (const char*)"Joshua"
#define b07 (const char*)"Judges"
#define b08 (const char*)"Ruth"
#define b09 (const char*)"1 Samuel"
#define b10 (const char*)"2 Samuel"
#define b11 (const char*)"1 Kings"
#define b12 (const char*)"2 Kings"
#define b13 (const char*)"1 Chronicles"
#define b14 (const char*)"2 Chronicles"
#define b15 (const char*)"Ezra"
// the following changed due to c/c++ predefined b16()
#define book16 (const char*)"Nehemiah"
#define b17 (const char*)"Esther"
#define b18 (const char*)"Job"
#define b19 (const char*)"Psalms"
#define b20 (const char*)"Proverbs"
#define b21 (const char*)"Ecclesiastes"
#define b22 (const char*)"Song of Songs"
#define b23 (const char*)"Isaiah"
#define b24 (const char*)"Jeremiah"
#define b25 (const char*)"Lamentations"
#define b26 (const char*)"Ezekiel"
#define b27 (const char*)"Daniel"
#define b28 (const char*)"Hosea"
#define b29 (const char*)"Joel"
#define b30 (const char*)"Amos"
#define b31 (const char*)"Obadiah"
#define b32 (const char*)"Jonah"
#define b33 (const char*)"Micah"
#define b34 (const char*)"Nahum"
#define b35 (const char*)"Habakkuk"
#define b36 (const char*)"Zephaniah"
#define b37 (const char*)"Haggai"
#define b38 (const char*)"Zechariah"
#define b39 (const char*)"Malachi"
#define b40 (const char*)"Matthew"
#define b41 (const char*)"Mark"
#define b42 (const char*)"Luke"
#define b43 (const char*)"John"
#define b44 (const char*)"Acts"
#define b45 (const char*)"Romans"
#define b46 (const char*)"1 Corinthians"
#define b47 (const char*)"2 Corinthians"
#define b48 (const char*)"Galatians"
#define b49 (const char*)"Ephesians"
#define b50 (const char*)"Philippians"
#define b51 (const char*)"Colossians"
#define b52 (const char*)"1 Thessalonians"
#define b53 (const char*)"2 Thessalonians"
#define b54 (const char*)"1 Timothy"
#define b55 (const char*)"2 Timothy"
#define b56 (const char*)"Titus"
#define b57 (const char*)"Philemon"
#define b58 (const char*)"Hebrews"
#define b59 (const char*)"James"
#define b60 (const char*)"1 Peter"
#define b61 (const char*)"2 Peter"
#define b62 (const char*)"1 John"
#define b63 (const char*)"2 John"
#define b64 (const char*)"3 John"
#define b65 (const char*)"Jude"
#define b66 (const char*)"Revelation"


#define bs01 (const char*)"ge"
#define bs02 (const char*)"ex"
#define bs03 (const char*)"le"
#define bs04 (const char*)"nu"
#define bs05 (const char*)"de"
#define bs06 (const char*)"jo"
#define bs07 (const char*)"ju"
#define bs08 (const char*)"ru"
#define bs09 (const char*)"1sa"
#define bs10 (const char*)"2sa"
#define bs11 (const char*)"1ki"
#define bs12 (const char*)"2ki"
#define bs13 (const char*)"1ch"
#define bs14 (const char*)"2ch"
#define bs15 (const char*)"ez"

#define bs16 (const char*)"ne"
#define bs17 (const char*)"es"
#define bs18 (const char*)"jb"
#define bs19 (const char*)"ps"
#define bs20 (const char*)"pr"
#define bs21 (const char*)"ec"
#define bs22 (const char*)"so"
#define bs23 (const char*)"is"
#define bs24 (const char*)"je"
#define bs25 (const char*)"la"
#define bs26 (const char*)"ez"
#define bs27 (const char*)"da"
#define bs28 (const char*)"ho"
#define bs29 (const char*)"jl"
#define bs30 (const char*)"am"
#define bs31 (const char*)"ob"
#define bs32 (const char*)"jo"
#define bs33 (const char*)"mi"
#define bs34 (const char*)"na"
#define bs35 (const char*)"hb"
#define bs36 (const char*)"ze"
#define bs37 (const char*)"hg"
#define bs38 (const char*)"ze"
#define bs39 (const char*)"ma"
#define bs40 (const char*)"mt"
#define bs41 (const char*)"mk"
#define bs42 (const char*)"lk"
#define bs43 (const char*)"jn"
#define bs44 (const char*)"ac"
#define bs45 (const char*)"ro"
#define bs46 (const char*)"1co"
#define bs47 (const char*)"2co"
#define bs48 (const char*)"ga"
#define bs49 (const char*)"ep"
#define bs50 (const char*)"ph"
#define bs51 (const char*)"cl"

#define bs52 (const char*)"1th"
#define bs53 (const char*)"2th"

#define bs54 (const char*)"1tm"
#define bs55 (const char*)"2tm"
#define bs56 (const char*)"ti"
#define bs57 (const char*)"ph"
#define bs58 (const char*)"he"
#define bs59 (const char*)"Ja"

#define bs60 (const char*)"1pet"
#define bs61 (const char*)"2pet"
#define bs62 (const char*)"1jn"
#define bs63 (const char*)"2jn"
#define bs64 (const char*)"3jn"

#define bs65 (const char*)"jd"
#define bs66 (const char*)"re"




#define BOOKMARK_INDICATOR_DIRECT "B"
#define BOOKMARK_INDICATOR_IMAGE "<img src=\"file:/&@&/bookmarkw.png\" alt=\"\" width=\"10\" height=\"26\" >"

#define HTML_DICT_INDICATOR "D"

#define EDIT_INDICATOR_DIRECT "E"
#define EDIT_INDICATOR_DIRECT_C 'E'

#define CHAPTER_XREF "C"
#define CHAPTER_XREF_C 'C'

#define COPY_INDICATOR_DIRECT "C"
#define COPY_INDICATOR_DIRECT_C 'C'
#define COPY_INDICATOR_DIRECT_SWITCH_VAR 'C'
#define COPY_INDICATOR_DIRECT_SWITCH_VAR_STR "c"

#define FOOTNOTE_INDICATOR "F"
#define LANGUAGE_GREEK "G"
#define LANGUAGE_HEBREW "H"

#define COPYRIGHT_INDICATOR_SHOW_WINDOW "I"
#define COPYRIGHT_HTML_INDICATOR "R"

#define NOTES_INDICATOR_DIRECT "N"
#define SPEECH_INDICATOR_DIRECT "S"
#define USER_XREF   "U"             // user cross reference
#define USER_XREF_C   'U'             // user cross reference

#define HEADER_INDICATOR "D"


#define HTML_DICT_INDICATOR_C 'D'
#define LANGUAGE_GREEK_C 'G'
#define LANGUAGE_HEBREW_C 'H'
#define CHAPTER_XREF_C 'C'
#define CHAPTER_XREF_G_C 'G'
#define COPYRIGHT_HTML_INDICATOR_C 'R'
#define COPYRIGHT_INDICATOR_SHOW_WINDOW_C 'I'
#define BOOKMARK_INDICATOR_DIRECT_C 'B'
#define NOTES_INDICATOR_DIRECT_C 'N'

#define SPEECH_INDICATOR_DIRECT_C 'S'
#define FOOTNOTE_INDICATOR_C 'F'



#define BOOK_INFO_TABLENAME "BookInfoTable"
#define BOOK_NAME_COLUMN "BookName"
#define BOOK_NUMBER_COLUMN "BookNumber"
#define BOOK_ORDER_NUMBER_COLUMN "BookOrder"
#define BOOK_SNAME_COLUMN "BookShortName"
#define BOOK_VSNAME_COLUMN "BookVeryShortName"
#define CHAPTER_NUMBER_COLUMN "Chapter"
#define CHAPTER_VERSE_TABLENAME "ChapterVerseTable"
#define FOOTNOTE_NUMBER_COLUMN "FtnNmbr"
#define FOOTNOTE_TABLENAME "FootNoteTable"
#define FROM_STRONGS_GREEK "(From Strong\'s Greek)"
#define FROM_STRONGS_HEBREW "(From Strong\'s Hebrew)"
#define GREEK_TABLE "Greek"
#define HEBREW_TABLE "Hebrew"
#define JavaBookMark "bookmark"
// Top is the Y offset.  Left is the X offset.
// is relatively x and y coordinates, close enough to scroll to
#define javascriptScrollOffset "<script language=\"JavaScript\">\rfunction jumpScroll(anchor) \r{\r window.location.hash=anchor;\r }\r \
function ObjectPosition(objName) {\
var obj = document.getElementById(objName);\
var curtop = 0;\
var curleft = 0;\
if (obj && obj.offsetParent) {\
do {\
    if (!obj) break;\
   curleft += obj.offsetLeft;\
   curtop += obj.offsetTop;\
   } while (obj = obj.offsetParent);\
}\
var rslt=objName+\"+\"+curtop+\"+\"+curleft;\
return rslt;\
}\
function ObjectPositionLeft(objName) {\
var obj = document.getElementById(objName);\
var curleft = 0;\
if (obj && obj.offsetParent) {\
do {\
    if(!obj) break; \
   curleft += obj.offsetLeft;\
} while (obj = obj.offsetParent);\
}\
return curleft;\
}\
</script>\r\n\r\n"
/* wsm -- remove, was causing problems in Qt
 * Qt WebFrame can get scroll location
\
function pushPageLocation() {\
 var currentOffset = \"http://Z_\"+window.pageYOffset;\
 window.location = currentOffset;\
}\
onscroll = pushPageLocation;\
</script>\r\n\r\n"
*/

#define LANGUAGE_COLUMN "Language"

#define LAST_BOOK_IN_OT 39
#define LAST_BOOK_IN_BIBLE 66

#define MAXIMUM_GREEK_STRONGS 5624
#define MAXIMUM_HEBREW_STRONGS 8674
#define NOT_FOUND "not found"
#define NT_TABLE "NT"
#define NUMBER_CHAPTERS_COLUMN "NumberOfChapters"
#define NUMBER_VERSES_COLUMN "NumberOfVerses"
#define NUM_BOOKS_IN_BIBLE 66
#define OT_TABLE "OT"
#define SPAN_CLASS_ENGLISH "<span class\x3D\"english\">"
#define SPAN_CLASS_GREEK "\n       <span class\x3D\"greek\">" /*was ref greek*/
#define SPAN_CLASS_STRONGS "\n     <span class\x3D\"strongs\">"
#define SPAN_CLASS_UNIT "\n<span class\x3D\"unit\">"
#define STRONGS_CROSS1 "cross1"
#define STRONGS_CROSS2 "cross2"
#define STRONGS_CROSS3 "cross3"
#define STRONGS_DATABASE_SUFFIX ".str"
#define STRONGS_DEFINITION "Definition"
#define STRONGS_NUMBER_COLUMN "StrNu"
#define STRONGS_ORIG_LANGUAGE_COLUMN "Original"
#define STRONGS_PHONIC "Phonic"


#define t09 (const char*)"OneSam"
#define t10 (const char*)"TwoSam"
#define t11 (const char*)"OneKing"
#define t12 (const char*)"TwoKing"
#define t13 (const char*)"OneChr"
#define t14 (const char*)"TwoChr"
#define t22 (const char*)"SOS"
#define t46 (const char*)"OneCor"
#define t47 (const char*)"TwoCor"
#define t52 (const char*)"OneThes"
#define t53 (const char*)"TwoThes"
#define t54 (const char*)"OneTim"
#define t55 (const char*)"TwoTim"
#define t60 (const char*)"OnePeter"
#define t61 (const char*)"TwoPeter"
#define t62 (const char*)"OneJohn"
#define t63 (const char*)"TwoJohn"
#define t64 (const char*)"ThreeJohn"


#define TABLE_NAME_COLUMN (const char*)"TableName"



#define BookOfLawText       (const char*)"The Torah (The Law)"
#define BookOfHistoryText   (const char*)"History"
#define BookOfPoetryText    (const char*)"Poetry"
#define MajorProphetText    (const char*)"Major Prophets"
#define MinorProphetText    (const char*)"Minor Prophets"
#define GospelsText         (const char*)"Gospels"
#define BookOfChurchHistoryText (const char*)"Beginning of the Church"
#define PaulineEpistlesText (const char*)"Paul's Letters"
#define GeneralEpistlesText (const char*)"Other Letters"
#define BookOfProphecyText  (const char*)"The Revelation"

 enum BookTypesEnumList {
   BookOfLawType =      1
     ,BookOfHistoryType =  2
     ,BookOfPoetryType =   3
     ,MajorProphetType =   4
     ,MinorProphetType =   5
     ,GospelsType     =    6
     ,BookOfChurchHistoryType =  7
     ,PaulineEpistlesType= 8
     ,GeneralEpistlesType =9
     ,BookOfProphecyType = 10
}  ;

typedef enum BookTypesEnumList BookTypesEnum;

#define NumBooksInLaw               5
#define NumBooksInHistory           12
#define NumBooksInPoetry            5
#define NumBooksInMajorProphet      5
#define NumBooksInMinorProphet      12
#define NumBooksInGospels           4
#define NumBooksInPaulineEpistles   14
#define NumBooksInGeneralEpistles   7
#define NumBooksInChurchHistory     1
#define NumBooksInBookOfProphecy    1

//#include "structs.h"


QString BibleBook(int BookNumber);

BibleBookInfoStructType getAlphabeticalBibleBookInfo(int BookNumber);
BibleBookInfoStructType getBibleBookInfo(int BookNumber);
void GetBookChapterVerse(int BookNumber, BookChapterVerseStructType& bcv);
BibleBookInfoStructType getBibleBookInfoByName(QString bookName);
QString BibleBook(int BookNumber);
#define getBookName(a) BibleBook(a)  /* return the book name*/
#define getBookTableName(a) BibleBook(a) /* book name is table name */
extern QStringList BookNamesList;
QStringList getBookNamesList();

#endif // BIBLEINFO_H
