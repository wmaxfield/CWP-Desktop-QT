#ifndef DIALOGBOOKMARKLISTVIEWER_H
#define DIALOGBOOKMARKLISTVIEWER_H

#include <QDialog>
#include <QList>
#include "chapterversestructclass.h"

#if C0_COMPILE==1
  #define nullptr 0
#endif

namespace Ui {
class DialogBookMarkListViewer;
}

class DialogBookMarkListViewer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBookMarkListViewer(QWidget *parent = nullptr);
    ~DialogBookMarkListViewer();

    void resizeEvent ( QResizeEvent * event ) ;
    void        showEvent(QShowEvent *e);

    QList<ChapterVerseStructType>myResultsArray;
    QStringList namesArray;
    void reloadData();
    QString topic;
    QString bookmark;
private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_pbClose_2_clicked();

    void showContextMenu(const QPoint&);

    void deleteItem();
private:
    Ui::DialogBookMarkListViewer *ui;
    int mainwindowWidth, mainwindowHeight;
};

#endif // DIALOGBOOKMARKLISTVIEWER_H
