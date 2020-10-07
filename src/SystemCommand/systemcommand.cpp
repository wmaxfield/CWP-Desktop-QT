#include "systemcommand.h"
#include <QProcess>
#include "misclibrary.h"

//------------------------------------------------------
//------------------------------------------------------
SystemCommandClass::SystemCommandClass(QObject *parent) : QObject(parent)
{
}

//------------------------------------------------------
//------------------------------------------------------
bool SystemCommandClass::run(QString command,bool waitForFinish)
{
#ifdef Q_OS_MAC
    return RunMac(command,waitForFinish);

#endif

#ifdef Q_OS_LINUX
    return RunLinux(command, waitForFinish);

#endif
#ifdef Q_OS_WIN
    Q_UNUSED(command)
    Q_UNUSED(waitForFinish)
  return 0;
#endif
}
//------------------------------------------------------
//------------------------------------------------------
bool SystemCommandClass::RunLinux(QString Command,bool waitForFinish){
#ifdef Q_OS_LINUX
    // under qprocess you must have the command given to the shell in one
    // variable.  using a list simply confuses qprocess as it hands off to the shell
    // ie:QString cmd = sqliteFile+" "+fname+" < "+ui->leSchema->text();

    QString cmd;

    cmd = Command;

    static QProcess *proc_ovpn;
    // if we are racing, kill the numbers.
    if (proc_ovpn)
       if (proc_ovpn->state()==QProcess::Running)
          return false;

    proc_ovpn= new QProcess(this);

    QStringList l ;
    l.clear();

    l << "PATH=$PATH:/opt/local/bin;/usr/bin;/bin;/usr/sbin";
    proc_ovpn->setEnvironment(l);
    l.clear();

    l <<"-c" << cmd;


    proc_ovpn->setProcessChannelMode(QProcess::ForwardedChannels);
    proc_ovpn->start("/bin/sh",l);

    if(!proc_ovpn->waitForStarted()) {//default wait time 30 //date [-jnu] [[[[[cc]yy]mm]dd]HH]MM[.ss]
        // -j do not try to set the date

        //qWarning() << " cannot start process ";
        MsgBox(tr("Cannot Perform Command"));
        return false;
    }

    if (waitForFinish){
        QString strng;

        int waitTime = 20000 ; //20 sec
        if (!proc_ovpn->waitForFinished(waitTime)){
            //qWarning() << "timeout .. ";
            strng=proc_ovpn->readAllStandardOutput();
            MsgBox(tr("Command timeout!"));
            MsgBox(strng);
            return false;
        }
    }

    //strng=proc_ovpn->readAllStandardOutput();
    //return str;
    //if(strng.length())
    //    MessageBoxOK("Executed:"+strng);
#else
    Q_UNUSED(Command)
    Q_UNUSED(waitForFinish)
#endif
    return false;
}
//------------------------------------------------------
// return true if execution attempted, false if not.
//------------------------------------------------------
bool SystemCommandClass::RunMac(QString Command,bool waitForFinish)
{
#ifdef Q_OS_MAC
    // under qprocess you must have the command given to the shell in one
    // variable.  using a list simply confuses qprocess as it hands off to the shell
    // ie:QString cmd = sqliteFile+" "+fname+" < "+ui->leSchema->text();

    QString cmd;

    cmd = Command;

    static QProcess *proc_ovpn;

    proc_ovpn= new QProcess(this);
     QStringList l ;
    l.clear();

    l << "PATH=$PATH:/opt/local/bin;/usr/bin;/bin";
    proc_ovpn->setEnvironment(l);
    l.clear();

    l <<"-c" << cmd;


    proc_ovpn->setProcessChannelMode(QProcess::ForwardedChannels);
    proc_ovpn->start("/bin/sh",l);

    if(!proc_ovpn->waitForStarted()) {//default wait time 30 sec
        //qWarning() << " cannot start process ";
        MsgBox(tr("Command Failed"));
        //MessageBoxOK(tr("Command"),tr("Cannot Perform Command"));
        return false;
    }

    if (waitForFinish){
        QString strng;

        int waitTime = 20000 ; //20 sec
        if (!proc_ovpn->waitForFinished(waitTime)){
            //qWarning() << "timeout .. ";
            strng=proc_ovpn->readAllStandardOutput();
            MsgBox(tr("Command Failed"));
            return false;
        }

    }
 #else
    Q_UNUSED(Command)
    Q_UNUSED(waitForFinish)
#endif

    return true;
}
