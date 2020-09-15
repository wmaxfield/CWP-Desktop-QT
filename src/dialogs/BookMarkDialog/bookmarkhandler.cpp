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
        //pickerViewArray = [gBookBookMarkDB getTopicArray];
        labelVerse->setText( TextBook(bookNumber.toInt() )+QString(" ")+chapter+QString(":")+verse);
        buttonAddBookmark->setText("Add bookmark to selected Topic");
    } else {
        //pickerViewArray = [gBookMarkDB getTopicArray];//[[NSArray arrayWithObjects:@"hello",@"test",nil] retain];
        labelVerse->setText(BibleBook(bookNumber.toInt() )+QString(" ")+chapter+QString(":")+verse);
        buttonAddBookmark->setText("Add verse to selected Topic");// forState:UIControlStateNormal];
    }
}
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
    //QDialog(parent),
    QMainWindow(parent),
    ui(new Ui::BookMarkHandler)
{
    ui->setupUi(this);
    touch = false; // true for phones???
    labelVerse = new QLabel("verse");
    buttonAddBookmark = new QPushButton(tr("    Add Book Mark    "));
    m_wheel1=0;

    makeBookMarkWheel();
    setCentralWidget(mBookMarks);
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
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::stop(int index) {
  //static int stopcount;
 //Qt::MouseButtons b = QApplication::mouseButtons();
  //stopcount++;
  selectedRow = index;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::onAddBookMarkClicked(){
    if (pickerViewArray.count()==0) {
        MsgBox("You must have some topics before adding a bookmark! Press 'Add A Topic'.");
        return;
    }
  //  DialogAddTopic *dat = new DialogAddTopic(this);// set for delete on close

   // dat->show();
    if (selectedRow> pickerViewArray.count()-6)
        selectedRow =pickerViewArray.count()-6;

    if (selectedRow < 0)
        selectedRow =0;

    QString topic;

    topic = pickerViewArray[selectedRow];
    gBookMarkDB->addBookmarkUnderTopic(topic,/* usingBook:*/bookNumber, /* andChapter:*/chapter,/* andVerse*/verse,/* isTexBook:*/isTextBook);
    Preferences->setCurrentVerse(verse.toInt()); // set the verse for any navigation away from this page for history purposes
    close();// we are finished
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::doUpdate(){
    if (m_wheel1)
        delete m_wheel1;

    m_wheel1 = new StringWheelWidget(touch);

    pickerViewArray = gBookMarkDB->getTopicArray();
    // only make wheel movable if more than one bookmark
    //if (BookMarks.count()>1)
        for (int k = 0 ; k < 5; k++)
            pickerViewArray.append("");



    m_wheel1->setItems( pickerViewArray );
    grid->addWidget( m_wheel1, wheelRow, 0,1,20,Qt::AlignCenter );

    connect(m_wheel1,SIGNAL(stopped(int)),this,SLOT(stop(int)));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BookMarkHandler::topicButtonClicked(){
    DialogAddTopic *dat = new DialogAddTopic(this);
    dat->show();

}

//-----------------------------------------------------------------------------
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

    //BookMarks << "Red" << "Magenta" << "Peach" << "Orange" << "Yellow" << "Citro" << "Green" << "Cyan" << "Blue" << "Violet";
 //   labelVerse = new QLabel("verse");
    grid->addWidget(labelVerse,row++,0,1,20, Qt::AlignHCenter);

    QPushButton *NewTopicButton = new QPushButton(tr("  New Topic "));
    connect(NewTopicButton, SIGNAL(clicked()), this, SLOT(topicButtonClicked()));
    grid->addWidget(NewTopicButton,row++,0,1,20, Qt::AlignHCenter);

    wheelRow=row;
    doUpdate();
    row++;


    connect(buttonAddBookmark, SIGNAL(clicked()), this, SLOT(onAddBookMarkClicked()));

    grid->addWidget( buttonAddBookmark, row++, 0,1,20, Qt::AlignHCenter);//, 1, 3 );


   QLabel *l = new QLabel("  ");
   grid->addWidget( l, row++, 0,1,20, Qt::AlignHCenter);//, 1, 3 );

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

