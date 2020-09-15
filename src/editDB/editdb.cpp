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
*/#include "editdb.h"
#include <QFile>
#include <QTextStream>
#include "misclibrary.h"
#include <QMessageBox>
#include "../BibleDatabaseClass/bibledatabaseclass.h"
#include <QCoreApplication>
#include "../shared/bibleinfo.h"

static QFile   f;
int CurrentChapter;
int CurrentVerse;
int LastVerse;
int LastChapter;
QString CurrentLine;
int CurrentBookNumber;
BookChapterVerseStructType bcv;

//#define ENABLE_DEBUG 1
#if ENABLE_DEBUG
dbWordType EnglishCompareLineWFW[1024]; // 1,000 words in a single verse
 int EnglishCompareLineWordWFWCount;
 dbWordType TargetCompareLineWFW[1024]; // 1,000 words in a single verse
 int TargetCompareLineWordWFWCount;
 dbWordType TargetInsertLine[1024]; // 1,000 words in a single verse
 int TargetInsertLineWordCount;


QString lookForText(QTextStream &in,QString text);
int lookForBook(QTextStream &in) ;
int findAndReturnBookNumber(QTextStream &in);
int findAndReturnChapterNumber(QTextStream &in);
int findAndReturnVerseNumber(QTextStream &in);// text is in CurrentLine
int getLanguage(QTextStream &in,QString languageName);

int getEnglishCompareLineFromDB(BibleDatabaseClass *webDB,dbWordType *englishCompareLine,int *wordCount, int BookNumber,int Chapter, int Verse);// the the WEBDB words into language array for comparison to target language
int getLanguageInsertLine(QTextStream &in,dbWordType *targetInsertLine,int *targetCount,QString languageName); // put words into the language array for database insertion later
int getTextIntoTargetCompareLineWFW(QTextStream &in,dbWordType *targetCompareLineWFW,int *wordCount,QString languageName);
int processTargetCompareLineFromEnglishWFWandTargetWFW(dbWordType *targetInsertLine,dbWordType *englishCompareLineWFW,
                                                        int *englishCompareLineWFWCount,dbWordType *targetCompareLineWFW);
int putTargetInsertLineIntoDatabase(BibleDatabaseClass *targetDB,dbWordType *targetInsertLine,int *wordCount,int BookNumber,int Chapter,int Verse);

/*  Format of text file
 * John
 * Chapter 1
 *
 * English
 *
 * 1 In the beginning was the Word, and the Word was with God, and the Word was God.
 *
 * Ilonggo
 *
 * 1 Sa ginsuguran amo ang Pulong, kag ang Pulong kaupod sang Diyos, kag ang Pulong amo ang Diyos.
 *
 * Tagalog
 *
 * 1 Sa pasimula ay ang Salita, at ang Salita ay kasama ng Diyos, at ang Salita ay Diyos.
 *
 * English wfw
 *
 * 1 In | the | beginning | was | the | Word, | and | the | Word | was | with | God, | and | the | Word | was | God.
 *
 * Ilonggo wfw
 *
 * 1 Sa || ginsuguran || amo ang | Pulong, | kag | ang | Pulong || kaupod | sang Diyos, | kag | ang | Pulong || amo ang Diyos.
 *
 * Tagalog wfw
 *
 * 1 Sa || pasimula | ay | ang | Salita, | at | ang | Salita | ay | kasama | ng Diyos, | at | ang | Salita | ay | Diyos.
*/


//------------------------------------------------
// return -1 if error, 0 if success
//------------------------------------------------
int parse(void (*updateUI)(),QString fileName,BibleDatabaseClass *WebDB,BibleDatabaseClass *targetDB,QString LanguageName,int *StopIt) {


    // open the file
    f.setFileName(fileName);
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"error opening file",f.errorString());
        return -1;
    }

    updateUI();
    qApp->processEvents(QEventLoop::AllEvents,50);

    // get the text stream
   QTextStream in(&f);

   // get the book number we are working with
   int BookNumber = findAndReturnBookNumber(in);

   if (BookNumber<0){
       exitme:
       f.close();
       return -1;
   }

   CurrentBookNumber=BookNumber;
   int maxVerse;
   int index,index2;
   dbWordType *wPtr;

   do {


       LastChapter=CurrentChapter;

       CurrentChapter = findAndReturnChapterNumber(in);
       if (CurrentChapter == -3 ){
           // end of file, bail normally
           break;
       }
       if (CurrentChapter<1) {
           goto exitme;// some sort of problem
       }

       //if (CurrentChapter<1)
       //    goto exitme;

        GetBookChapterVerse( BookNumber, bcv);// pick up the information for max verses

       maxVerse = bcv.NumberOfVerses[CurrentChapter];


       do {
 //       for(index=0;index < 10;index++)
           if (*StopIt)
               break;

       if(getLanguage(in,"English")<0)
           goto exitme;

       CurrentVerse =findAndReturnVerseNumber(in); // puts verse text in Current Line.  returns verse number
       if (CurrentVerse<1)
           goto exitme;

       updateUI();
       qApp->processEvents(QEventLoop::AllEvents,50);

       // null out the line
       wPtr=EnglishCompareLineWFW;
       for(index=0; index < EnglishCompareLineWordWFWCount;index++){
           wPtr->Word="";
           wPtr->pre="";
           wPtr->post="";
           wPtr->assigned=0;
           wPtr->DictEntryNumber="";
           wPtr->OrigWordLanguage="";
           wPtr++;
       } EnglishCompareLineWordWFWCount=0;
       // English text line not needed. use from the Database
       // get the English WFW into the array
       int ECCount;
        if((ECCount=getEnglishCompareLineFromDB(WebDB,EnglishCompareLineWFW,&EnglishCompareLineWordWFWCount,BookNumber,CurrentChapter,CurrentVerse))<0)// the the WEBDB words into language array for comparison to target language
            goto exitme;
        //if (ECCount==1)// for ASV missing verses
        //    continue;
        // get the tagalog or ilonggo text line
        if (getLanguageInsertLine(in,TargetInsertLine,&TargetInsertLineWordCount,LanguageName)) // put words into the language array for database insertion later
            goto exitme;//fail

        // null out the line
        wPtr=TargetCompareLineWFW;
        for(index=0; index < TargetCompareLineWordWFWCount;index++){
            wPtr->Word=""; wPtr->DictEntryNumber="";
            wPtr->post=""; wPtr->OrigWordLanguage="";
            wPtr->pre="";
            wPtr->WordFormat="0";
            wPtr++;
        } TargetCompareLineWordWFWCount=0;

        //----------------------------------------------------------------------
        // get the target WFW into the array
        if (getTextIntoTargetCompareLineWFW(in,TargetCompareLineWFW,&TargetCompareLineWordWFWCount, LanguageName))
            goto exitme;

        if (ECCount==1)
            goto putTargetIntoDatabase;
        //----------------------------------------------------------------------
        if (TargetCompareLineWordWFWCount< EnglishCompareLineWordWFWCount){
            // there may be some blanks in the english.  If so, spread out the Target
            for (index=0; index <EnglishCompareLineWordWFWCount; index++ ){
                if(EnglishCompareLineWFW[index].Word.length()==0){
                    // shift everything right by one in the target
                    for(index2=TargetCompareLineWordWFWCount;index2>=index;index2--){
                        TargetCompareLineWFW[index2+1]=TargetCompareLineWFW[index2];
                    }// this basically does nothing for now, but later may fix.
                    TargetCompareLineWordWFWCount++;
                    // now we have the target slot at index ready.  move over the strong's and assign the english
                    dbWordType *dpt = &TargetCompareLineWFW[index];
                    dbWordType *dpe = &EnglishCompareLineWFW[index];
                    dpt->DictEntryNumber=dpe->DictEntryNumber;
                    dpt->OrigWordLanguage=dpe->OrigWordLanguage;
                    dpt->WordFormat=dpe->WordFormat;
                    dpt->Word=""; // this is a blank word, so don't let anything creep in
                    dpe->assigned=1; // this has been assigned
                }
                // we are finished, the number of words match up
                if (TargetCompareLineWordWFWCount==EnglishCompareLineWordWFWCount)
                    break;
            }
        }

        if(TargetCompareLineWordWFWCount != EnglishCompareLineWordWFWCount) {
        QMessageBox::information(0,"Word Count Mismatch",LanguageName+
                                 " wfw count != English wfw count\nEnglish: "+
                                 QString::number(EnglishCompareLineWordWFWCount)+", "+LanguageName+
                                ":"+QString::number(TargetCompareLineWordWFWCount)+
                                 " \nChapter "+QString::number(CurrentChapter)+", Verse "+QString::number(CurrentVerse));
            goto exitme;
        }

        //----------------------------------------------------------------------
        // now tag the Target
        if (processTargetCompareLineFromEnglishWFWandTargetWFW(TargetInsertLine,EnglishCompareLineWFW,&EnglishCompareLineWordWFWCount,TargetCompareLineWFW))
            goto exitme;

putTargetIntoDatabase:
        //----------------------------------------------------------------------
        // now put the target line into the database
        if (putTargetInsertLineIntoDatabase(targetDB,TargetInsertLine,&TargetInsertLineWordCount,BookNumber,CurrentChapter,CurrentVerse))
            goto exitme;

        } while(CurrentVerse < maxVerse && !in.atEnd());
       if(*StopIt)
           break;

   } while(CurrentChapter < bcv.NumberOfChapters && !in.atEnd());


   //*StopIt=0;// reset just in case
   f.close();
    return 0;
}
//----------------------------------------------------------------------
// return the number of entries into englishCompareLine
//----------------------------------------------------------------------
int getEnglishCompareLineFromDB(BibleDatabaseClass *webDB,dbWordType *englishCompareLine,int *wordCount, int BookNumber,int Chapter, int Verse){// the the WEBDB words into language array for comparison to target language

    SEARCH_INFO_TYPE SIT;
    RecordSet *rs;
    BibleBookInfoStructType bis= getBibleBookInfo( BookNumber);

    SIT.BookNumber=BookNumber;
    SIT.ChapterNumber=Chapter;
    SIT.VerseNumber=Verse;
    SIT.BookTableName=bis.TableName;// get table name
    rs = webDB->GetVerseRecords(/*forParms*/&SIT);// STI unused
    if(!rs)
        return -1;//error
    int count=0;

    while (!rs->atEOF()){
        englishCompareLine->Chapter=QString::number(Chapter);
        englishCompareLine->Verse=QString::number(Verse);
        englishCompareLine->pre=rs->IdxField(PRE_FIELD);
        englishCompareLine->Word=rs->IdxField(WORD_FIELD);
        englishCompareLine->post=rs->IdxField(POST_FIELD);
        englishCompareLine->OrigWordLanguage=rs->IdxField(LANG_FIELD);
        englishCompareLine->DictEntryNumber=rs->IdxField(DICT_FIELD);
        englishCompareLine->WordFormat=rs->IdxField(WORD_FORMAT_FIELD);
        englishCompareLine->WordSeqNumber=rs->IdxField(WORD_SEQ_FIELD);
        englishCompareLine->assigned=0;// initialize to unworked state
        englishCompareLine++;
        count++;
        rs->MoveNext();
    }

    if(rs) {
        rs->done();
        delete rs;
    }
    *wordCount=count;
    return count;
}
//------------------------------------------------
//------------------------------------------------
void handleSpecialChars(dbWordType *targetLine){
    QChar c;
    QString s;
    s = targetLine->Word;
    do {
        if (s.length()==0)
            break;
        c = s[s.length()-1];
        if (Instr(":;'\",)?!.-”“‘’",QString(c))){
            s.chop(1);// remove that character
            targetLine->post = QString(c)+targetLine->post;
        } else {
            break; // finished
        }

    }while(1);

    do {
        if (s.length()==0)
            break;
        c = s[0];
        if(Instr("-('\"”“‘’;:",QString(c))){
            s.remove(0,1);
            targetLine->pre =QString(c)+targetLine->pre;
        } else {
            break;
        }
    }while(1);
    targetLine->Word=s;
}
//------------------------------------------------
// find the line matching the language
//  we should be at the language Verse line now
//------------------------------------------------
int getLanguageInsertLine(QTextStream &in,dbWordType *targetLine,int *targetCount,QString languageName){ // put words into the language array for database insertion later

    int index;
    int limit = *targetCount*2;
    if (limit > 1024)
        limit =1024;
    dbWordType *dp=targetLine;
    // target count has the length of the characters in the last line read
    // clear out the old junk, was confusing previous
    for(index=0; index < limit;index++){
        dp->Word="";
        dp->post="";// clear out old junk
        dp->pre="";// clear out old junk
        dp->assigned=0;
        dp->DictEntryNumber="";
        dp->OrigWordLanguage="";
        dp->WordFormat="";
        dp++;
    }

    if (getLanguage(in,languageName)<0){
       return -1; // failed
    }

    // we are now at the language line (i.e. tagalog)
    if(!findAndReturnVerseNumber(in)){
        return -1; //failed
    }

    // we have the line in CurrentLine
    QStringList l = CurrentLine.split(" ");// turn into an array
    // remove the verse number, the first item
    l.removeFirst(); // remove the verse number

    int count=0;

    foreach(const QString &s, l){// making a const makes it almost 10 times faster
        targetLine->Word=s;
        targetLine->post="";// clear out old junk
        targetLine->pre="";// clear out old junk
        handleSpecialChars(targetLine);
        targetLine->assigned=0;
        targetLine->DictEntryNumber="";
        targetLine->OrigWordLanguage="";
        targetLine->WordFormat="";
        count++;
        targetLine++;
    }

    *targetCount=count;
    return 0;
}



//------------------------------------------------
//------------------------------------------------
int getTextIntoTargetCompareLineWFW(QTextStream &in,dbWordType *targetLine,int *targetCount,QString languageName){

    if (getLanguage(in,languageName)<0){
        return -1; // failed
    }
    if (!Instr(CurrentLine,"wfw")){
        MsgBox("wfw tag not found, verse "+QString::number(CurrentVerse));
        return -1; //failed
    }

    // we are now at the language line (i.e. tagalog)
    if(!findAndReturnVerseNumber(in)){
        MsgBox("Verse Number tag not found, verse "+QString::number(CurrentVerse));
        return -1; //failed
    }

    // clear out the old target info
    int index;
    int limit = *targetCount*2;
    dbWordType *dp = targetLine;
    for (index=0; index < limit;index++){
        dp->Word="";
        dp->DictEntryNumber="";
        dp->OrigWordLanguage="";
        dp->post="";
        dp->pre="";
        dp->WordFormat="";
        dp->assigned=0;
        dp++;
    }

    int count=0;

    // we have the line in CurrentLine
    QStringList l = CurrentLine.split(" ");// turn into an array
    QStringList l2;
    // remove the verse number, the first item
    l.removeFirst(); // remove the verse number
    // now go through the line and remove second word of two word pair
    CurrentLine=l.join(" ");

    l=CurrentLine.split("|");
    count=0;
    foreach(const QString &s, l){// making a const makes it almost 10 times faster
        targetLine->Word=s.trimmed();
        if(Instr(targetLine->Word," ")){
            l2 = targetLine->Word.split(" ");
            if(l2.size()>1)
                targetLine->Word=l2.at(1);// get the leftmost word of the set
            else
                targetLine->Word=l2.at(0);
        }
        handleSpecialChars(targetLine);
        targetLine->assigned=0;
        count++;
        targetLine++;
    }

    *targetCount=count;
    return 0;

}

//--------------------------------------------------
// set up the ilonggo/tagalog line to have strong's
//--------------------------------------------------
int processTargetCompareLineFromEnglishWFWandTargetWFW(dbWordType *targetInsertLine,dbWordType *englishCompareLineWFW,int *englishCompareLineCount,
                                                        dbWordType *targetCompareLineWFW){

    // multiple passes
    // when a match occurs, set assigned = 1. at that point, the word falls out of use.
    // when all words are assigned, then is done
    // 0) attribute a match to all words that don't have strongs
    // 1) find the exact word to word matches
    // for x = 1 to 5
    // 2) look for matches x word(s) to the left
    // 3) look for matches x word(s) to the right
    int index,index2;
    dbWordType *ePtr;
    //0) make all non-strongs assigned
    ePtr = englishCompareLineWFW;
    for(index=0;index < *englishCompareLineCount;index++){
        if (ePtr->DictEntryNumber.toInt()==0)
            ePtr->assigned=TRUE;
        ePtr++;
    }

    dbWordType *dbTargetPtr=targetInsertLine;// tagalog or ilonggo
    dbWordType *dbTargetWFWPtr=targetCompareLineWFW;
    dbWordType *targetPtr;//target pointer
    //1) find exact word to word matches. skip already assigned
    ePtr=englishCompareLineWFW;
    for(index2=0;index2<*englishCompareLineCount;index2++) {
        if(!ePtr->assigned) {
            // if tagalog/ilonggo word matches tagalog/ilonggo wfw then move the english word over
            if(StringsAreEqual(dbTargetPtr->Word,dbTargetWFWPtr->Word)) {
                dbTargetPtr->DictEntryNumber=ePtr->DictEntryNumber;
                dbTargetPtr->OrigWordLanguage=ePtr->OrigWordLanguage;
                dbTargetPtr->WordFormat=ePtr->WordFormat;// pick up photo info also
                ePtr->assigned=TRUE;
                dbTargetPtr->assigned=TRUE;
            }
        }
        dbTargetPtr++;
        ePtr++;
        dbTargetWFWPtr++;
    }

     QString otherWord;
     int index3;
    // now look for offset matches
    // for x = 1 to 6

        dbTargetPtr=targetInsertLine;// tagalog or ilonggo
        dbTargetWFWPtr=targetCompareLineWFW;
        ePtr=englishCompareLineWFW;
        // if this strong's has not been assigned, then
        // if the next (x) word over in the InsertLine matches the
        // WFW word, then put the strong's number there
        for(index2=0;index2<*englishCompareLineCount;index2++) {
            if (!(ePtr->assigned|| ePtr->DictEntryNumber==0)) {
                // see if a word can be assigned
                for (index = 1; index < 12;index++){
                    targetPtr=&dbTargetPtr[index+index2];

                    otherWord=targetPtr->Word;
                    // if tagalog/ilonggo word matches tagalog/ilonggo wfw [x] offset then move the english word over
                    if(StringsAreEqual(otherWord,dbTargetWFWPtr->Word) && !targetPtr->assigned) {
                        targetPtr->DictEntryNumber=ePtr->DictEntryNumber;
                        targetPtr->OrigWordLanguage=ePtr->OrigWordLanguage;
                        targetPtr->WordFormat=ePtr->WordFormat;// pick up picture info
                        ePtr->assigned=TRUE;
                        targetPtr->assigned=TRUE;
                        break; // done with this inner loop
                    }


                    // now look for -1 to -5 offsets
                    // don't look farther to the left, the answer is almost always
                    // to the right
                    if (index <= 5) {
                        index3 = index2-index;
                        if (index3<0)
                            index3=0;
                        targetPtr=&dbTargetPtr[index3];
                        otherWord = targetPtr->Word;
                        // if the word to the left of the insert line matches the WFW, then
                        // use that strong's number
                        // if tagalog/ilonggo word matches tagalog/ilonggo wfw [x] offset then move the english word over
                        // this accidentally assigned same word (same meaning) to word already assigned.
                        // fix by setting targetPtr->assigned to be true
                        if(StringsAreEqual(otherWord,dbTargetWFWPtr->Word) && !targetPtr->assigned) {
                            targetPtr->DictEntryNumber=ePtr->DictEntryNumber;
                            targetPtr->OrigWordLanguage=ePtr->OrigWordLanguage;
                            targetPtr->WordFormat=ePtr->WordFormat;
                            ePtr->assigned=TRUE;
                            targetPtr->assigned=TRUE;
                            break;// done with this inner loop
                        }
                    }
                }
            }

            // move to next word
            ePtr++;
            dbTargetWFWPtr++;

        }

    return 0; // all ok
}

//------------------------------------------------
// save the tagalog/ilonggo database line
//------------------------------------------------
int putTargetInsertLineIntoDatabase(BibleDatabaseClass *targetDB,dbWordType *targetInsertLine,int *wordCount,int BookNumber,int Chapter,int Verse) {

//    void BibleDatabaseClass::InsertWordIntoDatabase(BibleBookInfoStructType *bbis , /*Chapter*/ int chapter , /*Verse*/ int verse ,
//    int wordSequence, QString word, QString origWordLanguage, int dictionaryEntry, QString preWord,  QString postWord)

    BibleBookInfoStructType bbis=getBibleBookInfo(BookNumber);
    int index;
    int wc=*wordCount;
    int ws=0;
    for (index=0; index < wc;index++){
        targetDB->InsertWordIntoDatabase(&bbis,Chapter,Verse,ws,targetInsertLine->Word,targetInsertLine->OrigWordLanguage,
                                         targetInsertLine->DictEntryNumber,targetInsertLine->pre,targetInsertLine->post);
        ws+=10; // allow room for inserts in the future
        if(targetDB->Error())
            return -1; // error
        targetInsertLine++;// next word to insert
    }

    return 0; // all ok
}

//------------------------------------------------
// return 0 if no verse number, verse number otherwise
//------------------------------------------------
int findAndReturnVerseNumber(QTextStream &in) { // text is in global CurrentLine

    if(in.atEnd())
        return 0;  //
    QString line ;
    //
    do {
         line = in.readLine().trimmed();
        if(line.length()){
            CurrentLine=line;
            QStringList l = line.split(" ");
            if (l.size()>0){
                int verse = atoi(l[0].toUtf8().constData());
                return verse;
            }
        }
    }while(!in.atEnd());

        return 0;

}
//------------------------------------------------
// return 0 if found, -1 if error
//------------------------------------------------
int getLanguage(QTextStream &in,QString languageName){
    QString t = lookForText(in,languageName);

    if (t.length()==0){
       return -1;
    }

    CurrentLine = t;
    return 0;
}


//------------------------------------------------
//------------------------------------------------
int findAndReturnBookNumber(QTextStream &in) {

    int    BookNumber = lookForBook(in);

    if (BookNumber < 0){
        return -1;
    }

    return BookNumber; // return the book number we are looking at.
}
//------------------------------------------------
// return chapter number or
// -1 if bad number
// -2 if not there
// -3 if end of file
//------------------------------------------------
int findAndReturnChapterNumber(QTextStream &in) {

    QString t = lookForText(in,"chapter");
    // get the chapter number if it is there
    if (t.length()==0){
       return -2;// not there
    }
    if (Instr(t,"end")){
        return -3; //end of file
    }

    QStringList l = t.split(" ");
    if (l.size()<2){
        return -2;// simply not there
    }

    int Chapter = atoi(l[1].toUtf8().constData());

    if (Chapter<1){
        return -1;// bad chapter
    }

    return Chapter;
}
//--------------------------------------------
//--------------------------------------------
QString getNextLine(QTextStream &in){
    QString t;
    do {
        t = in.readLine().trimmed();
        if(t.length()>0)
            return t;
    }while (!in.atEnd());

    return "";
}



 //--------------------------------------------
 // return Book Number.  return -1 if not found.
 //--------------------------------------------
int lookForBook(QTextStream &in) {
    if(in.atEnd())
        return -1;
    QString line ;
    BibleBookInfoStructType bis;

    do {
        line =in.readLine();
        line=line.trimmed();
        // skip blank lines
        if (line.length()==0)
            continue;

        bis= getBibleBookInfoByName(line);
        if (bis.BookNumber >=0)
            return bis.BookNumber;
    } while(!in.atEnd());

    return -1;
}



//--------------------------------------------
// return line when text found, "" if not found
//--------------------------------------------
QString lookForText(QTextStream &in,QString text) {

    if(in.atEnd())
        return "";  //
 QString line ;
    //
    do {
        line= in.readLine();
        if (Instr(line,text)){// case insensitive
            return line; // found text in the stream, ready for next part
        }
    }while(!in.atEnd());

        return "end";
}




#endif

