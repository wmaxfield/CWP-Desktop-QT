#ifndef DIALOGHISTORY_H
#define DIALOGHISTORY_H

#include <QDialog>
class DialogHistory;

#if C0_COMPILE==1
  #define nullptr 0
#endif

namespace Ui {
class DialogHistory;
}
extern DialogHistory *historyWindowPointer;

class DialogHistory : public QDialog
{
    Q_OBJECT

public:
    explicit DialogHistory(QWidget *parent = nullptr);
    ~DialogHistory();
    void showEvent(QShowEvent *e);
    // reject() in a Dialog is the closeEvent() equivalent for the mainWindow
    void reject()    { historyWindowPointer=nullptr;  QDialog::reject();  }

    void resizeEvent(QResizeEvent *event);
private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_pbClose_2_clicked();

private:
    Ui::DialogHistory *ui;
    int mainwindowWidth, mainwindowHeight;
};



#endif // DIALOGHISTORY_H
