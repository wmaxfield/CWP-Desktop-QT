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
*/#include "dialogselectbookinbible.h"
#include "ui_dialogselectbookinbible.h"
#include "mainwindow.h"
#include "misclibrary.h"
#include "preferencesmodule.h"
#include "notesdbclass.h"
#include "commentarymodule.h"
#include "WebDefinitionModule.h"
#include "PreferencesLibrary.h"
#include "dialogselectchapterinbook.h"
#include "../shared/bibleinfo.h"


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogSelectBookInBible::DialogSelectBookInBible(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectBookInBible)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close
    setModal(true);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogSelectBookInBible::~DialogSelectBookInBible()
{
    delete ui;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSelectBookInBible::showEvent(QShowEvent *se) {
    Q_UNUSED(se);

    reloadData();


}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSelectBookInBible::reloadData() {
    ui->listWidget->clear();
    QString item;
    struct indexAndSectionStruct indexPath;
    int section;

    indexPath.row=0;

    for (section=0;section<10;section++) {
        switch (section) {
        case 0:
            item = "The Torah (The Law)";
            break;
        case 1:
            item = "History";
            break;
        case 2:
            item = "Poetry";
            break;
        case 3:
            item = "Major Prophets";
            break;
        case 4:
            item = "Minor Prophets";
            break;
        case 5:
            item = "Gospels";
            break;
        case 6:
            item = "Beginning of the Church";
            break;
        case 7:
            item = "Paul's Letters";
            break;
        case 8:
            item = "Other Letters";
            break;
        case 9:
            item = "The Revelation";
            break;
        default:
            item = "unknown section";
            break;
        }

        // add the "heading" item
        QListWidgetItem* pItem =new QListWidgetItem(item);
           pItem->setForeground(Qt::white);   // sets white text
           pItem->setBackground(Qt::gray); // sets gray background
           pItem->setData(Qt::UserRole,HEADING_TYPE);
           ui->listWidget->addItem(pItem);


        // now add the items in the section

        int numItems = getNumberOfItems(section);

        for ( int index = 0; index < numItems; index++) {
            indexPath.row ++;
            indexPath.section = section;

            BibleBookInfoStructType cell = cellForRowAtIndexPath(indexPath);
            if (cell.BookName.length()>0) {
                QListWidgetItem* pItem2 =new QListWidgetItem(cell.BookName);
                pItem2->setData(Qt::UserRole,cell.BookNumber);
                ui->listWidget->addItem(pItem2);
            }

        }
    }
}
// each section has a certain number of ooks
int NumRowsinSectionForBible[]=
{
 NumBooksInLaw,  //             5
 NumBooksInHistory ,  //         12
 NumBooksInPoetry,  //          5
 NumBooksInMajorProphet,  //    5
 NumBooksInMinorProphet,  //    12
 NumBooksInGospels,  //         4
 NumBooksInChurchHistory,  //   1
 NumBooksInPaulineEpistles,  // 14
 NumBooksInGeneralEpistles,  // 7
 NumBooksInBookOfProphecy  //  1

};
// the book number for the row click
int BibleBookValue[] = {
    0,
    1,2,3,4,5,//5
    0,
    6,7,8,9,10,11,12,13,14,15,16,17,//12
    0,
    18,19,20,21,22,//5
    0,
    23,24,25,26,27,//5
    0,
    28,29,30,31,32,33,34,35,36,37,38,39,//12
    0,
    40,41,42,43,//4
    0,
    44,//1
    0,
    45,46,47,48,49,50,51,52,53,54,55,56,57,58,//14
    0,
    59,60,61,62,63,64,65,
    0,
    66
};

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int DialogSelectBookInBible:: getNumberOfItems(int section) {
    // Return the number of rows in the section.
    return NumRowsinSectionForBible[section];
}

// Customize the appearance of table view cells.
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BibleBookInfoStructType  DialogSelectBookInBible::cellForRowAtIndexPath(struct indexAndSectionStruct  indexPath) {

      return getBibleBookInfo(indexPath.row);

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int DialogSelectBookInBible::getSectionFromRow(int row) {
    int section;
    int offset =1;
    int numRows=0;

    for (section =0; section < 9; section++) {
        numRows += getNumberOfItems(section)+offset;
        if (row < numRows)
            return section;
        //offset++;
    }
    return 0;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogSelectBookInBible::on_listWidget_clicked(const QModelIndex &index)
{

    CWPIndexPath indexPath;
    BibleBookInfoStructType cell;


   indexPath.row = index.row();
   indexPath.section = getSectionFromRow(indexPath.row);

   if (getNumberOfItems(indexPath.section)<=0)
       return;

   QListWidgetItem *itemPtr;

    itemPtr=ui->listWidget->currentItem();
    QString itemName = itemPtr->text();
    int itemPicked;
     itemPicked = itemPtr->data(Qt::UserRole).toInt();
    if (itemPicked==HEADING_TYPE )
        return;

    cell=  getBibleBookInfo(BibleBookValue[indexPath.row]);

    // later, pick up the chapter and verse we were in on this book
    Preferences->setCurrentVerse( 1);
    Preferences->setCurrentBook( cell.BookNumber);
   // Preferences->setCurrentChapter( 1); // first chapter

    DialogSelectChapterInBook *scib=new DialogSelectChapterInBook();
    if (!scib) {
        fvcp->LoadWebPage();
        close();
        return;
    }

    scib->setGeometry(this->geometry());// position over top of books
    scib->book = cell.BookNumber;
    scib->bookName = itemName;
    scib->show();

    //fvcp->LoadWebPage();// force going to that book
    // now close ourselves
    close();


}


void DialogSelectBookInBible::on_pbClose_clicked()
{
    close();
}
