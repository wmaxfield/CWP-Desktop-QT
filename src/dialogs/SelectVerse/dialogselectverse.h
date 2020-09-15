#ifndef DIALOGSELECTVERSE_H
#define DIALOGSELECTVERSE_H

#include <QDialog>
#include "noteswindow.h"
#include "bibleinfo.h"

namespace Ui {
class DialogSelectVerse;
}

class DialogSelectVerse : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectVerse(QWidget *parent = 0);
    ~DialogSelectVerse();

    NotesWindow *delegate;//)(int book,int chapter,int verse);

    int book;
    int chapter;
    int verse;

private slots:

    void on_pbCancel_clicked();

    void on_pbOK_clicked();

    void on_comboBook_currentIndexChanged(int index);

    void on_comboVerse_currentIndexChanged(int index);

    void on_comboChapter_currentIndexChanged(int index);

private:
    Ui::DialogSelectVerse *ui;
    BookChapterVerseStructType bcv;

};

#endif // DIALOGSELECTVERSE_H
