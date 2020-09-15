#ifndef SELECTBIBLESDIALOG_H
#define SELECTBIBLESDIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QListWidgetItem>

#include "indexpath.h"

namespace Ui {
class SelectBiblesDialog;
}
typedef struct cellStruct {
    QString text;
    int     type;
} cellStructType;

class SelectBiblesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectBiblesDialog(QWidget *parent = 0);
    ~SelectBiblesDialog();

    void showEvent(QShowEvent *se) ;
    
    cellStructType  cellForRowAtIndexPath(struct indexAndSectionStruct  indexPath);

    int sectionRows[4];

    int getSectionFromRow(int row);
    int getNumberOfItems(int section);
    void reloadData();


private slots:

    //QListWidget *w;
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

    //void on_listWidgetAvailable_clicked(const QModelIndex &index);


    void on_listWidget_clicked(const QModelIndex &index);


private:
    Ui::SelectBiblesDialog *ui;
};

#endif // SELECTBIBLESDIALOG_H
