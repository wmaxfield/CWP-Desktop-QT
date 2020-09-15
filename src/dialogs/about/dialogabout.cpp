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
#include "dialogabout.h"
#include "ui_dialogabout.h"
#include <QSysInfo>  // gets mac/window version
#include <QString>
#ifdef Q_OS_LINUX
#include <inttypes.h>
#endif

#ifndef CWP_TITLE_STRING
#define CWP_TITLE_STRING "CWP Title Needs To be defined"
#endif
DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    ui->lblTitle->setText(CWP_TITLE_STRING);
    QString v = QT_VERSION_STR;
    ui->lblQtVersion->setText("Qt Library version: "+v+". Available at www.qt.io");
    v = "Application Build Date: ";
    v = v+__DATE__;
    v = v +", ";
    v = v +__TIME__;
    ui->lblDate->setText(v);

    scene = nullptr;

    ui->lblCWPVersion->setText(QString("Version :" CWP_VERSION_NUMBER));
//CWP_VERSION_NUMBER

    setGraphicsView(ui->IconView);
    setImage(":/icons/Pics/icon75x75.png");
}

void DialogAbout::setImage(QString imageName)
{
    QPixmap pixmap(imageName);
    scene->addPixmap(pixmap);
    _view->setScene(scene);
    _view->show();
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::on_pbOK_clicked()
{
    close();
}
//----------------------------------------------------------------
//----------------------------------------------------------------
void DialogAbout::setGraphicsView(QGraphicsView *view) {
    _view = view;
    if (_view) {
        _view->setStyleSheet("background:transparent; border: none;");
        scene = new QGraphicsScene(this);
        _view->setScene(scene);
        //LedOff();
    }
}
