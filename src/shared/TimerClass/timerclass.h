#ifndef TIMERCLASS_H
#define TIMERCLASS_H
#include <QtCore>
#include <QTimer>

class TimerClass : public QObject
{
    Q_OBJECT

public:
    TimerClass();
    TimerClass(int delay);
    TimerClass(int delay,int singleShot,void *fn);

    void setDelay(int delay);
    void start(int delay);
    void SingleShot(int delay);
    void setFunction(void *fn);
    void stop(); // stop running timer

    QTimer timer;
    QString userString;

private slots:
    void timerFired();

private:
    int _delay;
    void (*fn)();
    void (*fns)(QString s);
};

#endif // TIMERCLASS_H
