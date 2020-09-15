#ifndef NOTESWINDOW_H
#define NOTESWINDOW_H
///
//  NotesViewController.h
//  Crossword-Student
//
//  Created by WADE MAXFIELD on 11/17/10.
//  Copyright 2010 Wade Maxfield, the CrosswordProject. All rights reserved.
//

#include <QMainWindow>

namespace Ui {
class NotesWindow;
}

class NotesWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit NotesWindow(QWidget *parent = 0);
    ~NotesWindow();
    void saveNote();
    void showEvent(QShowEvent *e);
    void resizeEvent ( QResizeEvent * event ) ;
    int wWidth,wHeight;
    void setPlainText(QString Text);
    int chapter ;
    int bookNumber ;
    int verse ;
    int noteIndex ;
    int dontReloadText;

    void addBookChapterVerse(int book,int chapter, int verse);
private slots:
    void on_pbItalic_clicked();

    void on_pbOK_clicked();

    void on_pbNoItalic_clicked();

    void on_pbCancel_clicked();

    void on_pbBoldOn_clicked();

    void on_pbBoldOff_clicked();

    void on_pbUnderlineOn_clicked();

    void on_pbUnderlineOff_clicked();

    void on_pbVerse_clicked();

private:
    Ui::NotesWindow *ui;
};

#if 0
- (IBAction)              addUnderlineButtonPushed:(id)sender;
- (IBAction)              addUnUnderlineButtonPushed:(id)sender;
//-------------------------------------------


-(void) saveNote; // save the note

#define NotesPopOverWidth 650.0
#define NotesPopOverHeight 300.0

@end
#endif

#endif // NOTESWINDOW_H
