#ifndef DIALOGPREFERENCES_H
#define DIALOGPREFERENCES_H

#include <QDialog>

namespace Ui {
class DialogPreferences;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
class DialogPreferences : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogPreferences(QWidget *parent = 0);
    ~DialogPreferences();
    void updateCWPDirectory();// update after user change
private slots:
    void on_pbOK_clicked();

    void on_cbEnableStrongs_clicked();

    void on_cbBlendStrongs_clicked();

    void on_cbThisBook_clicked();

    void on_SliderRed_valueChanged(int value);

    void on_SliderGreen_valueChanged(int value);

    void on_SliderBlue_valueChanged(int value);

    void on_cbEnglish_clicked();

    void on_sliderFontSize_valueChanged(int value);

    void on_cbOT_clicked();

    void on_cbNT_clicked();

    void on_cbDebugHTML_clicked();

    void on_cbSQLExec_clicked();

    void on_cbEditVerses_clicked();

    void on_pbChangeCWPDirectory_clicked();

    void on_pbShowCWPDirectory_2_clicked();

    void on_cbShowBookmark_clicked();

    void on_cbShowCopyIt_clicked();

private:
    Ui::DialogPreferences *ui;
};

#endif // DIALOGPREFERENCES_H
