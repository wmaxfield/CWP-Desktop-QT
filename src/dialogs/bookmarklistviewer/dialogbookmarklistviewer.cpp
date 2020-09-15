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
*/#include "dialogbookmarklistviewer.h"
#include "ui_dialogbookmarklistviewer.h"
#include <QLabel>

#include "chapterversestructclass.h"
#include "mainwindow.h"
#include "cwpdefines.h"
#include "indexpath.h"
#include "mainwindow.h"
#include "preferencesmodule.h"
#include "dialoghistory.h"
#include "userhistoryclass.h"
#include "BibleDatabaseClass/bibledatabaseclass.h"
#include "bookhandlerclass.h"
#include "BookMarkDB.h"


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogBookMarkListViewer::DialogBookMarkListViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBookMarkListViewer)
{
    ui->setupUi(this);
    ui->listWidget->setStyleSheet("QListWidget::item { border-bottom: 1px solid black; border-left: 5px solid white; border-right: 5px solid white; border-top: 5px solid white; } ");

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));

    mainwindowWidth = this->width();
    mainwindowHeight = this->height();
    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogBookMarkListViewer::showEvent(QShowEvent *e) {
    Q_UNUSED(e);

    reloadData();
  //  ui->lineEdit->setFocus();

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogBookMarkListViewer::reloadData() {
    ui->listWidget->clear();

    int index;
    int Count = myResultsArray.count();
    QString t,t2;
    QStringList a;
    ChapterVerseStructType cvs;
    BibleBookInfoStructType info;
    int bblidx;
    BibleDatabaseClass *bbld;

    SEARCH_INFO_TYPE sit;

    bblidx= Preferences->SelectedBooks[0].Index;// always show first book's verses

    if (bblidx < 0) bblidx=0;

    bbld =(BibleDatabaseClass *) ((BookHandlerClass *)Books[bblidx])->Database;

    for (index =0; index < Count ;index++) {

        QListWidgetItem* pItem =new QListWidgetItem();
        pItem->setSizeHint(QSize(0,89));// ??
        ui->listWidget->addItem(pItem);

        QLabel *label = new QLabel(ui->listWidget);

        cvs = myResultsArray[index];
        info = getBibleBookInfo(cvs.BookNumber);

        sit.BookNumber = cvs.BookNumber;
        sit.ChapterNumber = cvs.Chapter;
        sit.VerseNumber = cvs.Verse;
        sit.BookTableName= info.TableName;



        t2 = bbld->GetPlainVerse(&globalSTI,&sit,bbld->getDbType());


        t = "<font size=\"+1\" face=\"helvetica\" color=\"dark blue\"><b>&nbsp;&nbsp;&nbsp;&nbsp;";
        t = t +  info.BookName + " "+QString::number(cvs.Chapter)+":"+QString::number(cvs.Verse);
        t = t + "</b></font><br /><br /><font face=\"helvetica\"  color=\"purple\">&nbsp;&nbsp;";
        t = t + t2.left(300);//t2.left(100);

        //This is the text.  ";
        //t = t + "This is the second part of the text how long is it and how many lines of label does it occupy.  Let's make it a little longer....</font>";
        label->setText(t);
        label->setWordWrap(true);
        label->setEnabled(false);
        t = QString::number(cvs.BookNumber)+":"+QString::number(cvs.Chapter)+":"+QString::number(cvs.Verse)+":"+QString::number(cvs.Posn);
        pItem->setData(Qt::UserRole,t);

        ui->listWidget->setItemWidget(pItem,label);

    }
}

//------------------------------------------------------------
// overrides resizeEvent to allow button movement
//------------------------------------------------------------
void DialogBookMarkListViewer::resizeEvent ( QResizeEvent * event ) {

    Q_UNUSED(event);
    /* QMessageBox msgBox;
       msgBox.setText("Resized");
       msgBox.exec();
       */
    //(void)event;
  // return;
    // move the quit button
   QRect r = ui->listWidget->geometry();
    int newx,newy,resizex,resizey;
    //r.setX(r.x()+20);
    newx = this->width();
    newy = this->height();
    resizex = newx - mainwindowWidth;
    resizey = newy - mainwindowHeight;
    //x = this->ui->pbQuit->pos().x();
    //y = this->ui->pbQuit->pos().y();
    //this->ui->pbQuit->move(x+resizex, y+resizey);
    //this->webView->width() = newx;
    r.setWidth(resizex +r.width());
    r.setHeight(resizey + r.height());

    ui->listWidget->setGeometry(r);


    r = ui->pbClose_2->geometry();
    int width = r.width();
    int height = r.height();
    r.setX(r.x()+resizex/2);
    r.setY(r.y()+resizey);
    r.setHeight(height);
    r.setWidth(width);
    ui->pbClose_2->setGeometry(r);


    // newy - 50;
    mainwindowWidth = newx;
    mainwindowHeight = newy;
    //activityIndicator->stopAnimation();

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogBookMarkListViewer::~DialogBookMarkListViewer()
{
    delete ui;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogBookMarkListViewer::on_listWidget_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    //int a = 4;
    ChapterVerseStructType cvs;
   // cvs = userHistory->getHistoryItemByIndex(index.row(),NO);

    QStringList a;
    QListWidgetItem *itemPtr = ui->listWidget->currentItem();
    QString t;

    t = itemPtr->data(Qt::UserRole).toString();
    a = t.split(":");
    if (a.count()<3) {
        return;// dont' do anything if an error
    }
    cvs.BookNumber = a[0].toInt();
    cvs.Chapter = a[1].toInt();
    cvs.Verse = a[2].toInt();
    cvs.Posn = a[3].toInt();

    userHistory->pushHistoryItemWithBook(Preferences->getCurrentBook(),Preferences->getCurrentChapter(),Preferences->getCurrentVerse(),fvcp->getPageScrollPosition());

    Preferences->setCurrentBook(cvs.BookNumber);
    Preferences->setCurrentChapter(cvs.Chapter);
    Preferences->setCurrentVerse(cvs.Verse);
    Preferences->currentPosition = 0;

    fvcp->doScrollToVerseAfterWebPageLoad=TRUE;
    fvcp->verseToScrollToAfterWebPageLoad=cvs.Verse;
    fvcp->LoadWebPage();

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogBookMarkListViewer::on_pbClose_2_clicked()
{
    close();
}
void DialogBookMarkListViewer::deleteItem(){
       for (int i = 0; i < ui->listWidget->selectedItems().size(); ++i) {
           // remove curent item on selected row
           bookmark = namesArray[i];
           QListWidgetItem *item = ui->listWidget->takeItem(i);
           gBookMarkDB->deleteBookMark(bookmark,topic);
           // now delete it
           delete item;
       }
}
void DialogBookMarkListViewer::showContextMenu(const QPoint &pos)
{
    // Handle global position
       QPoint globalPos = ui->listWidget->mapToGlobal(pos);

       // Create menu and insert some actions
       QMenu myMenu;
       //myMenu.addAction("Insert", this, SLOT(addItem()));
       myMenu.addAction("Delete",  this, SLOT(deleteItem()));

       // Show context menu at handling position
       myMenu.exec(globalPos);
}
