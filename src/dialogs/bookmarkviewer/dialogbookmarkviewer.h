#ifndef DIALOGBOOKMARKVIEWER_H
#define DIALOGBOOKMARKVIEWER_H

#include <QDialog>

namespace Ui {
class DialogBookMarkViewer;
}

class DialogBookMarkViewer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBookMarkViewer(QWidget *parent = 0);
    ~DialogBookMarkViewer();

    void showEvent(QShowEvent *e);
    void        resizeEvent ( QResizeEvent * event );

    QStringList     pickerViewArray;
    void reject();

private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_pbClose_2_clicked();

private:
    Ui::DialogBookMarkViewer *ui;

    int mainwindowWidth, mainwindowHeight;

};
extern DialogBookMarkViewer   * bmvPtr ;
#endif // DIALOGBOOKMARKVIEWER_H
