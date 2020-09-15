#ifndef TRANSLATIONTEXTFILEDIALOG_H
#define TRANSLATIONTEXTFILEDIALOG_H

#include <QDialog>
#include "BibleDatabaseClass/bibledatabaseclass.h"
#include "../editDB/editdb.h"
#include <QListWidget>

namespace Ui {
class TranslationTextFileDialog;
}

class TranslationTextFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TranslationTextFileDialog(QWidget *parent = 0);
    ~TranslationTextFileDialog();
    BibleDatabaseClass *cWebDB;

    QString textFileName;
    dbWordType *getEnglishVerseString(int BookNumber, int chapter, int verse ,int *wordCount);
    void showEvent(QShowEvent *e);
    //void        resizeEvent ( QResizeEvent * event );


    QStringList     pickerViewArray;
    QDialog *showMe;
private slots:
    void on_pbSelectFile_clicked();

    void on_pbOpenAndProcess_clicked();

    void on_pbWebDB_clicked();

    void on_pushButton_clicked();

    void on_listWidget_activated(const QModelIndex &index);
    void on_listWidget_clicked(const QModelIndex &index);

    void on_cbTaglish_clicked();

    void on_cbTagalogIlongo_clicked();

private:
    Ui::TranslationTextFileDialog *ui;
};

#endif // TRANSLATIONTEXTFILEDIALOG_H
