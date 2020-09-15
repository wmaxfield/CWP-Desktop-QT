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
*/#include "selectbiblesdialog.h"
#include "ui_selectbiblesdialog.h"
#include "misclibrary.h"
#include "preferencesmodule.h"
#include "notesdbclass.h"
#include "commentarymodule.h"
#include "WebDefinitionModule.h"
#include "PreferencesLibrary.h"

#include "mainwindow.h"
#include <QFileInfo>
#include <QDir>


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
SelectBiblesDialog::SelectBiblesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectBiblesDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close

    setModal(true);

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
SelectBiblesDialog::~SelectBiblesDialog()
{
    delete ui;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void SelectBiblesDialog::showEvent(QShowEvent *se) {
    Q_UNUSED(se);

    QRect r;
    QRect mwr=fvcp->geometry();

    r.setX(mwr.x()+mwr.width()/2-width()/2);
    r.setY(mwr.y()+mwr.height()/3);
    move(r.x(),r.y());
    // load the list
    //QStringList l =getDirectoryListing(getBiblesDirectory());
/*
 * QListWidget* widget = new QListWidget();
widget->addItem("Foo");
// To Insert Item A specific row
widget->insertItem(0, "Bar");
// To Access Selected Item
widget->currentItem();
*/
    reloadData();


}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void SelectBiblesDialog::reloadData() {
    ui->listWidget->clear();
    QString item;
    struct indexAndSectionStruct indexPath;
    int section;

    for (section=0;section<4;section++) {
        switch (section) {
        case 0:
            item = "Selected";
            indexPath.type = -1; // selected
            break;
        case 1:
            item = "Available Bibles";
            indexPath.type = TYPE_BBL_TYPE;
            break;
        case 2:
            item = "Available Notes";
            indexPath.type = TYPE_NOTES_TYPE;
            break;
        case 3:
            item = "Available Commentaries";
            indexPath.type = TYPE_COMMENTARY_TYPE;
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
            indexPath.row =index;
            indexPath.section = section;

            cellStructType cell = cellForRowAtIndexPath(indexPath);
            if (cell.text.length()) {
                QListWidgetItem* pItem2 =new QListWidgetItem(cell.text);
                pItem2->setData(Qt::UserRole,cell.type);
                ui->listWidget->addItem(pItem2);
            }

        }
    }
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int SelectBiblesDialog::getNumberOfItems(int section) {
        int numRows;

        switch (section) {
            case 0:
                if (Preferences) {
                    numRows= Preferences->getNumberOfSelectedBooks()+Preferences->getNumberOfSelectedNotes()+Preferences->numberOfSelectedCommentaries;
                    return numRows;
                }
                break;

            case 1:
                return gNumberOfBibles - Preferences->getNumberOfSelectedBooks();
                //break;

            case 2:
                numRows= gNumberOfNotes-Preferences->getNumberOfSelectedNotes();
                if (numRows<0) {
                    numRows=0;
                }
                return numRows;
                //break;

            case 3:
                numRows = gNumberOfCommentaries - Preferences->numberOfSelectedCommentaries;
                if (numRows<0) {
                    numRows=0;
                }
                return numRows;
                //break;


            default:
                return 0;
                //break;
        }
        return 0;

}

void SelectBiblesDialog::on_pushButtonOK_clicked()
{
    // put ok here
    close();
}

void SelectBiblesDialog::on_pushButtonCancel_clicked()
{
        // cancel code here
    close();
}


// Customize the appearance of table view cells.
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
cellStructType  SelectBiblesDialog::cellForRowAtIndexPath(struct indexAndSectionStruct  indexPath) {


    BookHandlerClass *bookAtRow;
    int section=indexPath.section;
    int row =indexPath.row;
    int testRowIndex=0;
    int currentBookIndex=0;
    QString  n="";
    QString text="";
    cellStructType cell={"",0};
    int type=BIBLE_DATABASE_SUFFIX_TYPE;
    int numSelectedBooks = Preferences->getNumberOfSelectedBooks();


    switch (section) {
    // selected
        case 0:    // used bibles, etc    // if the section is 0, then is the name in the preferences list
            if (row < numSelectedBooks) {
                BookInfoStructType *bis=&Preferences->SelectedBooks[row];
                QFileInfo f(bis->Name);
                text = f.baseName();
                type = BOOK_DATABASE_SUFFIX_TYPE;
            }else {
                if (row<numSelectedBooks+Preferences->getNumberOfSelectedNotes()) {
                    QFileInfo f(Preferences->SelectedNotes[row-numSelectedBooks].Name);
                    text=f.baseName();
                    type = NOTES_DATABASE_SUFFIX_TYPE;
                } else {
                    QFileInfo f(Preferences->SelectedCommentaries[row-numSelectedBooks-Preferences->getNumberOfSelectedNotes()].Name);
                    text = f.baseName();
                    type = COMMENTARY_DATABASE_SUFFIX_TYPE;
                }

            }
            break;

            // available
        case 1:// available bibles
        {
            //   int type;

            while (currentBookIndex < Books.count() ) {

                while (currentBookIndex < Books.count()) {
                    bookAtRow = Books[currentBookIndex];
                    n = bookAtRow->name;

                    if ( Instr(n, getSuffixStringFromTypeInt(BIBLE_DATABASE_SUFFIX_TYPE))> 0) {
                        currentBookIndex++;
                        break;
                    }
                    currentBookIndex++;
                }

                QFileInfo f(n);

                if (isBibleNameInSelectedArray(f.baseName())) {
                    continue;
                }

                if (testRowIndex == row) {
                    break;
                }

                testRowIndex++;
            }
            QFileInfo f(n);
            text = f.baseName();
            type = BIBLE_DATABASE_SUFFIX_TYPE;
        }      break;

        case 2:// available notes
        {   // if section is #1, then if is in the selected list, then is not in this one
            int currentRowIndex=0;
            NotesDBClass *noteAtRow=nullptr;

            if (currentRowIndex >= gNumberOfNotes)
                break;

            while (currentRowIndex < gNumberOfNotes) {
                extern NotesDBClass *gNotes[];
                noteAtRow = ((NotesDBClass*)gNotes[currentRowIndex]) ;

                if (isNoteNameInSelectedArray(noteAtRow->_BibleName)) {
                    currentRowIndex++;
                    continue;
                }

                if (testRowIndex == row) {
                    break; // found it
                }

                currentRowIndex++; // may escape from loop below, so increment now
                testRowIndex++;

            }
            if (noteAtRow!=nullptr){
                QFileInfo f(noteAtRow->_BibleName);
                text  = f.baseName();
            } else {
                QFileInfo f(getFullPathOfUserDocumentsCWPDirectory()+"/"+USER_NOTES_FILE_DEFAULT_NAME+NOTE_DATABASE_SUFFIX);
                text  = f.baseName();
            }
            type = NOTES_DATABASE_SUFFIX_TYPE;

        }   break;

        case 3://available commentaries
        {

            while (currentBookIndex < Books.count() ) {

                while (currentBookIndex < Books.count()) {
                    bookAtRow = Books[currentBookIndex];
                    n = bookAtRow->name;
                    if ( Instr(n, getSuffixStringFromTypeInt(COMMENTARY_DATABASE_SUFFIX_TYPE))> 0) {
                        currentBookIndex++;
                        break;
                    }
                    currentBookIndex++;
                }

                // cAtRow = bookAtRow->Database;
                 QFileInfo f(n);

                if (isCommentaryNameInSelectedArray(f.baseName())) {
                    continue;
                }

                if (testRowIndex == row) {
                    break;
                }

                testRowIndex++;
            }
            QFileInfo f(n);

            text = f.baseName();
            type = COMMENTARY_DATABASE_SUFFIX_TYPE;
        }    break;

    }

    cell.text = text;
    cell.type = type;
    return cell;

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int SelectBiblesDialog::getSectionFromRow(int row) {
    int section;
    int offset =1;
    int numRows=0;

    for (section =0; section < 4; section++) {
        numRows += getNumberOfItems(section)+offset;
        if (row < numRows)
            return section;
        //offset++;
    }
    return 0;

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void SelectBiblesDialog::on_listWidget_clicked(const QModelIndex &index)
{

    CWPIndexPath indexPath;
    cellStructType cell;


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

#if 1
    //
    // ------------------------------------------------------------------------
    // the available bibles array was touched while the popup was visible
    // ------------------------------------------------------------------------
    //-(void)didTapOnBookInTableView:(UITableView *)tableView  IndexPath:(NSIndexPath *)indexPath
    {
        QString b;

        changeWasDueToUserInteraction = YES;
        currentScrollPosition=currentFVC->getPageScrollPosition();

        currentFVC->setCurrentVerseByScrollPosition();

        verseBeforeNumberOfBooksChanged = Preferences->getCurrentVerse();

        //int totalNumberOfSelectedBooks = Preferences->getNumberOfSelectedBooks()+Preferences->getNumberOfSelectedNotes()+Preferences->numberOfSelectedCommentaries;

        switch (indexPath.section) {
            case 0:        //in the "selected" area
                if (indexPath.row <= Preferences->getNumberOfSelectedBooks()) {
                    b=Preferences->SelectedBooks[indexPath.row-1].Name;
                    if (isBibleNameInSelectedArray(b)) {
                        removeBibleFromSelectedArrayByName(b);
                        numberOfBooksShownChanged = YES;
                    }
                }else {
                    if (indexPath.row <= Preferences->getNumberOfSelectedBooks()+Preferences->getNumberOfSelectedNotes()) {
                        b=Preferences->SelectedNotes[indexPath.row-1-Preferences->getNumberOfSelectedBooks()].Name;
                        if (isNoteNameInSelectedArray(b)) {
                            removeNoteFromSelectedArrayByName(b);
                            numberOfBooksShownChanged = YES;
                        }
                    }else {
                        b=Preferences->SelectedCommentaries[indexPath.row-1-Preferences->getNumberOfSelectedBooks()-Preferences->getNumberOfSelectedNotes()].Name;
                        if (isCommentaryNameInSelectedArray(b)) {
                            removeCommentaryFromSelectedArrayByName(b);
                            numberOfBooksShownChanged = YES;
                        }
                    }
                }
                break;

            case 1:// available bibles
            {
                extern int OpenBibleByNameAndPutInSelectedArray(QString Name, int index); // preferenceslibrary.m


                b = itemName;
                if (!isBibleNameInSelectedArray(b)) {
                    OpenBibleByNameAndPutInSelectedArray(b,Preferences->getNumberOfSelectedBooks());
                    Preferences->incrementNumberOfSelectedBooks();
                    numberOfBooksShownChanged = YES;
                }
            } break;

            case 2:// available notes
            { //it is in the "available" area


                if (!isNoteNameInSelectedArray(itemName)) {
                    OpenPersonalNotesByNameAndPutInSelectedArray(itemName);
                    Preferences->incrementNumberOfSelectedNotes();
                    saveSelectedNotes();
                    numberOfBooksShownChanged = YES;
                }

                reloadData();
                 fvcp->LoadWebPage();// force load now
                //fvcp->refreshWebPage = YES;
                return; // don't do the SaveSelectedBooks() below
                        //saveSelectedNotes(); not needed, done in the AddNoteToSelectedArray

            } break;

            case 3:// the commentaries
            { //it is in the "available" area

                if (!isCommentaryNameInSelectedArray(itemName)) {
                    OpenCommentaryByNameAndPutInSelectedArray( itemName);
                    numberOfBooksShownChanged = YES;
                }

                reloadData();
                 fvcp->LoadWebPage();// force load now
                //fvcp->refreshWebPage = YES;
                return; // don't do the SaveSelectedBooks() below
                        //saveSelectedBooks(); not needed, done in the OpenCommentaryByNameAndPutInSelectedArray

            } break;
        }

        saveSelectedBooks();

        reloadData();

        // [selectBooksPopOverController dismissPopoverAnimated:YES];
        //fvcp->refreshWebPage = YES;
         fvcp->LoadWebPage();// force load now
        //    currentFVC->LoadWebPage];
    }
#endif

}



