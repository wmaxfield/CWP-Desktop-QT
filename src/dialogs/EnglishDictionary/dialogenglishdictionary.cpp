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
*///
//  Dictionary.m
//  CWP-Student
//
//  Created by WADE MAXFIELD on 1/21/11.
//  Copyright 2011, 2020 Wade Maxfield. All rights reserved.
//

#include "dialogenglishdictionary.h"
#include "ui_dialogenglishdictionary.h"
#include "misclibrary.h"
#include "structs.h"
#include "preferencesmodule.h"
#include "HTMLModule.h"
#include "bibledictionarydialog.h"
#include "WebEngineViewWebPage.h"
#include "timerclass.h"
#include "noteswindow.h"

static NotesWindow *dnw;
static TimerClass *nwTimer;
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
static void handleDebugWebWindowShow(QString vp) {

    if (!dnw)
        dnw = new NotesWindow();

    if (dnw) {
        dnw->dontReloadText=YES;
        dnw->show();
        dnw->setPlainText(vp);

    }

}


DialogEnglishDictionary::DialogEnglishDictionary(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEnglishDictionary)
{

    ui->setupUi(this);

#ifdef WEBKIT_USED
    webView = new QWebView(this);
#endif
#ifdef WEBENGINE_USED
    webView = new QWebEngineView(this);
#endif

    webView->setObjectName(QStringLiteral("webView"));
    webView->setGeometry(QRect(5, 50, 390, 231));
    webView->setUrl(QUrl(QStringLiteral("about:blank")));


#ifdef WEBKIT_USED
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);// or DelegateAllLinks
    //webView->setStyleSheet("");
    webView->setGeometry(QRect(5, 50, 390, 231));

    connect( webView->page() , SIGNAL( linkClicked(const QUrl &)), this, SLOT(link_click(const QUrl &)));
    //connect( webView, SIGNAL(loadFinished(bool)),this,SLOT(webViewDidFinishLoad(bool)));
#endif
#ifdef WEBENGINE_USED
    webView->setPage(new MyWebPage());

    connect( webView->page() , SIGNAL( linkClicked(const QUrl &)), this, SLOT(link_click(const QUrl &)));

#endif

    connect(&webTimer,SIGNAL(timeout()),this,SLOT(processLink()));

    dstp=0;
    mainwindow=0;
    calledFromStrongs = NO;

    mainwindowWidth = this->width();
    mainwindowHeight = this->height();

    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close

    this->setWindowTitle( tr("Webster's 1913 Dictionary"));

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogEnglishDictionary::~DialogEnglishDictionary()
{
    delete ui;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogEnglishDictionary::showEvent(QShowEvent *e) {
    Q_UNUSED(e);

    if (!gDictionaryDB) {
        // English Dictionary
        QString DictionaryDBName = getFullPathOfAppBundleCWPDictionaryDirectory() + QString(ENGLISH_DICTIONARY_DB_NAME);

        gDictionaryDB = new dictionaryDB( /*initWithDBName:*/ DictionaryDBName,
                                          /* andType:*/TYPE_DICTIONARY_TYPE,
                                          /*andDictionaryType:*/DICTIONARY_TYPE_ENGLISH_DICTIONARY);
        if (!gDictionaryDB)
            return;
    }

     ui->lineEdit->setText(this->dictionarySearchWord);
     on_pbSearch_clicked();
}
//------------------------------------------------------------
// overrides resizeEvent to allow button movement
//------------------------------------------------------------
void DialogEnglishDictionary::resizeEvent ( QResizeEvent * event ) {

    Q_UNUSED(event);
    // move the quit button
   QRect r = webView->geometry();
    int newx,newy,resizex,resizey;
    //r.setX(r.x()+20);
    newx = this->width();
    newy = this->height();
    resizex = newx - mainwindowWidth;
    resizey = newy - mainwindowHeight;

    r.setWidth(resizex +r.width());
    r.setHeight(resizey + r.height());

    webView->setGeometry(r);

    r = ui->pbOK->geometry();
    int width = r.width();
    int height = r.height();
    r.setX(r.x()+resizex/2);
    r.setY(r.y()+resizey);
    r.setHeight(height);
    r.setWidth(width);
    ui->pbOK->setGeometry(r);


    // newy - 50;
    mainwindowWidth = newx;
    mainwindowHeight = newy;
    //activityIndicator->stopAnimation();

}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogEnglishDictionary::updateDisplayWithSearchWordDefinition() {


    QString vp;

    if (Instr(dictionarySearchWord, "%20")) {
        dictionarySearchWord = myReplace(dictionarySearchWord,"%20"," ");
    }


    vp = gDictionaryDB->getDefinitionsForWord(dictionarySearchWord,/* limitedToNumberOfDefinitions:*/numberOfDefinitions);
    int fontsize =Preferences->getFontSize();
    fontsize *= 0.7;
    if (fontsize<10) {
        fontsize+=2;
    }
//    MsgBox(vp); <a id="d5276" style="color:black" href="D_In_d5276">In</a>

    vp =   HTML_STANDARD_HEADER HTML_NO_UNDERLINE_STYLE "<p style=\"white-space: pre-wrap; word-wrap: break-word; font-size: "+QString::number(fontsize)+"pt\"" HTML_GENERIC_END_TAG +vp+   "</p>" HTML_STANDARD_END;

    webView->setHtml(vp,QUrl::fromLocalFile("/"));


    //---------------------------------------------------------
    // if debug html is set, show the html string for every page
    if (Preferences->getDebug() & DEBUG_HTML){
        if (!nwTimer)
            nwTimer = new TimerClass(50,YES,(void*)handleDebugWebWindowShow);
        else
            nwTimer->start(50);
        nwTimer->userString = vp;
    }

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogEnglishDictionary::link_click(const QUrl &url) {


    urlString = url.toString();

     webTimer.start(50);// when fires, processLink() is called
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogEnglishDictionary::processLink(){

    webTimer.stop();
    if (Instr(urlString,"_"))
    {
        if ( Instr(urlString, "file:///")) {
            urlString = Right(urlString, 8);// remove file:///
        }

        if (Instr(urlString,"http://") || Instr(urlString, "file://")) {
            urlString = Right(urlString, 7);// remove http://
        }

        QString Number;
        QStringList array;
        array = mySplit(urlString,"_");
        if (array.count()) {

            QChar c;

            Number = array[0];
            c = Number.at(0);


            switch (c.toUpper().toLatin1()) {

                case 'D':
                EnglishDictionary:
                    // english dictionary
                    if (array.count()>1) {
                        dictionarySearchWord = array[1];
                        updateDisplayWithSearchWordDefinition();
                        return ;
                    }
                    break;


                case '/':
                {
                c = array[0].at(0).toUpper().toLatin1();
                    if (c == 'D') {
                        goto EnglishDictionary;
                    }
                }
                    break;

                default:
                    break;
            }

        }
    }
}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogEnglishDictionary::on_pbOK_clicked()
{
    close();
    /* following not needed if modal dialog
     * if dialog is not modal, then is needed

    if (dstp) {
        dstp->hide();
        dstp->show();
    } else {

        if (mainwindow){
            mainwindow->hide();
            mainwindow->show();
        }


    }*/

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogEnglishDictionary::on_lineEdit_editingFinished()
{
    // look up the word
    on_pbSearch_clicked();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogEnglishDictionary::on_pbSearch_clicked()
{

    numberOfDefinitions = 20;
    this->dictionarySearchWord = ui->lineEdit->text();
    updateDisplayWithSearchWordDefinition();

}

void DialogEnglishDictionary::on_pbBibleDictionary_clicked()
{
    fvcp->on_actionBible_Dictionary_triggered();
    // now make the bible dictionary show the word
    bibleDictionaryWindowPtr->lookUpWordAndShowOnWebPage(dictionarySearchWord);
    close();
}
