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
*/
#include "copydialog.h"
#include "ui_copydialog.h"
#include "bibleinfo.h"
#include "misclibrary.h"
#include "structs.h"
#include "HTMLModule.h"
#include <QClipboard>
#include "../SystemCommand/systemcommand.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::setupDialog() {

    this->setModal(true);

    dontUpdate=YES;
    PopulateStartChapterCB();
    PopulateEndChapterCB();
    PopulateStartVerseCB();
    PopulateEndVerseCB();
    PopulateTranslationsCB();
    PopulateBooksCB();



    ui->cbStartChapter->setCurrentIndex(_StartChapter-1);
    ui->cbStartChapter->show();
    ui->cbEndChapter->setCurrentIndex(_EndChapter-1);
    ui->cbEndChapter->show();
    ui->cbStartVerse->setCurrentIndex(_StartVerse-1);
    ui->cbStartVerse->show();
    ui->cbEndVerse->setCurrentIndex(_EndVerse-1);
    ui->cbEndVerse->show();
    ui->cbTranslation->setCurrentIndex(_BibleNumber);
    ui->cbTranslation->show();


    ui->cbBibleBook->setCurrentIndex(_Book-1);
    ui->cbBibleBook->show();
    dontUpdate=NO;
    // default to add verses and book/chapter
    ui->cbAddVerse->setChecked(true);
    ui->cbAddBookChapter->setChecked(true);
    _AddVerse=true;
    _AddBookChapter=true;

    BuildVerses();
    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::PopulateBooksCB() {
    ui->cbBibleBook->clear();
    int i;

    for ( i=1 ; i <= NUM_BOOKS_IN_BIBLE ; i++)
        ui->cbBibleBook->addItem(getBookName(i));

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::PopulateTranslationsCB() {
    ui->cbTranslation->clear();
    int numBooks=Books.count();

    for (int i=0 ; i<numBooks; i++)
        ui->cbTranslation->addItem(Books[i]->name);

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CopyDialog::CopyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyDialog)
{
    ui->setupUi(this);
    changedEndChapterManually=0;
    _Book=1;
    _StartChapter=1;
    _StartVerse=1;
    _EndChapter=1;
    _EndVerse=1;
    _BibleNumber=0;// start with first bible in system
    _AddBookChapter=NO;
    _AddVerse=NO;
    setupDialog();

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CopyDialog::CopyDialog( int Book, int Chapter, int Verse,int BibleNumber,QString BibleName,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyDialog)
{
    ui->setupUi(this);
    changedEndChapterManually=0;
    _Book=Book;
    _StartChapter=_EndChapter=Chapter;
    _StartVerse=_EndVerse=Verse;
    _BibleNumber=BibleNumber;
    _BibleName=BibleName;
   setupDialog();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::PopulateStartChapterCB(){
    ui->cbStartChapter->clear();
    GetBookChapterVerse(_Book,bcv);
    for (int i=1 ; i<=bcv.NumberOfChapters; i++)
        ui->cbStartChapter->addItem(QString::number(i));

}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::PopulateStartVerseCB(){
    ui->cbStartVerse->clear();
    GetBookChapterVerse(_Book,bcv);
    for (int i=1 ; i<=bcv.NumberOfVerses[_StartChapter]; i++)
        ui->cbStartVerse->addItem(QString::number(i));

}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::PopulateEndChapterCB(){
    ui->cbEndChapter->clear();
    GetBookChapterVerse(_Book,bcv);
    for (int i=1 ; i<=bcv.NumberOfChapters; i++)
        ui->cbEndChapter->addItem(QString::number(i));

}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::PopulateEndVerseCB(){
    ui->cbEndVerse->clear();
    GetBookChapterVerse(_Book,bcv);
    for (int i=1 ; i<=bcv.NumberOfVerses[_EndChapter]; i++)
        ui->cbEndVerse->addItem(QString::number(i));

}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::setParams(int Book, int Chapter, int Verse,int BibleNumber,QString BibleName) {

    _Book=Book;
    _StartChapter=_EndChapter=Chapter;
    _StartVerse=_EndVerse=Verse;
    _BibleName=BibleName;
    _BibleNumber=BibleNumber;

  setupDialog();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CopyDialog::~CopyDialog()
{
    delete ui;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::BuildVerses(){

    if (dontUpdate)
        return;

    // get the translation.
    BibleBookInfoStructType binfo = getBibleBookInfo(_Book);
    BookChapterVerseStruct bcv ;
    GetBookChapterVerse(_Book,bcv);
    //QString bookTableName = binfo.TableName;
    int Verse = _StartVerse;
    int Chapter = _StartChapter;
    QString v;

    if (_BibleNumber <0 ){
        return;
    }

    BibleDatabaseClass *bibleDatabaseClass =((BibleDatabaseClass*)(Books[_BibleNumber]->Database));
    SEARCH_INFO_TYPE sit;
    //int i;
    sit.BookNumber=_Book;
    sit.BookTableName = binfo.TableName;
    int firstThru=0;

    ui->pteVerses->clear();

    if (_AddBookChapter){
            v=getBookName(_Book);
            v = v+ " "+QString::number(Chapter);
        if (_AddVerse && _AddBookChapter){
            v = v+":";
            firstThru=1;
        }
        ui->pteVerses->appendPlainText(v);
    }



    do {
        sit.VerseNumber=Verse;
        sit.ChapterNumber=Chapter;

        v=bibleDatabaseClass->GetPlainVerse(0,&sit,bibleDatabaseClass->getDbType());
        if (_AddVerse){
            v = QString::number(Verse)+" "+v;
        }
        // first verse gets butted up against the chapter number and colon
        if (firstThru){
            firstThru=0;
            ui->pteVerses->moveCursor(QTextCursor::End);
            ui->pteVerses->insertPlainText(v);
            ui->pteVerses->moveCursor(QTextCursor::End);
        } else {
            ui->pteVerses->appendPlainText(v);
        }
        if (++Verse > bcv.NumberOfVerses[Chapter]){
            Verse =1;
            if (++Chapter > bcv.NumberOfChapters){
                break;// exit while loop
            }
        }
        // if we moved to the next chapter, the verse collection is finished.
        if (Chapter > _EndChapter )
            break;

        // if in the end chapter, and the vers is the end verse, finished.
        if ( Chapter == _EndChapter &&  Verse > _EndVerse)
                break; // finished with the verse copy into the text box


        if (Verse == 1){
            if (_AddBookChapter){
                v=getBookName(_Book);
                v = v+ " "+QString::number(Chapter);

                if (_AddVerse && _AddBookChapter){
                    v = v+":";
                }
                ui->pteVerses->appendPlainText(v);
                firstThru=1;
            }
        }

    } while(1);
}

//-----------------------------------------------------------------------------
// on the MAC, the past into email is a vcf card.  This is a QT bug for
// version 5.3.
//-----------------------------------------------------------------------------
void CopyDialog::on_pbCopy_clicked()
{
#ifdef Q_OS_MAC
    QString text=ui->pteVerses->document()->toPlainText();

    SystemCommandClass scc;

    QString command = "echo \""+text+"\" | /usr/bin/pbcopy";

    scc.run(command,true);

#else

    QString text=ui->pteVerses->document()->toPlainText();
    //ui->leTextForCopy->setText(text);
    QApplication::clipboard()->setText(text);
#endif
    close();

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::on_cbTranslation_currentIndexChanged(int index)
{
    if (dontUpdate)
        return;
    _BibleNumber = index;
    BuildVerses();

}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::on_cbStartChapter_currentIndexChanged(int index)
{
    if (dontUpdate)
        return;
    _StartChapter = index+1;
    BuildVerses();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::on_cbStartVerse_currentIndexChanged(int index)
{
    if (dontUpdate)
        return;
    _StartVerse=index+1;
    BuildVerses();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::on_cbEndChapter_currentIndexChanged(int index)
{
    if (dontUpdate)
        return;
    _EndChapter = index+1;
    BuildVerses();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::on_cbEndVerse_currentIndexChanged(int index)
{
    if (dontUpdate)
        return;
    _EndVerse=index+1;
    BuildVerses();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::on_pbClose_clicked()
{
    close();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::on_cbBibleBook_currentIndexChanged(int index)
{
    if (dontUpdate)
        return;
    _Book = index+1;
    BuildVerses();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::on_cbAddVerse_clicked()
{
    _AddVerse=ui->cbAddVerse->isChecked();
    BuildVerses();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CopyDialog::on_cbAddBookChapter_clicked()
{
    _AddBookChapter=ui->cbAddBookChapter->isChecked();
    BuildVerses();
}
