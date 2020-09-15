#ifndef PREFERENCESLIBRARY_H
#define PREFERENCESLIBRARY_H

//
//  PreferencesLibrary.h
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 10/4/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//
#include <QString>
//#include "notesdb.h"  // for notes file definition
#include "cwpdefines.h"


#define INI_PREF_VERSION "PrefVersion"  // the database version

#define BOOK_LOC_SEPARATOR ":"
#ifndef CATEGORY_PREFERENCE
#define CATEGORY_PREFERENCE "pref"
#endif

#define CATEGORY_PARAMS     "cparams"

#define INI_SHOW_COPY_TAG "CopyTagShow"
#define INI_ENABLE_BOOKMARK_TAG "BookMarkShow"

#define INI_ENABLE_ENGLISH_DICTIONARY "EnglishDictionary"

#define INI_DEBUG  "debug"

#define INI_R_COLOR "red"
#define INI_G_COLOR "green"
#define INI_B_COLOR "blue"

#define COULD_NOT_FIND_MEDIA_FOLDER_AUTO "Could not find the media folder.  If your version includes pictures, then select \"Media Preferences\" from the \"Manage\" menu and locate the folder."
#define DATE_OF_LAST_VERSION_CHECK "VersionCheckDate"
#define DEFAULT_COL_NAME "Name"
#define DEFAULT_COL_SIZE "Size"
#define DONT_SHOW_ "0"
#define ERROR_INSERT_TABLE1 "There was an error inserting a table into the Project File.  It could not be written to.  The directory may not be writeable.  Please fix the problem, and try again."
#define FORM_SPIN_ERR_MSG "An error occured saving a form spin variable.  The error was "
#define GET_STRING "GET"


#define INI_ADVANCED_HTML "AdvancedHTML"
#define INI_BOOK "Book"
#define INI_BOOKSWINDOW_HEIGHT "BooksWindowHeight"
#define INI_BOOKSWINDOW_LEFT "BooksWindowLeft"
#define INI_BOOKSWINDOW_TOP "BooksWindowTop"
#define INI_BOOKSWINDOW_WIDTH "BooksWindowWidth"
#define INI_BOOKS_BOOK "BooksBookName"
#define INI_BOOKS_CHAPTER "BooksChapter"
#define INI_BOOKS_PARAGRAPH "BooksParagraph"
#define INI_BOOK_LOC "LOC_BOOK_"


#define INI_CHAPTER "chapter"
#define INI_CHECK_VERSION "EnableCheckVersion"
#define INI_CREATE_TABLES "CreateTables"
#define INI_CWP_DIRECTORY "CWPDirectory"

#define INI_DATABASE_PATH "BookDatabaseRootFolder"
#define INI_DB_SELECT "DBSelection"
#define INI_DELETE "Delete"
#define INI_DESC_FIELD "DescriptionField"
#define INI_DONT_SHOW "DontShow"
#define INI_DO_STACK_VERSES "StackVerses"
#define INI_DROP_DATABASE "DopDatabase"
#define INI_DROP_TABLES "DropTables"

#define INI_ENABLE_SHOW_PICTURES "ShowPictures"
#define INI_ENABLE_VERSE_EDIT "VerseEditEnabled"


#define INI_FIELD_INS "INS"
#define INI_FIELD_SIZE "Size"
#define INI_FIELD_TYPE "Type"
#define INI_FONT_SIZE "FontSize"


#define INI_HIDE_NAV_BUTTONS "HideNavigationButtons"
#define INI_HIDE_STRONGS_NUMBERS "HideStrongsNumbers"
#define INI_INSERT "Insert"


#define INI_LEFT "Left"
#define INI_LOCK_BOOKS "LockBooksWindow"

#define INI_MAIN_WINDOW_XYWH  "MainWindowXYWH"  // geometry just before close
#define INI_MAX_TABLES_ACROSS "MaxTablesAcross"
#define INI_MEDIA_FOLDER "MediaFolder"
#define INI_MULTIMEDIA_ENABLED "Multimedia"

#define INI_BIBLE_DICTIONARY_XYWH "BibleDictionaryXYWH"
#define INI_BIBLE_DICTIONARY_SEARCH_TEXT "BibleDictionarySearchText"

#define INI_NOTES_FILE_NAME "NotesFileName"
#define USER_NOTES_FILE_DEFAULT_NAME DEFAULT_NOTES_FILE
#define INI_NUM_SELECTED_NOTES "NumSelectedNotes"
#define INI_NUM_SELECTED_BOOKS "NumSelectedBooks"



#define INI_NOTES_PREF "NotesBookMarkPreferences"
#define INI_NUMBER_OF_BOOKS "NumberOfBooks"
#define INI_NUM_COLUMNS "NumberOfColumns"
#define INI_NUM_SELECTED_COMMENTARIES "NumCommentaries"
#define INI_NUM_FIELDS "NumberOfFields"
#define INI_NUM_TABLES "NumberOfTables"

#define INI_OTHER_TABLE "OtherTable"
#define INI_OTHER_TABLE_DESC "OtherTableDesc"
#define INI_OTHER_TABLE_FIELD "OtherTableField"
#define INI_OUPUT_TO_FILE "OutputToFile"
#define INI_OUTPUT_FILENAME "SQLFileName"
#define INI_OUTPUT_TO_CLIPBOARD "OutputToClipboard"
#define INI_OUTPUT_TO_WINDOW "OutputToWindow"

#define INI_PROJECT_AUTHOR "Author"
#define INI_PROJECT_INFO "ProjectInfo"
#define INI_PROJECT_NAME "Name"



#define INI_REMEMBER_WINDOWS "RememberWinPos"



#define INI_SELECT "Select"
#define INI_SERVER_INFO "ServerInfo"
#define INI_SERVER_NAME "TestServer"
#define INI_SERVER_PWD "TestPwd"
#define INI_SERVER_USER "TestUser"
#define INI_SHOW_STRONGS "ShowStrongs"
#define INI_SPEAK_FULL_CHAPTER "SpeakFullChapter"
#define INI_SPEECH_VOICE "MacVoice"
#define INI_SPEECH_VOICE_LINUX "VoiceLinux"
#define INI_SPEECH_ENABLED "SpeechEnabled"
#define INI_SPOKEN_HELP_ENABLED "SpokenHelpEnabled"
#define INI_SQL "SQL"
#define INI_SQL_INFO "SQLInfo"
#define INI_STANDARD_TEXT_LENGTH "StandardTextLength"
#define INI_STANDARD_TEXT_WIDTH "StandardTextWidth"
#define INI_STARTWINDOW_HEIGHT "StartWindowHeight"
#define INI_STARTWINDOW_LEFT "StartWindowLeft"
#define INI_STARTWINDOW_TOP "StartWindowTop"
#define INI_STARTWINDOW_WIDTH "StartWindowWidth"
#define INI_STRONGS_WORD_COLOR "StrongsWordColor"

#define INI_TABLE "Table"
#define INI_TABLE_COMMENT "TableComment"
#define INI_TABLE_INFO "TableInfo"
#define INI_TABLE_NAMES "TableName"
#define INI_TEMPLATE_RB "RBProject"
#define INI_TEMPLATE_WEB "WebTemplate"
#define INI_THIS_TABLE_FIELD "ThisTableField"
#define INI_TOP "Top"

#define INI_UPDATE "Update"
#define INI_USE_ALTERNATE_DATABASE_PATH "UseAlternateDBPath"
#define INI_USE_DEFAULT_DATABASE_PATH "UseDefaultDBPath"

#define INI_VERSE "Verse"
#define INI_VIEW "View"
#define INI_VOICE_PITCH "VoicePitch"
#define INI_VOICE_SPEED "VoiceSpeed"
#define INI_VOICE_VARIANCE "VoiceVariance"
#define INI_VOICE "Voice"

#define INI_WEBGEN_CREATE_AUTH "WegenCreateSecurity"
#define INI_WEBGEN_CREATE_DELETE "WebgenCreateDeletePages"
#define INI_WEBGEN_CREATE_INCLUDE_DIRECTORY "WebgenCreateIncludeDirectory"
#define INI_WEBGEN_CREATE_UPDATE "WebgenCreateUpdPages"
#define INI_WEBGEN_CREATE_VIEW "WebgenCreateViewPages"
#define INI_WEBGEN_DatabaseMachine "Server"
#define INI_WEBGEN_DatabasePassword "pwd"
#define INI_WEBGEN_DatabaseUserName "User"
#define INI_WEBGEN_DEBUG "DebugWebPages"


#define MAX_TABLES_ACROSS "3"
#define MEDIA_INFO_DB_NAME "MediaInfo.minf"
#define MIN_HORIZ_TABLE_OFFSET "60"
#define MIN_VERTICAL_TABLE_SPACING "20"
#define OTHER_TABLE_ "1"
#define OTHER_TABLE_DESC_ "3"
#define OTHER_TABLE_FIELD_ "2"
#define POST_STRING "POST"
#define SAVE_FIELD_ERROR "An error occured saving a field.  The error was"
#define SAVE_TABLE_ERROR "An error occured saving a table.  The error was"
#define SQL_ "6"
#define YOUR_PROJECT_IS_DAMAGED "Your project file is damaged.  You may need to start the project over again."





#define SEARCH_NT "searchnt"
#define SEARCH_OT "searchot"
#define SEARCH_BOOK "searchbook"


QString BibleKey(int BibleNumber);
QString CommentaryKey(int CommentaryNumber);
QString CommentaryTypeKey(int CommentaryNumber);
QString DocTypeKey(int Number);
QString NoteKey(int Number);
QString NoteTypeKey(int Number);

QString GetPrefVariable(QString VarName);
BOOL    SavePrefVariable(QString VarName,QString Value);

void saveSelectedBooks(void);
int OpenBookAndPutInSelectedArray(QString Type, QString Name, int index);
int OpenCommentaryByNameAndPutInSelectedArray(QString Name);// preferenceslibrary.m


BOOL      LoadDefaultPrefs(void);
BOOL      _LoadPreferences(void);


#endif // PREFERENCESLIBRARY_H
