#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>
#include <QtGui>
#include <QGraphicsView>

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAbout(QWidget *parent = 0);
    ~DialogAbout();
    QGraphicsScene *scene;

    void setGraphicsView(QGraphicsView *view);
    void setImage(QString imageName);

private slots:
    void on_pbOK_clicked();

private:
    Ui::DialogAbout *ui;
    QGraphicsView *_view;
};

#endif // DIALOGABOUT_H
