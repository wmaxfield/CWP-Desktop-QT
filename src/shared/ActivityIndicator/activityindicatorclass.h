#ifndef ACTIVITYINDICATORCLASS_H
#define ACTIVITYINDICATORCLASS_H

#include "../shared/busyIndicator/busyindicator.h"
#include "qmainwindow.h"

class ActivityIndicatorClass
{
public:
    ActivityIndicatorClass();
    ActivityIndicatorClass(QMainWindow *mw);

    BusyIndicator *ind;
    int hidesWhenStopped;
    int isAnimating(); // when true, activity indicator is running
    void animate() {
        ind->start();
        _isAnimating=1;
        ind->show();
    }
    void stopAnimation() {
        ind->stop();
        _isAnimating=0;
        if (hidesWhenStopped) {
            ind->hide();
        }
    }

private:
    int _isAnimating;
};

#endif // ACTIVITYINDICATORCLASS_H
