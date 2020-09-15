#ifndef BOOKMARKHANDLER_H
#define BOOKMARKHANDLER_H

#include <QMainWindow>
#include <QStringList>
#include <QLabel>
#include <QPushButton>
#include "wheelwidget.h"

#include "BookMarkDB.h"

#include "timerclass.h"
#include <QGridLayout>
#ifdef Q_OS_LINUX
#include <inttypes.h>
#endif

namespace Ui {
class BookMarkHandler;
}

class BookMarkHandler : public QMainWindow //public QDialog
{
    Q_OBJECT
    
public:
    explicit BookMarkHandler(QWidget *parent = 0);
    ~BookMarkHandler();
    void showEvent(QShowEvent *e);
    void            makeBookMarkWheel();
    void            doUpdate();

    QLabel          *labelVerse;
    QPushButton *buttonAddBookmark;
    QWidget         *mBookMarks;

    StringWheelWidget *m_wheel1;

    QGridLayout     *grid;

    uint16_t        isTextBook;
    QString         bookNumber;
    QString         chapter;
    QString         verse;
    int             selectedRow;
    int             refreshPicker;

    QStringList     pickerViewArray;

    TimerClass      *addBookMarkTimer;

public slots:

    void stop(int index);
    void topicButtonClicked();
    void pbOK_clicked();
    void onAddBookMarkClicked();
private:
    Ui::BookMarkHandler *ui;
    int             wheelRow;
    bool            touch;
    void on_actionAlways_on_Top_triggered(bool checked);
};

extern BookMarkHandler *bmh;

#endif // BOOKMARKHANDLER_H
