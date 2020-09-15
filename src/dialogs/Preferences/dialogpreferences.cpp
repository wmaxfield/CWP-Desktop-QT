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
*/#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"
#include "PreferencesLibrary.h"
#include "preferencesmodule.h"
#include "mainwindow.h"
#include "bibledictionarydialog.h"
#include "misclibrary.h"
#include <QFileDialog>

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogPreferences::DialogPreferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPreferences)
{
    ui->setupUi(this);

    //-----------------------------------------------------------
    // Color of Hilighted Font
    //-----------------------------------------------------------

    ui->label_Red->setStyleSheet("QLabel { color: red;}");
    ui->label_Green->setStyleSheet("QLabel { color: green;}");
    ui->label_Blue->setStyleSheet("QLabel { color: blue;}");

    ui->SliderRed->setMaximum(255);
    ui->SliderBlue->setMaximum(255);
    ui->SliderGreen->setMaximum(255);

    ui->SliderRed->setValue(Preferences->getRed());
    ui->SliderBlue->setValue(Preferences->getBlue());
    ui->SliderGreen->setValue(Preferences->getGreen());

    ui->SliderBlue->setTracking(NO);// only emit signal at end
    ui->SliderGreen->setTracking(NO);
    ui->SliderRed->setTracking(NO);

    //-----------------------------------------------------------
    // Dictionaries
    //-----------------------------------------------------------

    ui->cbBlendStrongs->setChecked(Preferences->getBlendStrongs());
    ui->cbEnableStrongs->setChecked(Preferences->getEnableDisplayStrongs());
    ui->cbEnglish->setChecked(Preferences->getShowEnglishDictionaryWindow());

    //-----------------------------------------------------------
    // font size
    //-----------------------------------------------------------

    ui->sliderFontSize->setMaximum(100);
    ui->sliderFontSize->setTracking(YES);
    ui->sliderFontSize->setValue(Preferences->getFontSize());

    //-----------------------------------------------------------
    // clean up memory after close
    //-----------------------------------------------------------

    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close
    //-----------------------------------------------------------
    // strong's Checkboxes
    //-----------------------------------------------------------

    if (Preferences->getEnableDisplayStrongs()){
        ui->cbEnableStrongs->setChecked(YES);
    } else {
        ui->cbEnableStrongs->setChecked(NO);
    }

    //-----------------------------------------------------------
    // search Checkboxes
    //-----------------------------------------------------------
    if (Preferences->getSearchThisBookOnly()) {
        ui->cbThisBook->setChecked(YES);
        ui->cbNT->setChecked(NO);
        ui->cbOT->setChecked(NO);
    } else {
        ui->cbThisBook->setChecked(NO);
        if (!Preferences->getSearchNT() && Preferences->getSearchOT()) {
            ui->cbNT->setChecked(YES);
            ui->cbOT->setChecked(YES);

        } else {
            ui->cbNT->setChecked(Preferences->getSearchNT());
            ui->cbOT->setChecked(Preferences->getSearchOT());
        }
    }

//-----------------------------------------------------------
// DEBUG FORMATTING START
//-----------------------------------------------------------
extern int enable_debug; // set in MainWindow.cpp

    if (enable_debug){
        ui->gbDebug->setVisible(YES);
        int r = Preferences->getDebug();
        if (r & DEBUG_HTML) {
            ui->cbDebugHTML->setChecked(YES);
        } else {
            ui->cbDebugHTML->setChecked(NO);
        }

        if (r & DEBUG_MENUS)
            ui->cbEnableMenus->setChecked(YES);
         else
            ui->cbEnableMenus->setChecked(NO);
    } else {

        // hide the debug groupbox
        ui->gbDebug->setVisible(NO);
        // resize the window and move the button so it
        // looks natural.
        int x= ui->pbOK->pos().rx();
        int y = ui->pbOK->pos().ry();
        int offset = ui->gbDebug->height();
        ui->pbOK->move(x,y-offset+2);
        setFixedHeight(height()-offset+2);// change window size
    }

    setFixedSize(size());
//-----------------------------------------------------------
// DEBUG FORMATTING END
//-----------------------------------------------------------


    //-----------------------------------------------------------
    // CWP directory
    //-----------------------------------------------------------

      QString str = Preferences->getCWPDirectory();
      bool cwpDirSet=true;

      if (str.length()==0){
          cwpDirSet=false;
           // get the documents directory (i.e. required on mac)
          str = getFullPathOfUserDocumentsCWPDirectory();
       }

      if (!cwpDirSet){
          // if it wasn't set before, set it now
          Preferences->setCWPDirectory(str);
      }
      ui->lb_CWP_Directory->setText(str);
      ui->lb_CWP_Directory->setToolTip(str);

    #ifdef Q_OS_WIN
       QRect r=  ui->label_Green->geometry();
       r.setWidth(r.width()+4);
       ui->label_Green->setGeometry(r);
    #endif



   //-----------------------------------------------------------
   // set current state of Bookmark Tag
   //-----------------------------------------------------------
   ui->cbShowBookmark->setChecked(Preferences->getShowBookMarkTag());
   //-----------------------------------------------------------
   // set current state of Copy Tag
   //-----------------------------------------------------------
   ui->cbShowCopyIt->setChecked(Preferences->getShowCopyTag());

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogPreferences::updateCWPDirectory() {

    QString dn = Preferences->getCWPDirectory();

    if (dn.length()==0){
        dn = getFullPathOfAppBundleCWPBibleDirectory();
    }
    ui->lb_CWP_Directory->setText(dn);

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogPreferences::~DialogPreferences()
{
    delete ui;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogPreferences::on_pbOK_clicked()
{
    close();
    delete this;// auto close not enabled
}

//------------------------------------------------------------
//------------------------------------------------------------
void DialogPreferences::on_cbEnableStrongs_clicked()
{
    if (ui->cbEnableStrongs->isChecked()) {
        Preferences->setEnableDisplayStrongs(YES);
        globalSTI.ShowStrongs=YES;
    } else {
        Preferences->setEnableDisplayStrongs(NO);
        globalSTI.ShowStrongs=NO;
    }

    fvcp->LoadWebPage();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogPreferences::on_cbBlendStrongs_clicked()
{
    if (ui->cbBlendStrongs->isChecked()){
        Preferences->setBlendStrongs(YES);
        globalSTI.BlendStrongsIntoWord = YES;
    } else {
        Preferences->setBlendStrongs(NO);
        globalSTI.BlendStrongsIntoWord = NO;
    }

    fvcp->LoadWebPage();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogPreferences::on_cbThisBook_clicked()
{
    // if checked, the other two need to be unchecked
    if (ui->cbThisBook->isChecked()) {
        ui->cbNT->setChecked(NO);
        Preferences->setSearchNT(NO);
        ui->cbOT->setChecked(NO);
        Preferences->setSearchOT(NO);
        Preferences->setSearchThisBookOnly(YES);
    } else {
        ui->cbNT->setChecked(YES);
        Preferences->setSearchNT(YES);
        ui->cbOT->setChecked(YES);
        Preferences->setSearchOT(YES);
        Preferences->setSearchThisBookOnly(NO);
    }
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogPreferences::on_SliderRed_valueChanged(int value)
{
//
    Preferences->setRed(value);
    fvcp->LoadWebPage();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogPreferences::on_SliderGreen_valueChanged(int value)
{
    Preferences->setGreen(value);
    fvcp->LoadWebPage();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogPreferences::on_SliderBlue_valueChanged(int value)
{
    Preferences->setBlue(value);
    fvcp->LoadWebPage();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogPreferences::on_cbEnglish_clicked()
{

    if (ui->cbEnglish->isChecked()){
        Preferences->setEnableEnglishDictionary(YES);
        globalSTI.enableEnglishDictionary = YES;
    } else {
        Preferences->setEnableEnglishDictionary(NO);
        globalSTI.enableEnglishDictionary = NO;
    }

    fvcp->LoadWebPage();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogPreferences::on_sliderFontSize_valueChanged(int value)
{
    Preferences->setFontSize(value);

    fvcp->LoadWebPage();

    if (bibleDictionaryWindowPtr) {
        bibleDictionaryWindowPtr->ReloadWebPage();
    }
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogPreferences::on_cbOT_clicked()
{
    if (ui->cbOT->isChecked()) {
        ui->cbThisBook->setChecked(NO);
        Preferences->setSearchThisBookOnly(NO);
        Preferences->setSearchOT(YES);
    }  else {
        Preferences->setSearchOT(NO);
    }
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogPreferences::on_cbNT_clicked()
{
    if (ui->cbNT->isChecked()) {
        ui->cbThisBook->setChecked(NO);
        Preferences->setSearchThisBookOnly(NO);
        Preferences->setSearchNT(YES);
    }  else {
        Preferences->setSearchNT(NO);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void DialogPreferences::on_cbDebugHTML_clicked()
{
    if (ui->cbDebugHTML->isChecked()) {
        Preferences->setDebug(Preferences->getDebug() | DEBUG_HTML);// show debug html
    } else {
        Preferences->setDebug(Preferences->getDebug() & ~DEBUG_HTML);// don't show debug html
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void DialogPreferences::on_cbSQLExec_clicked()
{
    extern int16_t gShowSQLExecError;
    if (ui->cbSQLExec->isChecked()) {
        gShowSQLExecError=YES;
        //Preferences->setDebug(Preferences->getDebug() | DEBUG_HTML);// show debug html
    } else {
        gShowSQLExecError=NO;
      //  Preferences->setDebug(Preferences->getDebug() & ~DEBUG_HTML);// don't show debug html
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void DialogPreferences::on_cbShowBookmark_clicked()
{
    if (ui->cbShowBookmark->isChecked()){
        Preferences->setShowBookMarkTag(YES);
        globalSTI.showBookMarkTag=YES;
    }else {
        Preferences->setShowBookMarkTag(NO);
        globalSTI.showBookMarkTag=NO;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void DialogPreferences::on_cbShowCopyIt_clicked()
{
    if (ui->cbShowCopyIt->isChecked()){
        Preferences->setShowCopyTag(YES);
        globalSTI.showCopyTag=YES;
    } else {
        Preferences->setShowCopyTag(NO);
        globalSTI.showCopyTag=NO;

    }
}



//------------------------------------------------------------
//------------------------------------------------------------
void DialogPreferences::on_cbEditVerses_clicked()
{
    if (ui->cbEditVerses->isChecked()){
        globalSTI.EnableVerseEditing=YES;
    } else {
        globalSTI.EnableVerseEditing=NO;
    }
    Preferences->setEnableVerseEdit(globalSTI.EnableVerseEditing);
}
//------------------------------------------------------------
//------------------------------------------------------------
void DialogPreferences::on_pbChangeCWPDirectory_clicked()
{
    QString textFileName;
    // select the directory the bible database will be created in
    textFileName=QFileDialog::getExistingDirectory(
               this,
               tr("Select Main CWP Directory"),
             //--> note the foo.txt  "/drop-folder/DropBox/CWP-Public/CWP/foo.txt",
                Preferences->getCWPDirectory(),
               QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
               );
    // for debug, allow dialog to be dismissed
    qApp->processEvents(QEventLoop::AllEvents,50);

    if (textFileName.length()){
        Preferences->setCWPDirectory(textFileName);
        MsgBox("You must close CWP and reopen for this change to take effect.");
        // show the user it happened
        updateCWPDirectory();
        ui->lb_CWP_Directory->setToolTip(textFileName);
    } else {
        //Preferences->setCWPDirectory("");
        // do nothing
    }

}
//------------------------------------------------------------
// "Reveal in Finder" or "Show in Explorer"
//------------------------------------------------------------
void DialogPreferences::on_pbShowCWPDirectory_2_clicked()
{
    QDesktopServices::openUrl( QUrl::fromLocalFile( ui->lb_CWP_Directory->text() ) );
}


