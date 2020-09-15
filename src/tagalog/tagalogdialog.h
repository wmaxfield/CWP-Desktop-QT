#ifndef TAGALOGDIALOG_H
#define TAGALOGDIALOG_H

#include "ui_mainwindow.h"
#include <QDialog>
#include <QFile>
#include <QTimer>
#include "BibleDatabaseClass/bibledatabaseclass.h"

namespace Ui {
class TagalogDialog;
}

class TagalogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagalogDialog(QWidget *parent = 0);
    ~TagalogDialog();

    QString textFileName;  // file name we are working on
     QTimer *timer ;
    QString language;
    int here;
    int StopIt;

    BibleDatabaseClass *cWebDB;
    BibleDatabaseClass *TargetDB;
    void checkEnable();
    void processCmdFile();

//public slots:
    void timerFired();
private slots:
    void on_pbCancel_clicked();

    void on_pbSelectFile_clicked();

    void on_cbIlonggo_clicked();

    void on_cbTagalog_clicked();

    void on_pbOpenAndProcess_clicked();

    void on_pbTargetDB_clicked();

    void on_pbWebDB_clicked();

    void on_pbOK_clicked();

    void on_pbCmdFile_clicked();

    void on_pbCreateChapterFiles_clicked();

    void on_pbReRun_clicked();

    void on_pushButton_clicked();

    void on_pbStop_clicked();

private:
    Ui::TagalogDialog *ui;
};
extern // open the tagalog window
TagalogDialog *tagalogDialogPtr;
#endif // TAGALOGDIALOG_H
