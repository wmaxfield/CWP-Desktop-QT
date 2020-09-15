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
*/#include "createbibledialog.h"
#include "ui_createbibledialog.h"
#include <QFileDialog>
#include <QProcess>
//#include <QtGlobal>
#include "misclibrary.h"
#include <QDebug>

int16_t BibleNameChars;
int16_t BibleDirChars;
int16_t BibleShemaChars;

CreateBibleDialog::CreateBibleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateBibleDialog)
{
    ui->setupUi(this);
    sqliteFile="";
}

void CreateBibleDialog::checkEnableCreate() {
    if (BibleNameChars && BibleDirChars) {
        ui->pbCreateBible->setEnabled(true);
    } else {
        ui->pbCreateBible->setEnabled(false);
    }
}

CreateBibleDialog::~CreateBibleDialog()
{
    delete ui;
}



void CreateBibleDialog::on_leBibleName_returnPressed()
{
    // see if there are characters in the text
    BibleNameChars=ui->leBibleName->text().length();
    checkEnableCreate();
}

//------------------------------------------------------------
//------------------------------------------------------------
void CreateBibleDialog::on_pbSelectSchema_clicked()
{
    QString textFileName;
    // select the schema file
    textFileName=QFileDialog::getOpenFileName(
               this,
               tr("Open SQL File"),
               "/drop-folder/DropBox/NewBible/foo.txt",
               "SQL (*.sql)"
               );

    ui->leSchema->setText(textFileName);
    BibleShemaChars = ui->leSchema->text().length();
    checkEnableCreate();
}

//------------------------------------------------------------
//------------------------------------------------------------
void CreateBibleDialog::on_pbSelectBibleDirectory_clicked()
{
    QString textFileName;
    // select the directory the bible database will be created in
    textFileName=QFileDialog::getExistingDirectory(
               this,
               tr("Open Directory"),
               "/drop-folder/DropBox/NewBible/foo.txt",
               QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
               );
    ui->leBibleDirectory->setText(textFileName);
    BibleDirChars=ui->leBibleDirectory->text().length();
    checkEnableCreate();
}


//------------------------------------------------------------
//------------------------------------------------------------
void CreateBibleDialog::on_pbCreateBible_clicked()
{

    QFile file (ui->leBibleDirectory->text()+"/"+ui->leBibleName->text());
    if(file.exists())
        file.remove();
    QString fname = ui->leBibleDirectory->text()+"/"+ui->leBibleName->text();



    if (sqliteFile.length()==0){
        QFile file3("/usr/bin/sqlite3");
        QFile file2("/opt/local/bin/sqlite3");
        if (!file2.exists()){
            if (file3.exists()){
                sqliteFile = file3.fileName();
            }
        } else {
            sqliteFile = file2.fileName();
        }


        if (sqliteFile.length()==0){
            MsgBox("Was not able to locate sqlite3.  Please select the sqlite3 executable.");
            // select the schema file
            sqliteFile=QFileDialog::getOpenFileName(
                   this,
                   tr("Select sqlite3 executable"),
                   "/usr/bin/foo.txt",
                   "*"
                   );
        }


    }
    // under qprocess you must have the command given to the shell in one
    // variable.  using a list simply confuses qprocess as it hands off to the shell
    QString cmd = sqliteFile+" "+fname+" < "+ui->leSchema->text();

    static QProcess *proc_ovpn;
    if (!proc_ovpn)
        proc_ovpn= new QProcess(this);


    QStringList l ;
    l.clear();
    l << "PATH=$PATH:/opt/local/bin;/usr/bin";
    proc_ovpn->setEnvironment(l);
    l.clear();
    l <<"-c" << cmd;



    proc_ovpn->setProcessChannelMode(QProcess::ForwardedChannels);//QProcess::MergedChannels);

    proc_ovpn->start("/bin/sh",l);

    //proc_ovpn->start(ui->leBibleDirectory->text()+"/tagalog.sh");

    if(!proc_ovpn->waitForStarted()) {//default wait time 30 sec
        //qWarning() << " cannot start process ";
        MsgBox("cannot start Process");
        return;
    }

    QString strng;

    int waitTime = 20000 ; //60 sec
    if (!proc_ovpn->waitForFinished(waitTime)){
        //qWarning() << "timeout .. ";
        strng=proc_ovpn->readAllStandardOutput();
        MsgBox("shell process timeout!");
        MsgBox(strng);
        return;
    }

    //proc_ovpn->waitForFinished();
    strng=proc_ovpn->readAllStandardOutput();
    //return str;
    if(strng.length())
        MsgBox("Executed:"+strng);


    // now populate the copyright
    strng = "\"insert into copyright VALUES(replace('";
    QString s2(ui->teCopyright->toPlainText());

    s2.replace("\r","<br />");
    s2.replace("\n","<br />");
    s2.replace("'","''");
    strng = strng + s2 +"','\\r',char(10)));\"";
    cmd =sqliteFile+" "+fname+" "+strng;
    proc_ovpn->start("sh",QStringList() << "-c" << cmd);

   // proc_ovpn->setProcessChannelMode(QProcess::MergedChannels);

    if(!proc_ovpn->waitForStarted()) {//default wait time 30 sec
        //qWarning() << " cannot start process ";
        MsgBox("cannot start Process");
        return;
    }

     waitTime = 20000 ; //60 sec
    if (!proc_ovpn->waitForFinished(waitTime)){
        //qWarning() << "timeout .. ";
        strng = proc_ovpn->readAllStandardOutput();
        MsgBox("shell process timeout");
        MsgBox(strng);
        return;
    }

    // now populate the title
    strng = "\"insert into Title VALUES(replace('";
    s2=ui->leBibleTitle->text();
    s2.replace("'","''");
    strng = strng + s2 +"','\\r',char(10)));\"";
    cmd =sqliteFile+" "+fname+" "+strng;
    proc_ovpn->start("/bin/sh",QStringList() << "-c" << cmd);

   // proc_ovpn->setProcessChannelMode(QProcess::MergedChannels);

    if(!proc_ovpn->waitForStarted()) {//default wait time 30 sec
        //qWarning() << " cannot start process ";
        MsgBox("cannot start Process");
        return;
    }

     waitTime = 20000 ; //60 sec
    if (!proc_ovpn->waitForFinished(waitTime)){
        //qWarning() << "timeout .. ";
        strng = proc_ovpn->readAllStandardOutput();
        MsgBox("shell process timeout");
        MsgBox(strng);
        return;
    }






    strng = proc_ovpn->readAllStandardOutput();
    if (strng.length())
        MsgBox(strng);
    else
        MsgBox("Finished");


}


//------------------------------------------------------------
//------------------------------------------------------------
void CreateBibleDialog::on_leBibleName_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    BibleNameChars=ui->leBibleName->text().length();
    checkEnableCreate();
}


//------------------------------------------------------------
//------------------------------------------------------------
void CreateBibleDialog::on_pbClose_clicked()
{
    hide();
}

//------------------------------------------------------------
//------------------------------------------------------------
void CreateBibleDialog::on_pbexec_clicked()
{
    static QProcess *proc_ovpn;
    if (!proc_ovpn)
        proc_ovpn= new QProcess(this);


    QStringList l ;
    l.clear();
    l << "PATH=$PATH:/opt/local/bin";
    proc_ovpn->setEnvironment(l);
    l.clear();
    l <<"-c" << ui->leshell->text();

    //QStringList l ;
    proc_ovpn->setProcessChannelMode(QProcess::ForwardedChannels);//QProcess::MergedChannels);

    proc_ovpn->start("/bin/sh",l);

    //proc_ovpn->start(ui->leBibleDirectory->text()+"/tagalog.sh");

    if(!proc_ovpn->waitForStarted()) {//default wait time 30 sec
        //qWarning() << " cannot start process ";
        MsgBox("cannot start Process");
        return;
    }

    QString strng;

    int waitTime = 20000 ; //60 sec
    if (!proc_ovpn->waitForFinished(waitTime)){
        //qWarning() << "timeout .. ";
        strng=proc_ovpn->readAllStandardOutput();
        MsgBox("shell process timeout!");
        MsgBox(strng);
        return;
    }

    //proc_ovpn->waitForFinished();
    strng=proc_ovpn->readAllStandardOutput();
    //return str;
    MsgBox("Executed:"+strng);
    return;

}

//------------------------------------------------------------
//------------------------------------------------------------
void CreateBibleDialog::readfileIntoTE(QString filename, QTextEdit *textEdit){
    //QString filename="test.txt";
    QFile file(filename);
    if(!file.exists()){
        qDebug() << "does not exist "<<filename;
    }else{
        qDebug() << filename<<" reading...";
    }
    QString line;
    textEdit->clear();
    if (file.open(QIODevice::ReadOnly /*| QIODevice::Text*/)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
            if(Instr(line,"\r"))
                line.replace("\r","\n");
            textEdit->setText(textEdit->toPlainText()+line+"\n");
            qDebug() << "line: "<<line;
        }
    }



    file.close();
}

//------------------------------------------------------------
//------------------------------------------------------------
void CreateBibleDialog::on_pbReadCopyrightFIle_clicked()
{
    // read the copyright file into the text edit
    QString textFileName;
    // select the schema file
    textFileName=QFileDialog::getOpenFileName(
               this,
               tr("Open TXT File"),
               "/drop-folder/DropBox/NewBible/foo.txt",
               "TXT (*.txt)"
               );

    readfileIntoTE(textFileName,ui->teCopyright);
}
