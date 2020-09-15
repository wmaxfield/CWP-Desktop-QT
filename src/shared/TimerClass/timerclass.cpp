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
*/#include <QTimer>
#include "timerclass.h"
#include <QString>



//------------------------------------------------------------
//------------------------------------------------------------
TimerClass::TimerClass()
{
    fn =0;
    connect (&timer, SIGNAL(timeout()),this,SLOT(timerFired()));
    _delay = 500;
    userString="";
}
//------------------------------------------------------------
//------------------------------------------------------------
TimerClass::TimerClass(int delay,int singleShot,void *myfn) {
    setFunction(myfn);

    connect (&timer, SIGNAL(timeout()),this,SLOT(timerFired()));

    timer.setSingleShot(singleShot);

    timer.start(delay);// twice a second
    userString="";
}
//------------------------------------------------------------
//------------------------------------------------------------
void TimerClass::setFunction(void *myfn) {
  fn = (void (*)()) myfn;
  fns = (void (*)(QString s)) myfn;
}
//------------------------------------------------------------
//------------------------------------------------------------
void TimerClass::SingleShot(int delay) {
    timer.setSingleShot(1);

    timer.start(delay);// twice a second

}

//------------------------------------------------------------
//------------------------------------------------------------
void TimerClass::start(int delay) {
    timer.start(delay);
}
//------------------------------------------------------------
//------------------------------------------------------------
void TimerClass::stop() {
    timer.stop();
}

//------------------------------------------------------------
//------------------------------------------------------------
void TimerClass::setDelay(int mydelay) {
    _delay=mydelay;
}

//------------------------------------------------------------
//------------------------------------------------------------
void TimerClass::timerFired() {
    if (fn) {
        if (userString!="")
            (*fns)(userString);
        else
            (*fn)();
    }
}
