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
*/#include "translationtextfiledialog.h"
#include "ui_translationtextfiledialog.h"
#include <QFileDialog>
#include "bibleinfo.h"
#include "../editDB/editdb.h"
#include <QTextStream>
#include <QStringBuilder>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TranslationTextFileDialog::TranslationTextFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TranslationTextFileDialog)
{
    ui->setupUi(this);
    cWebDB=nullptr;
    showMe=nullptr;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TranslationTextFileDialog::~TranslationTextFileDialog()
{
    delete ui;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TranslationTextFileDialog::showEvent(QShowEvent *e){
    Q_UNUSED(e);
    pickerViewArray = getBookNamesList();
    int index;

    for (index = 0 ; index < pickerViewArray.count(); index++) {
        QListWidgetItem* pItem =new QListWidgetItem();
        //pItem->setSizeHint(QSize(0,40));// ??
        pItem->setTextAlignment(Qt::AlignLeft);
        pItem->setText(pickerViewArray[index]);
        ui->listWidget->addItem(pItem);
    }
    ui->cbTagalogIlongo->setChecked(1);// check it
    ui->cbTaglish->setChecked(0);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TranslationTextFileDialog::on_listWidget_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QListWidgetItem *itemPtr = ui->listWidget->currentItem();

    ui->leBookName->setText(itemPtr->text());

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TranslationTextFileDialog::on_pbSelectFile_clicked()
{
    // select the text file output name
    if (!textFileName.length()){
        textFileName=QFileDialog::getExistingDirectory(
                    this,
                    tr("Output Directory"),
                    "/drop-folder/Dropbox/NewBible/",
                    QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog
                    );
        ui->leDirName->setText(textFileName);
        if (!textFileName.length())
            return; // do nothing
    }

    ui->pbOpenAndProcess->setEnabled(true);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
dbWordType * TranslationTextFileDialog::getEnglishVerseString(int BookNumber,int Chapter,int Verse, int *wordCount){

    int result = getEnglishCompareLineFromDB(cWebDB,EnglishCompareLineWFW,wordCount,  BookNumber,Chapter, Verse);// the the WEBDB words into language array for comparison to target language
    if (result < 0){
        *wordCount =0;
    }

    return EnglishCompareLineWFW;

}
//-----------------------------------------------------------------------------
// put out the form:
/*
 *
    Mark

    Chapter 1

    English

    1 The beginning of the Good News of Jesus Christ, the Son of God.

    Ilonggo

    1

    Tagalog

    1

    English wfw

    1 The | beginning | of | the | Good | News | of | Jesus | Christ, | the | Son | of | God.

    Ilonggo wfw

    1  |  |  | |  |  |  |  |  |  |  |  |

    Tagalog wfw

    1  |  |  | |  |  |  |  |  |  |  |  |

    English

    2 As it is written in the prophets, "Behold, I send my messenger before your face, who will prepare your way before you.

    Ilonggo

    2

    Tagalog

    2

    English wfw

    2 As | it | is | written | in | the | prophets, | "Behold, | I | send | my | messenger | before | your | face, | who | will | prepare | your | way | before | you.

    Ilonggo wfw

    2  | | |  |  |  |   |  |  |  |  |   |  ||  ||  |  |  || .

    Tagalog wfw

    2   |||  |  |  |  |  |  |  |  |  |    |  |  |  ||  ||  ||
    ...

 */
//-----------------------------------------------------------------------------
void outputVersePattern(QTextStream &outStream,
                         int verse,
                        dbWordType *verseArray,
                        int wordCount,
                        int doTaglish) // if true, do taglish instead of illongo
{
    outStream << "English";
    outStream << "\n\n";

    QStringList a;
    int i;
    QString s= QString::number(verse);

    for ( i=0 ; i < wordCount; i++){
        a.append(verseArray[i].Word);
    }

    outStream << s % " ";
    outStream << a.join(" ") % "\n\n";

    if (doTaglish) {
        // no taglish wfw
        outStream << "Taglish";
        outStream << "\n\n";
        outStream << s % " \n\n";
        //return;

    } else {
    outStream << "Ilonggo";
    outStream << "\n\n";
    outStream << s % " \n\n";

    outStream << "Tagalog\n\n";
    outStream << s % " \n\n";
    }

    outStream << "English wfw";
    outStream << "\n\n";
    outStream << s % " ";

    for (i=0 ; i< wordCount-1; i++){
        outStream << verseArray[i].Word % " | ";
    }

    outStream <<  " " % verseArray[i].Word;

    outStream << "\n\n";


    if (doTaglish) {
        outStream << "Taglish wfw\n\n";
        outStream << s % " ";

        for (i=0 ; i< wordCount-1; i++){
            outStream << QString(verseArray[i].Word.length(),' ') % " | ";
        }
        outStream << QString(verseArray[i].Word.length(),' ');

        outStream << "\n\n";

    } else {
        outStream << "Ilonggo wfw\n\n";
        outStream << s % " ";

        for (i=0 ; i< wordCount-1; i++){
            outStream << QString(verseArray[i].Word.length(),' ') % " | ";
        }
        outStream <<  QString(verseArray[i].Word.length(),' ');

        outStream << "\n\n";

        outStream << "Tagalog wfw\n\n";
        outStream << s % " ";

        for (i=0 ; i< wordCount-1; i++){
            outStream << QString(verseArray[i].Word.length(),' ') % " | ";
        }
        outStream << QString(verseArray[i].Word.length(),' ');

        outStream << "\n\n";
    }
}

//-----------------------------------------------------------------------------
// create the text file template
//-----------------------------------------------------------------------------
void TranslationTextFileDialog::on_pbOpenAndProcess_clicked()
{

    if (ui->leBookName->text().length()==0){
        MsgBox("Please select a book!");
        return;
    }
    // open the bible file selected and create each of
    // chapter files
    BibleBookInfoStructType bbi = getBibleBookInfoByName( ui->leBookName->text());

    int BookNumber=bbi.BookNumber;
    if (BookNumber<=0)
        return;
    BookChapterVerseStructType bcv;
     GetBookChapterVerse( BookNumber,  bcv);

     if (!cWebDB){
         cWebDB=new BibleDatabaseClass(ui->le_cWEBDB->text());
         if (!cWebDB){
             MsgBox("Error Opening cWEB Database!");
             return;
         }
     }

    int numChapters = bbi.NumChapters;
    int chapter,verse,wordCount;
    if (numChapters) {

        for (chapter=1; chapter <= bbi.NumChapters; chapter++ ){
            QFile outFile(ui->leDirName->text() % "/" % ui->leBookName->text() % " " %QString::number(chapter) % ".txt");
            outFile.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream outStream(&outFile);

            outStream << ui->leBookName->text();
            outStream << "\n\n";

            QString s= "Chapter " + QString::number(chapter) ;
            outStream << "\n\n" % s % "\n\n";

            int numVerses = bcv.NumberOfVerses[chapter];
            for (verse=1; verse <=numVerses;verse++){
                dbWordType *verseString=  getEnglishVerseString(BookNumber,chapter,verse,&wordCount);
                outputVersePattern(outStream,verse,verseString,wordCount,ui->cbTaglish->isChecked());
            }

            outStream << "end\n\n";
            outFile.close();
        }

    }
    MsgBox("finished");
}

//-----------------------------------------------------------------------------
// locate the WEB bible database and open it
//-----------------------------------------------------------------------------
void TranslationTextFileDialog::on_pbWebDB_clicked()
{
    QString fname="";

        fname=QFileDialog::getOpenFileName(
                    this,
                    tr("Open cWEB File"),
                    "/drop-folder/Dropbox/NewBible/foo.bbl",
                    "Bible file (*.bbl)"
                    );
        ui->le_cWEBDB->setText(fname);

    if(fname.length()) {
        // select cWEB database and open
        cWebDB=new BibleDatabaseClass(ui->le_cWEBDB->text());
        if (!cWebDB){
            MsgBox("Error Opening cWEB Database!");
        }
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TranslationTextFileDialog::on_pushButton_clicked()
{
    close();
    if (showMe){
        showMe->show();
        showMe->raise();
        showMe->activateWindow();
        showMe=0;
    }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TranslationTextFileDialog::on_listWidget_activated(const QModelIndex &index)
{
 Q_UNUSED(index);
}

void TranslationTextFileDialog::on_cbTaglish_clicked()
{
    if (ui->cbTaglish->isChecked()){
        ui->cbTagalogIlongo->setChecked(0);
    } else
        ui->cbTagalogIlongo->setChecked(1);
}

void TranslationTextFileDialog::on_cbTagalogIlongo_clicked()
{
    if (ui->cbTagalogIlongo->isChecked()){
        ui->cbTaglish->setChecked(0);
    }else
        ui->cbTaglish->setChecked(1);
}
