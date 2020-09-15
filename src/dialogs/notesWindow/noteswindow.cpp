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
*///
//  NotesViewController.m
//  Crossword-Student
//
//  Created by WADE MAXFIELD on 11/17/10.
//  Copyright 2010 CrossWordProject. All rights reserved.
//
#include "noteswindow.h"
#include "ui_noteswindow.h"
#include "bibleinfo.h"
#include "misclibrary.h"
#include "structs.h"
#include "HTMLModule.h"
#include "mainwindow.h"
#include "notesdbclass.h"
#include "dialogselectverse.h"


DialogSelectVerse *dsvp;
//------------------------------------------------------------
// overrides resizeEvent to allow button movement
//------------------------------------------------------------
void NotesWindow::resizeEvent ( QResizeEvent * event ) {

    Q_UNUSED(event);

    // move the quit button
   QRect r = ui->textEdit->geometry();
           int newx,newy,resizex,resizey,x,y;
    //r.setX(r.x()+20);
    newx = this->width();
    newy = this->height();
    resizex = newx - wWidth;
    resizey = newy - wHeight;
    //x = this->ui->pbQuit->pos().x();
    //y = this->ui->pbQuit->pos().y();

    //this->ui->pbQuit->move(x+resizex, y+resizey);
    //this->webView->width() = newx;
    r.setWidth(resizex +r.width());
    r.setHeight(resizey + r.height());

    ui->textEdit->setGeometry(r);
    // newy - 50;
    wWidth = newx;
    wHeight = newy;
    //activityIndicator->stopAnimation();
    x = this->ui->pbCancel->pos().x();
    y = this->ui->pbCancel->pos().y();
    this->ui->pbCancel->move(x+resizex,y+resizey);
    x = this->ui->pbOK->pos().x();
    y = this->ui->pbOK->pos().y();
    this->ui->pbOK->move(x+resizex,y+resizey);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
NotesWindow::NotesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NotesWindow)
{
    ui->setupUi(this);
    dontReloadText=
    chapter =
    bookNumber =
    verse =
    noteIndex = 0;
    wWidth=this->width();
    wHeight=this->height();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
NotesWindow::~NotesWindow()
{
    delete ui;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::showEvent(QShowEvent *e) {
    Q_UNUSED(e);
    NotesDBClass *notedb;
    extern NotesDBClass *getNotesDB(int);

    if (dontReloadText) {
        dontReloadText=NO;
        return;
    }


    notedb = getNotesDB(noteIndex);
    static BibleBookInfoStruct bbi;

    bbi = getBibleBookInfo(bookNumber);
    QString   textFromDB=  notedb->GetTextFromDatabaseByBook(bookNumber,chapter,verse);// retain];
    ui->textEdit->setHtml(textFromDB);
    this->setWindowTitle(bbi.BookName +" "+ QString::number(chapter)+":"+QString::number(verse));
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::addBookChapterVerse(int Book,int Chapter, int Verse) {
    BibleBookInfoStructType bbis = getBibleBookInfo(Book);
     //bookName = bbis.BookName;

        QString link;

        link= " " HTML_HREF_STARTQ USER_XREF+ QString::number(Book)+"_"+QString::number(Chapter)+ HTML_COLON_F +QString::number(Verse);
        link = link + QUOTE HTML_GENERIC_END_TAG+bbis.BookName+ " "+QString::number(Chapter)+":"+QString::number(Verse)+ HTML_HREF_END " ";
        ui->textEdit->append(link);

        if (dsvp)
            dsvp->hide();
}
// ------------------------------------------------------------------------
// NOTE: This heavily depends upon the format of the html string from
// QT.  It will probably break in future versions.
// ------------------------------------------------------------------------
QString parseForNoteSave(QString s) {

    QStringList array;
    QStringList t;
    array = s.split("\n");  //when they stop using \n for some of this, it will break;

    // remove the first three parts
    array.removeFirst();
    array.removeFirst();
    array.removeFirst();
    array.removeFirst();

    s = array.join("");
    array = s.split("</p>");// remove paragraphs  (paragraphs will have to have <br /> added to end due to webkit formatting decisions)

    array[array.count()-1]=RemoveAll(array[array.count()-1],"</body>","</html>");

    for (int index=0 ; index < array.count(); index++ ) {
        t = array[index].split(">");// remove the paragraph style
        t.removeFirst();
        array[index]=t.join(">");
        if (Instr(array[index],HTML_BRStr)==0) {
            array[index]= array[index]+HTML_BRStr;
        }

    }

    return array.join("");
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::saveNote() {

    //fvcp->startWaitIndicator();
    NotesDBClass *notedb;
    extern NotesDBClass *getNotesDB(int);
    QString s;

    // for now, we only have on selected note on the display.  later we will have to figure out which note is selected?????
        notedb = getNotesDB(noteIndex);
        if (notedb) {
            s=ui->textEdit->toHtml();
            s = parseForNoteSave(s);
            notedb->SaveNoteToDatabase(s,/*BookNumber*/bookNumber,/*Chapter:*/chapter,/* Verse:*/verse);
        }


}

void NotesWindow::setPlainText(QString Text) {
    ui->textEdit->setPlainText(Text);
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::on_pbItalic_clicked()
{
   ui->textEdit->setFontItalic(true);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::on_pbOK_clicked()
{
    this->hide();
    saveNote();
    fvcp->LoadWebPage();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::on_pbNoItalic_clicked()
{
    ui->textEdit->setFontItalic(false);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::on_pbCancel_clicked()
{
    this->hide();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::on_pbBoldOn_clicked()
{
    ui->textEdit->setFontWeight(QFont::Bold);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::on_pbBoldOff_clicked()
{
    ui->textEdit->setFontWeight(QFont::Normal);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::on_pbUnderlineOn_clicked()
{
    ui->textEdit->setFontUnderline(YES);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::on_pbUnderlineOff_clicked()
{
    ui->textEdit->setFontUnderline(NO);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void NotesWindow::on_pbVerse_clicked()
{
    // show verse reference window here
    if (!dsvp) {
        dsvp = new DialogSelectVerse();
    }
    dsvp->delegate = this;// indicate we are to be called
    dsvp->show();
}
