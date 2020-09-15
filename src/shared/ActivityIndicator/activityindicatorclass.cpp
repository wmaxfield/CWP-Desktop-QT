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
*/#include "activityindicatorclass.h"
#include "qmainwindow.h"

ActivityIndicatorClass::ActivityIndicatorClass(QMainWindow *mw)
{
    QGridLayout *layout = new QGridLayout(mw->centralWidget());
    int i=0;
    int j=0;
    //QWidget *wp = new QWidget;
    ind = new BusyIndicator(mw);
    layout->setAlignment(ind,Qt::AlignCenter);
    ind->setColor(Qt::gray);
    ind->setIndicatorStyle(BusyIndicator::StyleRect);
    layout->addWidget(ind, i, j );
    mw->setCentralWidget(mw->centralWidget());

    //ind->stop();
   _isAnimating=0;
   hidesWhenStopped=1;
   stopAnimation();
}

ActivityIndicatorClass::ActivityIndicatorClass()
{

   _isAnimating=0;
}
int ActivityIndicatorClass::isAnimating() {
    return _isAnimating;
}
