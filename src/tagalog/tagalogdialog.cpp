/* ========================================
 *
 * Copyright Wade Maxfield, 2010-2020
 * Commercial license available
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

 * ========================================
*/#include "tagalogdialog.h"
#include "ui_tagalogdialog.h"
#include <QFileDialog>
#include "../editDB/editdb.h"
#include <QMessageBox>
#include <QTextStream>
#include "misclibrary.h"
#include <QTextStream>
#include "../translationtextfile/translationtextfiledialog.h"
#include "../dialogs/CreateBible/createbibledialog.h"
#include "../shared/bibleinfo.h"



static TagalogDialog *tdp;
// open the tagalog window
TagalogDialog *tagalogDialogPtr;

TagalogDialog::TagalogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagalogDialog)
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    ui->setupUi(this);
    ui->pbCancel->setVisible(false);// we don't need this one
    cWebDB=nullptr;
    TargetDB=nullptr;
    timer=nullptr;
    tdp=this;
    here=0;
    StopIt=0;
}

TagalogDialog::~TagalogDialog()
{
    delete ui;
}

void TagalogDialog::on_pbCancel_clicked()
{
    close(); // we are done
}

void TagalogDialog::on_pbSelectFile_clicked()
{
     textFileName=QFileDialog::getOpenFileName(
                this,
                tr("Open Text File"),
                "/drop-folder/DropBox/NewBible/foo.txt",
                "Text (*.txt)"
                );
    ui->leFileName->setText(textFileName);
   checkEnable();
}

void TagalogDialog::on_cbIlonggo_clicked()
{
    if (ui->cbIlonggo->isChecked()){
        language="ilonggo";
        ui->le_TargetDB->setText("/drop-folder/Dropbox/NewBible/ilonggo.bbl");
        ui->cbTagalog->setChecked(false);
        cWebDB=new BibleDatabaseClass(ui->le_cWEBDB->text());
        TargetDB=new BibleDatabaseClass(ui->le_TargetDB->text());
    }

    checkEnable();
}

void TagalogDialog::on_cbTagalog_clicked()
{
    if(ui->cbTagalog->isChecked()){
        language="tagalog";
        ui->le_TargetDB->setText("/drop-folder/Dropbox/NewBible/Tagalog.bbl");
        ui->cbIlonggo->setChecked(false);
        cWebDB=new BibleDatabaseClass(ui->le_cWEBDB->text());
        TargetDB=new BibleDatabaseClass(ui->le_TargetDB->text());
     }

   checkEnable();
}
void TagalogDialog::checkEnable() {
    if ((ui->cbIlonggo->isChecked()||ui->cbTagalog->isChecked())&&(cWebDB&&TargetDB)){
        ui->pbOpenAndProcess->setEnabled(true);

    } else {
        ui->pbOpenAndProcess->setEnabled(false);
    }
}


void TagalogDialog::timerFired() {
    //if(ui->lblBookName->text().length()==0){
        ui->lblBookName->setText(getBookName(CurrentBookNumber));
    //}
    ui->lblChapter->setText(QString::number(CurrentChapter));
    ui->lblVerse->setText(QString::number(CurrentVerse));
    ui->lblBookNumber->setText(QString::number(CurrentBookNumber));
}

void updateui() {
    tdp->timerFired();
}

// process the file
void TagalogDialog::on_pbOpenAndProcess_clicked()
{

    if (!textFileName.length()){
        textFileName=QFileDialog::getOpenFileName(
                    this,
                    tr("Open Text File"),
                    "/drop-folder/Dropbox/NewBible/foo.txt",
                    "Text file (*.txt)"
                    );
        ui->leFileName->setText(textFileName);
        if (!textFileName.length())
            return; // do nothing
    }
  /*  if (!timer){
    timer= new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(timerFired()));
        timer->start(100);on

    }else {
        timer->start();
    }
*/
#if ENABLE_DEBUG
    StopIt=0;
    parse(updateui,textFileName,cWebDB,TargetDB,language,&StopIt);
#endif
    MsgBox("Finished!");
    //timer->stop();
}

//----------------------------------------------
//----------------------------------------------
void TagalogDialog::on_pbTargetDB_clicked()
{
    QString fname="";

        fname=QFileDialog::getOpenFileName(
                    this,
                    tr("Open Target Database File"),
                    "/drop-folder/Dropbox/NewBible/foo.bbl",
                    "Bible file (*.bbl)"
                    );
        ui->le_TargetDB->setText(fname);

    if(fname.length()) {
        // select target database and open
        TargetDB=new BibleDatabaseClass(ui->le_TargetDB->text());
        if (!TargetDB){
            MsgBox("Error opening Target Database!");
        }
    }
    checkEnable();
}

//----------------------------------------------
//----------------------------------------------
void TagalogDialog::on_pbWebDB_clicked()
{
    QString fname="";

        fname=QFileDialog::getOpenFileName(
                    this,
                    tr("Open cWEB File"),
                    "/drop-folder/Dropbox/NewBible/foo.bbl",
                    "Bible file (*.bbl)"
                    );
        ui->le_cWEBDB->setText(fname);

    if(fname.length()) {
        // select cWEB database and open
        cWebDB=new BibleDatabaseClass(ui->le_cWEBDB->text());
        if (!cWebDB){
            MsgBox("Error Opening cWEB Database!");
        }
    }
    checkEnable();
}

//----------------------------------------------
//----------------------------------------------
void TagalogDialog::on_pbOK_clicked()
{
    close();

}
static QFile   f;
//---------------------------------------------
// open the command file and parse
// command:info
// target: /drop-folder/Dropbox/NewBible/tagalog.bbl
// compare: /drop-folder/Dropbox/NewBible/cWeb.BBL
// language: tagalog
// text:/drop-folder/Dropbox/NewBible/john.txt
// text:/drop-folder/Dropbox/NewBible/matthew.txt
// ....
// end:end
//---------------------------------------------
void TagalogDialog::on_pbCmdFile_clicked()
{
    if (here)
        return;
    here=1;
    //
    QStringList l;

        textFileName=QFileDialog::getOpenFileName(
                    this,
                    tr("Open Text File"),
                    "/drop-folder/Dropbox/NewBible/foo.txt",
                    "Text file (*.txt)"
                    );
        ui->le_CmdFIle->setText(textFileName);
        if (!textFileName.length()) {
            here = 0;
            return; // do nothing
        }

        qApp->processEvents(QEventLoop::AllEvents,50);
    processCmdFile();
}
//---------------------------------------------
//---------------------------------------------
void TagalogDialog::processCmdFile() {
    //
    QStringList l;
    QString Title="";


    // open the file
    f.setFileName(textFileName);
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr,"error opening file",f.errorString());
        here=0;
        return ;
    }
    // get the text stream
   QTextStream in(&f);
   QString line ;
   QString Cmd;
   QString info;
      //
      do {
          line= in.readLine().trimmed();
          // if the line is empty, get the next one
          if(line.length()==0)
              continue;
          ui->label_cmds->setText(line);

          if (line[0]=='/' && line[1]=='/' )
              continue;

          l = line.split(":");

          if(l.size()>1) {
              // get the command
              Cmd = l[0].trimmed();
              if (StringsAreEqual(Cmd,"target")){
                  ui->le_TargetDB->setText(l[1].trimmed());
                  if (TargetDB)
                      delete TargetDB;
                  TargetDB=new BibleDatabaseClass(ui->le_TargetDB->text());
                  continue; // next line
              }
              if(StringsAreEqual(Cmd,"compare")){
                 if (cWebDB)
                     delete cWebDB;
                  ui->le_cWEBDB->setText(l[1].trimmed());
                  cWebDB=new BibleDatabaseClass(ui->le_cWEBDB->text());
                  continue;
              }
              if(StringsAreEqual(Cmd,"language")){
                  info=l[1].trimmed();
                  if(StringsAreEqual(info,"tagalog")){
                      ui->cbTagalog->setChecked(true);
                      ui->cbIlonggo->setChecked(false);
                      language=info;
                  }
                  if(StringsAreEqual(info,"ilonggo")){
                      ui->cbIlonggo->setChecked(true);
                      ui->cbTagalog->setChecked(false);
                      language=info;
                  }
                  continue; // next line
              }

              if(StringsAreEqual(Cmd,"title")){
                 Title = l[1].trimmed();
                 continue;
              }

              if(StringsAreEqual(Cmd,"text")){
                  textFileName=l[1].trimmed();
                  ui->leFileName->setText(textFileName);
                  qApp->processEvents(QEventLoop::AllEvents,50);
                  // go parse the file
#if ENABLE_DEBUG
                  if(parse(updateui,textFileName,cWebDB,TargetDB,language,&StopIt)<0)
#endif
                      break;//finished, error
              }
              if(StringsAreEqual(Cmd,"end")){
                  //MsgBox("End of File Reached");
                  break; // exit the do loop, we are finished
              }
          }
        if (StopIt)
            break;
       } while( !in.atEnd());

   cWebDB->setTitle(Title);
   ui->lblChapter->setText("-");

   f.close();
   here=0;
   updateui(); // show last verse, etc.
   StopIt=0;
   MsgBox("finished");
}

//---------------------------------------------
//---------------------------------------------
void TagalogDialog::on_pbCreateChapterFiles_clicked()
{
    static TranslationTextFileDialog *ttd;
        if(!ttd)
            ttd = new TranslationTextFileDialog();

        ttd->setWindowFlags(Qt::WindowStaysOnTopHint);
        ttd->showMe=this;
        ttd->show();

}

//---------------------------------------------
//---------------------------------------------
void TagalogDialog::on_pbReRun_clicked()
{

    // if use has not yet selected command file, bail
    if(ui->le_CmdFIle->text().length()==0)
            return;

    textFileName = ui->le_CmdFIle->text();

    if (here)
        return;
    here=1;

    // allow ui refresh
    qApp->processEvents(QEventLoop::AllEvents,50);

    processCmdFile();
}

//------------------------------------------------------------
//------------------------------------------------------------
void TagalogDialog::on_pushButton_clicked()
{
    // call the create bible dialog
   static CreateBibleDialog *cbd;

   if (!cbd){
        cbd = new CreateBibleDialog(this);
   }

   cbd->show();
}

//------------------------------------------------------------
//------------------------------------------------------------
void TagalogDialog::on_pbStop_clicked()
{
    StopIt=1;
}
