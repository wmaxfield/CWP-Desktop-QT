#ifndef DIALOGADDTOPIC_H
#define DIALOGADDTOPIC_H

#include <QDialog>

namespace Ui {
class DialogAddTopic;
}

class DialogAddTopic : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAddTopic(QWidget *parent = 0);
    ~DialogAddTopic();
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_lineEditTopic_editingFinished();

private:
    Ui::DialogAddTopic *ui;
};

#endif // DIALOGADDTOPIC_H
