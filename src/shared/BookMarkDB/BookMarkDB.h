#ifndef BOOKMARKDB_H
#define BOOKMARKDB_H

//
//  BookMarkDB.h
//  Crossword-Try3
//
//  Created by WADE MAXFIELD on 10/28/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//

#include "../shared/Database/database.h"
#include "chapterversestructclass.h"

#define  BOOKMARKS_TABLE "BookMarks"
#define  BOOK_BOOK_MARKS_DB_NAME "BookBookMarks"
#define  BIBLE_BOOK_MARKS_DB_NAME "BibleBookMarks"

#define  CATEGORY_INFO "Info"
#define  CATEGORY_TOPIC_NAME "TopicName"

#define  CATEGORY_NUM_BOOKMARKS "NumBookMarks"
#define  NUMBER_OF_TOPICS "NumTopics"


class BookMarkDB : public SQliteDatabase
{
public:
    BookMarkDB();


    //----
    void     addBookmarkUnderTopic(QString Topic, /*usingBook*/ QString book, /*andChapter*/ QString chapter, /*andVerse*/ QString verse, /*isTexBook*/ BOOL isTextBook);

    BOOL          addTopic( QString TopicName);

    //----
    BOOL          CreateTables();// create the database tables

    //----
    BOOL          deleteBookMark(QString BookMark, /*underTopic*/ QString Topic);
    BOOL         DeleteVariable( QString TopicOrCategory, /*VariableName:  */  QString VariableName);
    BOOL         deleteBookMarkTopicOrCategory( QString  TopicOrCategory );

    //----
    QString    getVariableByName(QString VariableName, /*andCategoryOrTopicName*/ QString TopicOrCategory );
    int          getNumBookMarksInTopicOrCategory(QString TopicOrCategory);
    int          getNumTopics();
    QStringList      getTopicArray();
    QStringList      getBookmarkNamesArrayForTopic(QString Topic);
    QList<ChapterVerseStructType> getBookmarkNamesCVSArrayForTopic(QString Topic) ;

    void          initWithDBName(  QString dbName, /*andType*/ int Type);

    //----
    BOOL         saveVariable( QString TopicOrCategory, /*VariableName*/ QString VariableName, /*Value*/ QString myVar );




    BOOL        isOpen();         // when YES, is open
    QString     TopicOrCategoryKey(int number) ;
    QString     BookMarkKey(int  TopicOrCategoryNumber ,int BookMarkNumber) ;
};

extern BookMarkDB *gBookMarkDB;

#endif // BOOKMARKDB_H
