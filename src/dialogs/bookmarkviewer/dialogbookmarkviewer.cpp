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
*/#include "dialogbookmarkviewer.h"
#include "ui_dialogbookmarkviewer.h"
#include "BookMarkDB.h"
#include "dialogbookmarklistviewer.h"
#include "mainwindow.h"

DialogBookMarkViewer   * bmvPtr ;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DialogBookMarkViewer::DialogBookMarkViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBookMarkViewer)
{

    ui->setupUi(this);
    ui->listWidget->setStyleSheet("QListWidget::item { border-bottom: 1px solid black; border-left: 5px solid white; border-right: 5px solid white; border-top: 5px solid white; } ");

    mainwindowWidth = this->width();
    mainwindowHeight = this->height();

    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close
    this->setModal(true);

}
//------------------------------------------------------------
// overrides resizeEvent to allow button movement
//------------------------------------------------------------
void DialogBookMarkViewer::resizeEvent ( QResizeEvent * event ) {

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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DialogBookMarkViewer::reject()
{
    bmvPtr = nullptr;
    QDialog::reject();

}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DialogBookMarkViewer::showEvent(QShowEvent *e){
    Q_UNUSED(e);
    pickerViewArray = gBookMarkDB->getTopicArray();
    int index;


    for (index = 0 ; index < pickerViewArray.count(); index++) {
        QListWidgetItem* pItem =new QListWidgetItem();
        pItem->setSizeHint(QSize(0,40));// ??
        pItem->setTextAlignment(Qt::AlignCenter);
        pItem->setText(pickerViewArray[index]);
        ui->listWidget->addItem(pItem);
    }

    QRect r;
    QRect mwr=fvcp->geometry();

    r.setX(mwr.x()+mwr.width()/2-width()/2);
    r.setY(mwr.y()+mwr.height()/3);
    move(r.x(),r.y());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DialogBookMarkViewer::~DialogBookMarkViewer()
{
    delete ui;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DialogBookMarkViewer::on_listWidget_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QListWidgetItem *itemPtr = ui->listWidget->currentItem();
    close(); // close me

    DialogBookMarkListViewer *dbmlvp;

    dbmlvp= new DialogBookMarkListViewer();
    QString topic=itemPtr->text();
    dbmlvp->topic = topic;

    dbmlvp->myResultsArray = gBookMarkDB->getBookmarkNamesCVSArrayForTopic(topic);
    dbmlvp->namesArray     = gBookMarkDB->getBookmarkNamesArrayForTopic(topic);
    dbmlvp->show();

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DialogBookMarkViewer::on_pbClose_2_clicked()
{
    bmvPtr=nullptr;
    close();
}
