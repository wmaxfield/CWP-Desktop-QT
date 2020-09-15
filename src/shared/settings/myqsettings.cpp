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
*/#include "myqsettings.h"
#include <QDir>
//#include "ivbDefines.h"
#include "cwpdefines.h"

QSettings *settings;

void initQSettings(void) {
    if (settings) {
        delete settings;
    }
#ifdef Q_OS_MAC
    QString s =QDir::homePath()+"/Library/Application Support/"+MY_PROGRAM_NAME;
    QDir dir(s);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    s = s + "/settings.plist";
    settings = new QSettings(s,QSettings::NativeFormat);
#endif
#ifdef Q_OS_WIN32

    settings = new QSettings(QSettings::IniFormat,QSettings::UserScope,MY_PROGRAM_NAME);

  //  extern  void MessageBox(QString s);
  // MessageBox( settings->fileName());
#endif
#ifdef Q_OS_LINUX
    settings = new QSettings(QSettings::IniFormat,QSettings::UserScope,MY_PROGRAM_NAME);
#endif

}

QString getSetting(QString group,QString Key, QString Default) {
    QString s;

    settings->beginGroup(group);

    s=  settings->value(Key,Default).toString();
    settings->endGroup();

    return s;

}


void setSetting(QString group,QString Key, QString value){
   // QString s;
    settings->beginGroup(group);

     settings->setValue(Key,value);

     settings->endGroup();
}
