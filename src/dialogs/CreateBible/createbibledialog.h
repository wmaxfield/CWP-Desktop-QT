#ifndef CREATEBIBLEDIALOG_H
#define CREATEBIBLEDIALOG_H

#include <QDialog>
#include <QTextEdit>

#if C0_COMPILE==1
  #define nullptr 0
#endif

namespace Ui {
class CreateBibleDialog;
}

class CreateBibleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateBibleDialog(QWidget *parent = nullptr);
    ~CreateBibleDialog();

    void checkEnableCreate();
    void readfileIntoTE(QString filename, QTextEdit *textEdit);
private slots:

    void on_leBibleName_returnPressed();

    void on_pbSelectSchema_clicked();

    void on_pbSelectBibleDirectory_clicked();

    void on_pbCreateBible_clicked();

    void on_leBibleName_textChanged(const QString &arg1);

    void on_pbClose_clicked();

    void on_pbexec_clicked();

    void on_pbReadCopyrightFIle_clicked();

private:
    Ui::CreateBibleDialog *ui;
    QString sqliteFile;

};

#endif // CREATEBIBLEDIALOG_H
