#ifndef BIBLEDATABASECLASS_H
#define BIBLEDATABASECLASS_H
//
//  BibleDatabase.h
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/21/10.
//  Copyright 2010 CrossWordProject. All rights reserved.
// converted

#include "../shared/Database/database.h"
#include "structs.h"
//#include "HTMLModule.h"

#define CHAPTER_COLUMN "Chapter"
#define VERSE_COLUMN    "Verse"
#define WORD_SEQ_COLUMN  "WordSeqNumber"

#define WORD_COLUMN "Word"
#define WORD_FORMAT "WordFormat"
#define WORD_POST "post"
#define WORD_PRE "pre"

#define FoundNormal 0
#define FoundFONT   1
#define FoundTABLE  2
#define FoundTAG    3
#define FoundXREF   4
#define FoundBR     5
#define FoundIMG    6
#define FoundOL     7
#define FoundTR     8
#define FoundSPAN   9

//#define WORD_FIELD 2
// these are the indexes from record set point of view
// i.e. rs->IdxField(WORD_FIELD)
// when using the standard QueryBegin initialized in OpenWithFileName() or OpenWithType()
#define PRE_FIELD 1
#define WORD_FIELD 2
#define POST_FIELD 3
#define LANG_FIELD 4
#define STRONG_FIELD 5
#define DICT_FIELD   5
#define WORD_SEQ_FIELD 6
#define WORD_FORMAT_FIELD 7


class BibleDatabaseClass : public SQliteDatabase
{
public:
    BibleDatabaseClass();
    BibleDatabaseClass(QString FileName);
    BibleDatabaseClass(QString FileName, int type);

    QString QueryBegin;     // the beginning of the sql string
    QString QueryEnd;       // the ending

    int     BibleIndexNumber;
    QString _BibleName;      // the base name of the bible (without suffix)
    int     FootNote;

    int     objectType;     // BOOK_TYPE_BIBLE (BookHandlerClass.h)

    QString pathComponent;
    QString name;


    void        updateWordSeqNumber(SEARCH_INFO_TYPE *sit, int newSeqNumber);

       //QString      BibleName;
    void         setBibleName(QString bn);

    long         Close();
    BOOL         createTables();
    void         createIndex(QString TableName);


    int          openWithFileName(QString FileName);       // open with this name and deduce the type
    BOOL         openWithType(int lType);                              // open with preset name but this type

    QString     getSTitle();
    QString     GetVerse(SHOW_THIS_INFO_TYPE *STI, /*SearchParmsStructPointer*/SEARCH_INFO_TYPE *sit);
    QString     GetPlainVerse(SHOW_THIS_INFO_TYPE *STI, /*SearchParmsStructPointer*/SEARCH_INFO_TYPE *sit, /*Type*/int BibleType);
    int         GetFootnoteIndex();
    QString     getFootNoteTextFromBook(int Book, /*Chapter*/int  Chapter, /*Verse*/int Verse, /*SeqNumber*/int SeqNumber);
    QString     getCopyrightText();

    void        InsertWordIntoDatabase(BibleBookInfoStructType *bbis, /*Chapter*/int  Chapter, /*Verse*/int Verse, /* WordSequence*/int wordSequence,
                                /*Word*/QString word, /*OrigWordType*/QString origWordLanguage, /*DictionaryEntry*/QString dictionaryEntry,
                                /*PreWord*/ QString preWord, /*PostWord*/QString postWord);
//    void        insertSequenceNumberGapAtIndex(SEARCH_INFO_TYPE *sit, int index);
    void        ResetFootNote();

    void        SaveNoteToDatabase(QString noteText, /*BookNumber*/ int bookNumber,/*Chapter*/int  Chapter, /*Verse*/int Verse);

    QString     GetTextFromDatabaseByBook(/*BookNumber*/ int bookNumber,/*Chapter*/int  Chapter, /*Verse*/int Verse);

    void         DeleteFromDatabaseByBook(/*BookNumber*/ int bookNumber,/*Chapter*/int  Chapter, /*Verse*/int Verse);

    QString     ParseForDictionary(QString s, /*withPreviouslyFoundType*/int *foundType);
    RecordSet   *GetVerseRecords(/*forParms*/SEARCH_INFO_TYPE*SIT);

    BOOL        updateWordRecordInfo(SEARCH_INFO_TYPE*SIT, /*usingWord*/QString Word, /*andPre*/QString Pre, /*andPost*/QString Post, /*andLang*/QString Lang,
                               /*andNumber*/ QString Number, QString format);

    void        setName(QString lname);
    QString     getName(void);

    int         getObjectType();
    void        setTitle( QString ttl);
    QString     getTitle();

    QString     getBibleName();
    //BOOL        openWithType(int lType);
    BOOL        Open();
    QString     HandleABPWord(RecordSet *rs, SHOW_THIS_INFO_TYPE *sti, SEARCH_INFO_TYPE *sit);
    QString     HandleWord(RecordSet *rs, SHOW_THIS_INFO_TYPE *sti, SEARCH_INFO_TYPE *sit);

private:
    QString Title;             // get from the database
    QString shortTitle;

};


#endif // BIBLEDATABASECLASS_H
