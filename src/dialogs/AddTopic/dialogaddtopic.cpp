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
//  AddBibleBookMarkTopic.m
//  Crossword-Try3
//
//  Created by WADE MAXFIELD on 10/30/2010.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//  wsm 8-5-2011 fix button memory leak
#include "dialogaddtopic.h"
#include "ui_dialogaddtopic.h"
#include "BookMarkDB.h"
#include "timerclass.h"
#include "bookmarkhandler.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DialogAddTopic::DialogAddTopic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddTopic)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DialogAddTopic::~DialogAddTopic()
{
    delete ui;
}

void DialogAddTopic::showEvent(QShowEvent *e)
{
    ui->lineEditTopic->setFocus();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DialogAddTopic::on_buttonBox_accepted()
{
    // give topic to the BookMarks Array
    if (ui->lineEditTopic->text().length()>0) {
        //newTopic = ui->lineEditTopic->text();
        gBookMarkDB->addTopic(ui->lineEditTopic->text());

        bmh->doUpdate();
    }

    close();


}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DialogAddTopic::on_buttonBox_rejected()
{
    close();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DialogAddTopic::on_lineEditTopic_editingFinished()
{

}
