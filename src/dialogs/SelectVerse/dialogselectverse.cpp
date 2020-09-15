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
*/#include "dialogselectverse.h"
#include "ui_dialogselectverse.h"
#include "bibleinfo.h"





//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DialogSelectVerse::DialogSelectVerse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectVerse)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close

    for (int i=1; i <= NUM_BOOKS_IN_BIBLE;i++)
        ui->comboBook->addItem(BibleBook(i));

    delegate = 0;
    book=chapter=verse=1;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DialogSelectVerse::~DialogSelectVerse()
{
    delete ui;
}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSelectVerse::on_pbCancel_clicked()
{

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSelectVerse::on_pbOK_clicked()
{
    if (delegate) {
        delegate->addBookChapterVerse(book,chapter,verse);
    }
    hide();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSelectVerse::on_comboBook_currentIndexChanged(int index)
{
    book = index+1;
     GetBookChapterVerse(book,bcv);


     // fill in the number of chapters, and select chapter 1.
     ui->comboChapter->clear();
     for (int i = 1 ; i <= bcv.NumberOfChapters; i++)
         ui->comboChapter->addItem(QString::number(i));

     ui->comboChapter->setEnabled(YES);
     ui->comboChapter->setCurrentIndex(0);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSelectVerse::on_comboVerse_currentIndexChanged(int index)
{
    // nothing to be done here
    verse = index+1;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSelectVerse::on_comboChapter_currentIndexChanged(int index)
{
    //
    GetBookChapterVerse(book,bcv);
    chapter = index+1;

    ui->comboVerse->clear();
    for (int i=1;i <= bcv.NumberOfVerses[chapter];i++)
        ui->comboVerse->addItem(QString::number(i));

    ui->comboVerse->setCurrentIndex(0);
    ui->comboVerse->setEnabled(YES);
}
