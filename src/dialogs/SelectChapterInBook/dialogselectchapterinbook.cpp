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
#include "dialogselectchapterinbook.h"
#include "ui_dialogselectchapterinbook.h"
#include "preferencesmodule.h"
#include "mainwindow.h"

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogSelectChapterInBook::DialogSelectChapterInBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectChapterInBook)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close
#ifdef Q_OS_LINUX
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);// wsm fix linux modality problem
#endif
    setModal(true);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogSelectChapterInBook::~DialogSelectChapterInBook()
{
    delete ui;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSelectChapterInBook::showEvent(QShowEvent *) {
    this->setWindowTitle("Select Chapter For "+bookName);
    ui->listWidget->clear();
    int index;
    //int numChaptersInBook;
    BookChapterVerseStructType bcv;
    GetBookChapterVerse(book,bcv);

    for (index = 0 ; index < bcv.NumberOfChapters;index++ ){
        QListWidgetItem* pItem =new QListWidgetItem(QString::number(index+1));
        ui->listWidget->addItem(pItem);
    }

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSelectChapterInBook::on_pbClose_clicked()
{
    close();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSelectChapterInBook::on_listWidget_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)

   QListWidgetItem *itemPtr;

    itemPtr=ui->listWidget->currentItem();
    QString itemName = itemPtr->text();

    Preferences->setCurrentChapter(itemName.toInt());
    globalSTI.DoScrollToVerse=true;
    globalSTI.VerseToScrollTo=1;
    fvcp->LoadWebPage();// force going to that book
    // now close ourselves
    close();
}
