#ifndef RUNSHELLCOMMAND_H
#define RUNSHELLCOMMAND_H

#include <QObject>

class SystemCommandClass : public QObject
{
    Q_OBJECT
public:
    explicit SystemCommandClass(QObject *parent = nullptr);
    bool run(QString command,bool waitForFinish=false);
signals:

public slots:

private:
    bool RunMac(QString Command, bool waitForFinish);
    bool RunLinux(QString Command,bool waitForFinish);

};

#endif // RUNSHELLCOMMAND_H
