#ifndef COPYDIALOG_H
#define COPYDIALOG_H

#include <QDialog>
#include "structs.h"

namespace Ui {
class CopyDialog;
}

class CopyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CopyDialog(QWidget *parent = 0);
    explicit CopyDialog(int book, int chapter, int verse,int BibleNumber,QString BibleName,QWidget *parent=0);
    ~CopyDialog();

    void setParams(int book, int chapter, int verse,int BibleNumber,QString BibleName);
    int _Book;
    int _StartChapter,_EndChapter;
    int _StartVerse,_EndVerse;
    QString _BibleName;
    int  _BibleNumber;

    void setupDialog(); // do the work to populate the dialog fields
    void PopulateTranslationsCB();
    void PopulateStartChapterCB();
    void PopulateEndChapterCB();
    void PopulateEndVerseCB();
    void PopulateStartVerseCB();
    void BuildVerses();

private slots:

    void on_pbCopy_clicked();

    void on_cbTranslation_currentIndexChanged(int index);


    void on_cbStartChapter_currentIndexChanged(int index);

    void on_cbStartVerse_currentIndexChanged(int index);

    void on_cbEndChapter_currentIndexChanged(int index);

    void on_cbEndVerse_currentIndexChanged(int index);

    void on_pbClose_clicked();

    void on_cbBibleBook_currentIndexChanged(int index);
    void PopulateBooksCB();
    void on_cbAddVerse_clicked();

    void on_cbAddBookChapter_clicked();

private:
    Ui::CopyDialog *ui;
    int changedEndChapterManually;
    BookChapterVerseStructType bcv;
    int dontUpdate;
    int _AddVerse;
    int _AddBookChapter;
};

#endif // COPYDIALOG_H
