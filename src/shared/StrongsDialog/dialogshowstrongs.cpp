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
*/#include "mainwindow.h"
#include "dialogshowstrongs.h"
#include "ui_dialogshowstrongs.h"
#include "dialogenglishdictionary.h"
#include "strongsmodule.h"
#include "misclibrary.h"
#include "dialogenglishdictionary.h"
#include "bibledictionarydialog.h"
#include "MediaModuleDatabase.h"
#include "dialogbiblepicture.h"
#include "WebEngineViewWebPage.h"

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogShowStrongs::DialogShowStrongs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowStrongs)
{
    ui->setupUi(this);
#ifdef WEBKIT_USED
    webView = new QWebView(this);
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);// or DelegateAllLinks
    connect(webView->page() , SIGNAL( linkClicked(const QUrl &)), this, SLOT(link_click(const QUrl &)));
#endif
#ifdef WEBENGINE_USED
    webView = new QWebEngineView(this);
    webView->setPage(new MyWebPage());
    connect(webView->page(),SIGNAL(linkClicked(QUrl)),this,SLOT(link_click(const QUrl &)));
#endif

    webView->setObjectName(QStringLiteral("webView"));
    webView->setGeometry(QRect(5, 80, 391, 261));
    webView->setUrl(QUrl(QStringLiteral("about:blank")));

    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close

    this->setWindowTitle( tr("Strong's Definition"));



    QRect r;
    r = webView->geometry();
    mainwindowWidth =this->width();
    mainwindowHeight = this->height();
   // webviewOffset = this->height() - (r.height()+r.y());

    internalClick = false;
    //this->setParent(fvcp);
  this->setModal(true);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::showEvent(QShowEvent *e){
Q_UNUSED(e)
/*
    speakHelp("Here is the definition of the root word in the original Language for the word you touched;  Touch a colored number to navigate to a precursor definition.");
    if (Preferences.enableEnglishDictionary) {
        speakHelp("Touch the Dictionary button to look up the english word in Webster's dictionary.");
    }
*/
   /* if (Preferences.enableEnglishDictionary && [dictionarySearchWord length]  && !Instr(dictionarySearchWord, @"%20")) {
        if (!self.navigationItem.rightBarButtonItem) {
            UIBarButtonItem *nButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Dictionary" style:UIBarButtonItemStyleBordered target:self action:@selector(buttonDictionaryPush)];

            self.navigationItem.rightBarButtonItem = nButtonItem;
            [nButtonItem release];
        }
    }else {
        self.navigationItem.rightBarButtonItem = nil;
    }

   handleRS();
*/
    handleRS();
    ui->pbLast->setVisible(NO);
    BuildAndShowStrongsPage();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogShowStrongs::~DialogShowStrongs()
{
    delete ui;
}
//int i;
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
bool DialogShowStrongs::event(QEvent *e) {

#ifdef Q_OS_MAC
    if (e->type()==QEvent::WindowDeactivate){
        if(internalClick){
            internalClick=false;
        } else{
            close();
        }
    }

#else
#ifdef Q_OS_WIN
    if (e->type()==QEvent::WindowDeactivate){
        if(internalClick){
            internalClick=false;
        } else{
            close();
        }
    }
#endif
#endif

    return QDialog::event(e);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::moveButton(QPushButton *pb, int resizex) {
     QRect r = pb->geometry();
    int width = r.width();
    int height = r.height();

    r.setX(r.x()+resizex);
    r.setY(r.y());
    r.setHeight(height);
    r.setWidth(width);

    pb->setGeometry(r);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::moveLineEdit(QLineEdit *le,int resizex){
    QRect r = le->geometry();
   int width = r.width();
   int height = r.height();

   r.setX(r.x()+resizex);
   r.setY(r.y());
   r.setHeight(height);
   r.setWidth(width);

   le->setGeometry(r);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::moveLine(QFrame *le,int resizex){
    QRect r = le->geometry();
   int width = r.width();
   int height = r.height();

   r.setX(r.x());
   r.setY(r.y());
   r.setHeight(height);
   r.setWidth(width+resizex);

   le->setGeometry(r);
}
//------------------------------------------------------------
// overrides resizeEvent to allow button movement
//------------------------------------------------------------
void DialogShowStrongs::resizeEvent ( QResizeEvent * event ) {

    Q_UNUSED(event);


    // move the ok button
   QRect r = webView->geometry();
    int newx,newy,resizex,resizey;
    //r.setX(r.x()+20);
    newx = this->width();
    newy = this->height();
    resizex = newx - mainwindowWidth;
    resizey = newy - mainwindowHeight;
    //x = this->ui->pbQuit->pos().x();
    //y = this->ui->pbQuit->pos().y();
    //this->ui->pbQuit->move(x+resizex, y+resizey);
    //this->webView->width() = newx;
    r.setWidth(resizex +r.width());
    r.setHeight(resizey + r.height());

    webView->setGeometry(r);

    mainwindowWidth = newx;
    mainwindowHeight = newy;
    //activityIndicator->stopAnimation();
    r = ui->pbOK->geometry();
    int width = r.width();
    int height = r.height();
    r.setX(r.x()+resizex/2);
    r.setY(r.y()+resizey);
    r.setHeight(height);
    r.setWidth(width);
    ui->pbOK->setGeometry(r);

    moveButton(ui->pbDictionary,resizex);
    moveButton(ui->pbLanguage,resizex);
    moveLineEdit(ui->lineEdit,resizex);

    moveLine(ui->line,resizex);
    // width = r.width();
    // height = r.height();

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::handleRS() {

    int result;

    rs= nil;

    extern MediaModuleDatabase *gMediaModuleDatabase;

    if (!gMediaModuleDatabase) {
        ui->pbPictures->setVisible(NO);
        return;
    }


    rs = gMediaModuleDatabase->getMediaListUsingLanguage(Language,/*andStrongsNumber*/QString::number(strongsNumber),/* andSaveResultTo*/&result);



    if (rs && rs->NumberOfRows>0) {
       ui->pbPictures->setVisible(YES);
        // UIBarButtonItem *nButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Pictures" style:UIBarButtonItemStyleBordered target:self action:@selector(buttonPicturesPush)];
    } else {
        ui->pbPictures->setVisible(NO);
        //self.navigationItem.leftBarButtonItem=nil;
    }


}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::returnToLastLocation()
{
    QString lastLocation;

    lastLocation =getLastLocation();
    if (lastLocation.length()) {

        if (lastLocation=="" /*NO_MORE_LOCATIONS_STRING*/ ) {
            //buttonLastStrongs.hidden = YES;
            ui->pbLast->setVisible(NO);
            return;
        }

        goToLocation(lastLocation);
    }
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
static STRONGS_STRUCTURE_TYPE dummy;
void DialogShowStrongs::BuildAndShowStrongsPage()
{
    STRONGS_STRUCTURE_TYPE *StrongsStructure;
    QString StrongsHTML;
    if (gStrongsDatabase) {
        StrongsStructure = gStrongsDatabase->GetStrongsDefinition(strongsNumber,Language);
    }  else {
        StrongsStructure = &dummy;
        StrongsStructure->Definition=tr("Definition not found. Strongs Database may be missing.");
        StrongsStructure->Language='-';
        StrongsStructure->Phonic="";
        StrongsStructure->OriginalWord="";
        StrongsStructure->Number=0;
    }

    ui->pbLanguage->setText(StrongsStructure->Language);
    ui->lineEdit->setText(QString::number(StrongsStructure->Number));

    StrongsHTML = GetStrongsHTML(StrongsStructure);
    webView->setHtml(StrongsHTML ,QUrl::fromLocalFile("/"));



}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogShowStrongs::goToLocation(QString strongsLocation)
{
    QStringList loc = mySplit(strongsLocation, ":");

    Language =loc[0];// = [(NSString*)[loc objectAtIndex:0] characterAtIndex:0];
    strongsNumber = loc[1].toInt();//[(NSString*)[loc objectAtIndex:1] intValue];
    BuildAndShowStrongsPage();
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString DialogShowStrongs::getLastLocation()
{
    QString lastLoc;

    if (LastItems.count()==0) {
        lastLoc =""; //NO_MORE_LOCATIONS_STRING;
        return lastLoc;
    }

    lastLoc = LastItems.last();
            LastItems.removeLast();

    if (LastItems.count()==0) {
        ui->pbLast->setHidden(YES);
//        buttonLastStrongs.hidden = YES; // hide the back button
    }

    return lastLoc;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::saveCurrentLocation()
{
    QString strongsLoc = Language+":"+ QString::number(strongsNumber);
    LastItems.append(strongsLoc);
    ui->pbLast->setVisible(YES);
    //buttonLastStrongs.hidden = NO;// show the "back" button
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::link_click(const QUrl &url)
{

    QString urlString = url.toString();
    QChar c;

    internalClick=true;
    if (Instr(urlString,"_"))
    {
        urlString = Right(urlString, 7);// remove http://
        QString Number;
        QStringList array;
        urlString.remove('/');
        array = mySplit(urlString,"_");
        if (array.count()) {
            switch(array[0].at(0).toUpper().toLatin1()){
            case 'X': {
                Number =Right(array[1],1) ;
                if (Instr(Number,"/"))
                    Number = Left(Number,Number.length()-1);

                int number = Number.toInt();


                if (number==0) {
                    Number=Right(array[1] ,1); // try part number, a G or H in front of number
                    number = Number.toInt();
                    c = array[1].at(0).toUpper().toLatin1();
                } else {
                    // get the language from the cross reference word
                    c = array[1].at(0).toUpper().toLatin1();
                    if (!(c=='H' || c=='G'))
                       c=Language.at(0);// otherwise keep the current Language.
                }

                // if number is zero, don't move from this location.

                if (number ) {
                    if ((c=='G' || c=='H')) {
                        saveCurrentLocation(); // before overwriting the values, save where we are
                        Language = QString(c);
                        strongsNumber = number;
                        BuildAndShowStrongsPage();
                    }
                }
            }
                break;
            case 'D':
            //EnglishDictionary:
                // english dictionary
                if (array.count()>1) {
                    extern QString dictionarySearchWord; // global so everyone can see


                    dictionarySearchWord = array[1];
                    fvcp->showEnglishDictionary();
                    //handleElementID(array[2]);

                    return ;
                }

            break;
            }// switch
        }// if array.count
    }// if (Instr

}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::on_pbOK_clicked()
{
    close();
}



// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::on_pbLeft_clicked()
{
    saveCurrentLocation();

    if(--strongsNumber<1)
        strongsNumber=1;
     BuildAndShowStrongsPage();
    //handleRS();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::on_pbRight_clicked()
{
    saveCurrentLocation();

    if (Language == "G") {
        if (++strongsNumber >MAXIMUM_GREEK_STRONGS ) {
            strongsNumber = MAXIMUM_GREEK_STRONGS;
        }
    } else {
        if (++strongsNumber > MAXIMUM_HEBREW_STRONGS) {
            strongsNumber = MAXIMUM_HEBREW_STRONGS;
        }
    }
    BuildAndShowStrongsPage();
        //handleRS();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::on_pbLast_clicked()
{
    if (LastItems.count()==0)
        return;

    QString Loc = LastItems.last();
    LastItems.removeLast();

    if (LastItems.count()==0)
        ui->pbLast->setVisible(NO);

    QStringList l = Loc.split(":");

    Language = l[0];
    strongsNumber = l[1].toInt();
    BuildAndShowStrongsPage();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::on_pbLanguage_clicked()
{
//    - (IBAction)buttonGreekHebrewPushed:(id)sender {
        if (Language == "G") {
            ui->pbLanguage->setText("H");
            Language = "H";
        } else {
            ui->pbLanguage->setText("G");
            Language = "G";
        }

        on_lineEdit_editingFinished();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::on_lineEdit_editingFinished()
{

    if (strongsNumber<1) {
        strongsNumber=1;
    }

    if (Language == "H") {
        if (strongsNumber > MAXIMUM_HEBREW_STRONGS) {
            strongsNumber = MAXIMUM_HEBREW_STRONGS;
        }
    } else {
        if (strongsNumber >MAXIMUM_GREEK_STRONGS ) {
            strongsNumber = MAXIMUM_GREEK_STRONGS;
        }
    }

    BuildAndShowStrongsPage();
        //handleRS();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::on_pbPictures_clicked()
{
    DialogBiblePicture *dbp = new DialogBiblePicture();
    dbp->rs = rs;
    dbp->show();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::on_lineEdit_textEdited(const QString &arg1)
{
    //
    strongsNumber = arg1.toInt();
    on_lineEdit_editingFinished();// pick up other code
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::on_pbDictionary_clicked()
{
    DialogEnglishDictionary *dedp = new DialogEnglishDictionary(nullptr);
// set the dictionary word to look up here
    dedp->dstp = this;// so we can set its focus
    dedp->calledFromStrongs = YES;
    dedp->dictionarySearchWord = textInBible;
    dedp->show();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogShowStrongs::on_pbBibleDictionary_clicked()
{
    //
    fvcp->on_actionBible_Dictionary_triggered();
    // now make the bible dictionary show the word
    bibleDictionaryWindowPtr->lookUpWordAndShowOnWebPage(textInBible);
    close(); // clean up
}
