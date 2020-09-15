#ifndef MYQSETTINGS_H
#define MYQSETTINGS_H

#include <QSettings>

void initQSettings(void);
QString getSetting(QString group,QString Key, QString Default);
void setSetting(QString group,QString Key, QString value);


extern QSettings *settings;




#endif // MYQSETTINGS_H
