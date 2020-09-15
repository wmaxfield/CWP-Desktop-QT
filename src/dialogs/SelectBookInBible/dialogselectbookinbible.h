#ifndef DIALOGSELECTBOOKINBIBLE_H
#define DIALOGSELECTBOOKINBIBLE_H

#include <QDialog>
#include "indexpath.h"
#include "structs.h"


namespace Ui {
class DialogSelectBookInBible;
}

class DialogSelectBookInBible : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectBookInBible(QWidget *parent = 0);
    ~DialogSelectBookInBible();
    void showEvent(QShowEvent *se);
    void reloadData();
    int getSectionFromRow(int row);
    int getNumberOfItems(int n);
    BibleBookInfoStructType  cellForRowAtIndexPath(struct indexAndSectionStruct  indexPath);

private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_pbClose_clicked();

private:
    Ui::DialogSelectBookInBible *ui;
};

#endif // DIALOGSELECTBOOKINBIBLE_H
