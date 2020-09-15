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
*/#include "mainwindow.h"
#include <QApplication>
#include "initialization.h"
#include <QString>

MainWindow *currentFVC;
MainWindow *fvcp;


initialization *initObject;

int numberOfBooksShownChanged;
int changeWasDueToUserInteraction;
int currentScrollPosition;
int verseBeforeNumberOfBooksChanged;
QString debugFlag="";

int main(int argc, char *argv[])
{
    // argv[0]== MYSELF on Mac, Linux.  Windows???
    //Q_INIT_RESOURCE(application);
    QApplication a(argc, argv);

    a.setOrganizationName("CrossWordProject");
    a.setApplicationName("CrossWord Bible Program");

    // initalize CWP by creating the init object
    initObject = new initialization();

    // this is looked at in mainwindow.cpp,
    // expecting debug=1 for turning in debug
    if (QCoreApplication::arguments().size()>1){
       debugFlag = QCoreApplication::arguments().at(1);
    }
    if (initObject->failed){
       // a.aboutQt();
        return -1;
    }

    MainWindow w;


    w.show();



    
    int r;

    r=a.exec();

    return r;
}
/*
some icons from openclipart.org
*/
