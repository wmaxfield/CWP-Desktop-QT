#ifndef EDITDB_H
#define EDITDB_H
#include <QString>
#include "BibleDatabaseClass/bibledatabaseclass.h"
/*
CREATE TABLE Genesis  (Chapter integer KEY,Verse integer KEY,WordSeqNumber integer,Word varchar,pre varchar,post varchar,
WordFormat integer, OrigWordLanguage char(1), DictEntryNumber integer);
*/
#ifndef TRUE
#define TRUE 1
#endif

// database word structure
typedef struct dbWord {
    QString Chapter;
    QString Verse;
    QString WordSeqNumber;
    QString Word;
    QString pre;
    QString post;
    QString WordFormat;
    QString OrigWordLanguage;
    QString DictEntryNumber;
    int     assigned;
} dbWordType;


#if ENABLE_EDIT
extern dbWordType EnglishCompareLineWFW[1024]; // 1,000 words in a single verse
extern dbWordType EnglishLine[1024]; // 1,000 words in a single verse
extern dbWordType TagalogLine[1024]; // 1,000 words in a single verse
extern dbWordType IlonggoLine[1024]; // 1,000 words in a single verse
extern int CurrentChapter;
extern int CurrentVerse;
extern int LastVerse;
extern int LastChapter;
extern QString CurrentLine;
extern int CurrentBookNumber;

#endif


int parse(void (*fn)(),QString fileName,BibleDatabaseClass *WebDB,BibleDatabaseClass *targetDB,QString LanguageName,int *StopIt) ;
int getEnglishCompareLineFromDB(BibleDatabaseClass *webDB,dbWordType *englishCompareLine,int *wordCount, int BookNumber,int Chapter, int Verse);// the the WEBDB words into language array for comparison to target language

#endif // EDITDB_H
