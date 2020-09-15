#ifndef DIALOGSELECTCHAPTERINBOOK_H
#define DIALOGSELECTCHAPTERINBOOK_H

#include <QDialog>
#include <QString>

namespace Ui {
class DialogSelectChapterInBook;
}

class DialogSelectChapterInBook : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectChapterInBook(QWidget *parent = 0);
    ~DialogSelectChapterInBook();
    void showEvent(QShowEvent *);
        int book;
        QString bookName;

private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_pbClose_clicked();

private:
    Ui::DialogSelectChapterInBook *ui;
};

#endif // DIALOGSELECTCHAPTERINBOOK_H
