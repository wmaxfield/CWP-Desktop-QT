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
//  BookMarkManage.m
//  Crossword-Try3
//
//  Created by WADE MAXFIELD on 10/31/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//
#include <QtWidgets>
#include "bookmarkhandler.h"
#include "ui_bookmarkhandler.h"
#include "wheelwidget.h"
#include "BookMarkDB.h"
#include "mainwindow.h"
#include "dialogaddtopic.h"
#include "bibleinfo.h"

extern QString TextBook(int BookNumber);//BookManipulationLibary.m
extern BookMarkHandler *bmh;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::showEvent(QShowEvent *e){
    Q_UNUSED(e);
    if (isTextBook) {
        labelVerse->setText( TextBook(bookNumber )+QString(" ")+QString::number(chapter)+QString(":")+QString::number(verse));
        buttonAddBookmark->setText(tr("Add bookmark to selected Topic"));
    } else {
        labelVerse->setText(BibleBook(bookNumber )+QString(" ")+QString::number(chapter)+QString(":")+QString::number(verse));
        buttonAddBookmark->setText(tr("Bookmark verse under selected Topic"));
    }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::on_actionAlways_on_Top_triggered(bool checked)
{
    Qt::WindowFlags flags = this->windowFlags();
    if (checked)
    {
        this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        this->show();
    }
    else
    {
        this->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
        this->show();
    }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BookMarkHandler::BookMarkHandler(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BookMarkHandler)
{
    ui->setupUi(this);
    touch = false; // true for phones???
    labelVerse = new QLabel("verse");
    buttonAddBookmark = new QPushButton(tr("    Add Book Mark    "));
    m_wheel1=0;

    makeBookMarkWheel();
    setCentralWidget(mBookMarks);// put onto screen
    addBookMarkTimer=0; // zero out for later

    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close
     //this->setModal(true);<-- not available on a mainwindow
    on_actionAlways_on_Top_triggered(true);
    isTextBook=NO;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BookMarkHandler::BookMarkHandler(QWidget *parent,int book,int ch, int v,int _isTextBook) :
    QMainWindow(parent),
    ui(new Ui::BookMarkHandler)
{
    bookNumber=book;
    chapter = ch;
    verse = v;
    isTextBook = _isTextBook;


    ui->setupUi(this);
    touch = false; // true for phones???
    labelVerse = new QLabel("verse");
    labelLine  = new QLabel(" ");
    labelLine2 = new QLabel(" ");
    buttonAddBookmark = new QPushButton(tr("    Add Book Mark    "));
    m_wheel1=0;

    makeBookMarkWheel();
    setCentralWidget(mBookMarks);// put onto screen
    addBookMarkTimer=0; // zero out for later

    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close
     //this->setModal(true);
    on_actionAlways_on_Top_triggered(true);
    isTextBook=NO;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BookMarkHandler::~BookMarkHandler()
{
    delete ui;
    if (addBookMarkTimer)
        delete addBookMarkTimer;

    delete labelVerse;
    delete labelLine;
    delete labelLine2;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::stop(int index) {
  selectedRow = index;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::onAddBookMarkClicked(){
    if (pickerViewArray.count()==0) {
        MsgBox("You must have some topics before adding a bookmark! Press 'Add A Topic'.");
        return;
    }

    if (selectedRow> pickerViewArray.count()-6)
        selectedRow =pickerViewArray.count()-6;

    if (selectedRow < 0)
        selectedRow =0;

    QString topic;

    topic = pickerViewArray[selectedRow];
    gBookMarkDB->addBookmarkUnderTopic(topic,/* usingBook:*/QString::number(bookNumber), /* andChapter:*/QString::number(chapter),/* andVerse*/QString::number(verse),/* isTexBook:*/isTextBook);
    Preferences->setCurrentVerse(verse); // set the verse for any navigation away from this page for history purposes
    close();// we are finished
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::doUpdate(){
    if (m_wheel1)
        delete m_wheel1;

    m_wheel1 = new StringWheelWidget(touch);

    //------
    // create the topics for the selector wheel
    pickerViewArray = gBookMarkDB->getTopicArray();
     for (int k = 0 ; k < 5; k++)
        pickerViewArray.append("");

    m_wheel1->setItems( pickerViewArray );
    //-------

    grid->addWidget( m_wheel1, wheelRow, 0,1,20,Qt::AlignCenter );

    connect(m_wheel1,SIGNAL(stopped(int)),this,SLOT(stop(int)));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::addTopicButtonClicked(){
    DialogAddTopic *dat = new DialogAddTopic(this);
    dat->show();

}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::deleteTopicButtonClicked(){
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, tr("Delete Topic"), tr("Delete This Topic?"),
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
          QString topic;

          topic = pickerViewArray[selectedRow];
        gBookMarkDB->deleteBookMarkTopicOrCategory(topic);
        // remove the topic
        pickerViewArray = gBookMarkDB->getTopicArray();
         for (int k = 0 ; k < 5; k++)
            pickerViewArray.append("");

        m_wheel1->setItems( pickerViewArray );
        selectedRow=0; //point to first item

      } else {
        //qDebug() << "Yes was *not* clicked";
          return;
      }
}
//-----------------------------------------------------------------------------
// create the wheel and the buttons around it
//-----------------------------------------------------------------------------
void BookMarkHandler::makeBookMarkWheel()
{
    if (QApplication::desktop()->width() > 1000) {
        QFont f = font();
        f.setPointSize(f.pointSize() * 2);
        setFont(f);
    }

    mBookMarks = new QWidget(this);
    grid = new QGridLayout(mBookMarks);
    grid->setSpacing(10);


    int row = 0;

    grid->addWidget(labelVerse,row++,0,1,20, Qt::AlignHCenter);

    QPushButton *NewTopicButton = new QPushButton(tr("  New Topic "));
    connect(NewTopicButton, SIGNAL(clicked()), this, SLOT(addTopicButtonClicked()));
    grid->addWidget(NewTopicButton,row,0,1,20, Qt::AlignRight);

    QPushButton *DeleteTopicButton = new QPushButton(tr("  Delete Topic "));
    connect(DeleteTopicButton, SIGNAL(clicked()), this, SLOT(deleteTopicButtonClicked()));
    grid->addWidget(DeleteTopicButton,row++,0,1,20, Qt::AlignLeft);

    // add a line to do some visual separation from delete/add buttons
    grid->addWidget(labelLine,row++,0,1,20, Qt::AlignHCenter);


    wheelRow=row;
    doUpdate();
    row++;


    connect(buttonAddBookmark, SIGNAL(clicked()), this, SLOT(onAddBookMarkClicked()));

    grid->addWidget( buttonAddBookmark, row++, 0,1,20, Qt::AlignHCenter);//, 1, 3 );


 //  QLabel *l = new QLabel("  ");
   grid->addWidget( labelLine2, row++, 0,1,20, Qt::AlignHCenter);//, 1, 3 );

   QPushButton *shakeButton = new QPushButton(tr("    Cancel    "));
   connect(shakeButton, SIGNAL(clicked()), this, SLOT(pbOK_clicked()));
   grid->addWidget( shakeButton, row++, 0,1,20, Qt::AlignHCenter);//, 1, 3 );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::pbOK_clicked()
{
    fvcp->activateWindow();
    close();
}

