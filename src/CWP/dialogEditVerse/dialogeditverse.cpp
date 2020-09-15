#include "dialogeditverse.h"
#include "ui_dialogeditverse.h"
#include <QStringList>
#include "structs.h"
#include "Preferences/PreferencesLibrary.h"
#include "preferencesmodule.h"
#include "BibleDatabaseClass/bibledatabaseclass.h"
#include "biblemodule.h"
#include "misclibrary.h"
#include "strongsdatabaseclass.h"
#include "strongsmodule.h"
#include "mainwindow.h"
#include "bibleinfo.h"
#include "WebEngineViewWebPage.h"

DialogEditVerse *devPtr;

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
DialogEditVerse::DialogEditVerse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditVerse)
{
    //devPtr = this;
    A_me="DialogEditVerse";
    ui->setupUi(this);

#ifdef WEBKIT_USED
    webView = new QWebView(this);
#endif
#ifdef WEBENGINE_USED
    webView = new QWebEngineView(this);
#endif
    webView->setObjectName(QStringLiteral("webView"));
    webView->setGeometry(QRect(339, 244, 486, 306));
    webView->setMinimumSize(QSize(339, 276));
    webView->setMaximumSize(QSize(2000, 2000));
    webView->setUrl(QUrl(QStringLiteral("about:blank")));

    windowWidth = this->width();
    windowHeight = this->height();

    timerID=startTimer(50);
    verseChanged=false;
    doInitialRowClick=false;
    canResize=false;
    word = new QStringList();
    post = new QStringList();
    pre = new QStringList();
    lang = new QStringList();
    dict = new QStringList();
    wseq = new QStringList();
    wformat = new QStringList();

    _Book=0;
    _StartChapter=_EndChapter=1;
    _StartVerse=_EndVerse=1;
    _BibleName="";
    _BibleNumber=0;
    setWindowFlags(Qt::Popup);
}
//------------------------------------------------------------
//------------------------------------------------------------
 void DialogEditVerse::moveVerticalLBL(QLabel *le,int resizey){
    int y = le->pos().y();
    int x = le->pos().x();
     le->move(x,y+resizey);
}
//------------------------------------------------------------------------------------------------------------------
// allow caller to set information needed for showing verses
//------------------------------------------------------------------------------------------------------------------
DialogEditVerse::DialogEditVerse(int Book, int Chapter, int Verse,int BibleNumber,QString BibleName,QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogEditVerse)
{
    ui->setupUi(this);

#ifdef WEBKIT_USED
    webView = new QWebView(this);
#endif
#ifdef WEBENGINE_USED
    webView = new QWebEngineView(this);
    webView->setPage(new MyWebPage());
//    connect(webView->page(),SIGNAL(linkClicked(QUrl)),this,SLOT(link_click(const QUrl &)));

#endif
    webView->setObjectName(QStringLiteral("webView"));
    webView->setGeometry(QRect(339, 244, 486, 306));
    webView->setMinimumSize(QSize(339, 276));
    webView->setMaximumSize(QSize(2000, 2000));
    webView->setUrl(QUrl(QStringLiteral("about:blank")));

    windowWidth = this->width();
    windowHeight = this->height();

    timerID=startTimer(50);
    verseChanged=false;
    doInitialRowClick=false;
    canResize=false;
    word = new QStringList();
    post = new QStringList();
    pre = new QStringList();
    lang = new QStringList();
    dict = new QStringList();
    wseq = new QStringList();
    wformat = new QStringList();

    _Book=Book;
    _StartChapter=_EndChapter=Chapter;
    _StartVerse=_EndVerse=Verse;
    _BibleName=BibleName;
    _BibleNumber=BibleNumber;
    this->setWindowTitle(_BibleName +":"+BibleBook(Book));
    paramsSet=true;

}
//------------------------------------------------------------
//------------------------------------------------------------
/*
void DialogEditVerse::focusOutEvent(QFocusEvent *event){
 Q_UNUSED( event);
   // if (event->type()==QEvent::FocusOut){
        close();
   //     return ;
   // }

   // return ;
}*/
//------------------------------------------------------------
//------------------------------------------------------------
 void DialogEditVerse::moveVerticalButton(QPushButton *b, int resizey){
   int y = b->pos().y();
   int x = b->pos().x();
    b->move(x,y+resizey);
}
//------------------------------------------------------------
//------------------------------------------------------------
 void DialogEditVerse::moveVerticalLE(QLineEdit *le,int resizey){
    int y = le->pos().y();
    int x = le->pos().x();
     le->move(x,y+resizey);
}
//------------------------------------------------------------
//------------------------------------------------------------
void DialogEditVerse::update(){
    getVerseRecord();

    if(word->count()>0){
            if (ui->listWidget->count()>0){
                ui->listWidget->setCurrentRow(0);
                // postpone initial row count
                doInitialRowClick=true;
            }

        ui->listWidget->setFocus();
    }
}
//------------------------------------------------------------------------------------------------------------------
// word clicked on
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::handleRowClick(int indexPath){
  // indexPath= index.row();

   QListWidgetItem *itemPtr;

    itemPtr=ui->listWidget->currentItem();
    if (!itemPtr)
        return; // nothing to happen
    QString itemName = itemPtr->text();

    ui->leFormat->setText((*wformat)[indexPath]);
    ui->leLang->setText((*lang)[indexPath].trimmed());
    ui->leDict->setText((*dict)[indexPath].trimmed());
    ui->lePost->setText((*post)[indexPath].trimmed());
    ui->lePre->setText((*pre)[indexPath].trimmed());
    wordSequenceNumber=(*wseq)[indexPath].toInt();
    ui->teWord->document()->setPlainText((*word)[indexPath]);

    ui->leSeq->setText((*wseq)[indexPath]);
    QString dict,lang;
    dict=ui->leDict->text();
    lang=ui->leLang->text();
    if (dict.toInt()>0 && lang.length()>0){
        // we can show strong's
        BuildAndShowStrongsPage(dict.toInt(),lang);
    }else {
        QString s =ui->lePost->text();
         int n =s.toInt();
         if (n && lang.length()>0){
             BuildAndShowStrongsPage(n,lang);// is interlinear
         } else {
            webView->setHtml("");
         }
    }
}
//------------------------------------------------------------------------------------------------------------------
// show the strong's on the dialog
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::BuildAndShowStrongsPage(int strongsNumber,QString language)
{
    STRONGS_STRUCTURE_TYPE *StrongsStructure;
    QString StrongsHTML;
    if (gStrongsDatabase) {
        StrongsStructure = gStrongsDatabase->GetStrongsDefinition(strongsNumber,language);
    }  else {
        StrongsStructure = &dummy;
        StrongsStructure->Definition=tr("Definition not found. Strongs Database may be missing.");
        StrongsStructure->Language='-';
        StrongsStructure->Phonic="";
        StrongsStructure->OriginalWord="";
        StrongsStructure->Number=0;
    }

    StrongsHTML = GetStrongsHTML(StrongsStructure);
    webView->setHtml(StrongsHTML ,QUrl::fromLocalFile("/"));
}
//------------------------------------------------------------
//------------------------------------------------------------
DialogEditVerse::~DialogEditVerse()
{
    killTimer(timerID);
    delete ui;
   delete word;
   delete post ;
   delete pre;
   delete lang ;
   delete dict;
   delete wseq ;
   delete wformat ;

}
//------------------------------------------------------------
//------------------------------------------------------------
void DialogEditVerse::timerEvent(QTimerEvent *e){
    Q_UNUSED(e);
    //static int16_t countup;

    if (doInitialRowClick) {
        doInitialRowClick=false;
        handleRowClick(0);
        canResize=true;
        //update();
    }
    //i++;
}

//------------------------------------------------------------
//------------------------------------------------------------
void DialogEditVerse::showEvent(QShowEvent *e)
{
    Q_UNUSED(e);
    if (paramsSet){
        paramsSet=false;
        update();
    }
    //i++;
}
/*
bool DialogEditVerse::event(QEvent *e)
{
    bool result = QDialog::event(e);
    enum QEvent::Type ev = e->type();

    switch(ev){
        case QEvent::Timer:
         return result;
        //break;

        default:
          return result;
    }
}*/

//------------------------------------------------------------------------------------------------------------------
// get the verse from the bible
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::getVerseRecord(){
    BibleBookInfoStructType binfo = getBibleBookInfo(_Book);
    QString bookTableName = binfo.TableName;

        //
        SEARCH_INFO_TYPE sit ;
        BibleDatabaseClass *bibleDatabaseClass =((BibleDatabaseClass*)(Books[_BibleNumber]->Database));
        sit.BookTableName = bookTableName;
        sit.ChapterNumber = _StartChapter;
        sit.VerseNumber = _StartVerse;
        //QString returnValue;
        if (!word->isEmpty())
            word->clear();

        if (!post->isEmpty())
            post->clear();
        if (!pre->isEmpty())
            pre->clear();
        if (!lang->isEmpty())
            lang->clear();
        if (!dict->isEmpty())
            dict->clear();
        if (!wformat->isEmpty())
            wformat->clear();
        if (!wseq->isEmpty())
            wseq->clear();


        //env->ReleaseStringUTFChars(bookTableName_, bookTableName);
        RecordSet *rs;
       rs= bibleDatabaseClass->GetVerseRecords(&sit);

        //   QueryBegin = "SELECT pre, Word, post, OrigWordLanguage, DictEntryNumber, WordSeqNumber, WordFormat from " ;

        if (rs){
            if ((rs->FieldCount() != 0 )) {
                while (! rs->atEOF() ) {
                    word->append(rs->Field((char*)WORD_COLUMN));
                    post->append( rs->Field((char*)WORD_POST));
                    pre->append( rs->Field((char*)WORD_PRE));
                    lang->append(rs->Field((char*) ORIG_WORD_TYPE_COLUMN ));// only one field
                    dict->append(rs->Field((char*)DICTIONARY_ENTRY_COLUMN));
                    wformat->append(rs->Field((char*)WORD_FORMAT));
                    wseq->append(rs->Field((char*)WORD_SEQ_COLUMN ));

                   //stringList.append(v1+"_"+v2+"_"+v3+"_"+v4+"_"+v5+"_"+v6+"_"+v7);

                    rs->MoveNext();
                }
            }
            rs->done();// release sqlite table NOW
            delete rs;
        }

        // put the verse into the list

        ReloadData();
}
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::insertSpaceInWordSeqNumbersAtCurrentIndex()
{
    BibleBookInfoStructType binfo = getBibleBookInfo(_Book);
    QString bookTableName = binfo.TableName;


    BibleDatabaseClass *bibleDatabaseClass =((BibleDatabaseClass*)(Books[_BibleNumber]->Database));
    // search info
    SEARCH_INFO_TYPE sit ;
    sit.BookTableName = bookTableName;
    sit.ChapterNumber = _StartChapter;
    sit.VerseNumber = _StartVerse;
    //sit.WordSequenceNumber = wordSequenceNumber;

    int index = ui->listWidget->currentRow();
    if (index < 0)
        return;

    bibleDatabaseClass->showSQLExecError=1;

    // from current array index (which matches currentRow())
    // add one to the index
    // start from the back and move to the front, just in case we are
    // interrupted by power out, shutdown, crash, etc.
    int16_t endOfArray = word->size();
    int16_t i;


    for (i = endOfArray-1; i >= index ; i--){
        sit.WordSequenceNumber=(*wseq)[i].toInt();
        bibleDatabaseClass->updateWordSeqNumber(&sit,sit.WordSequenceNumber+1);
        (*wseq)[i]=QString::number(sit.WordSequenceNumber+1);// update in memory
    }
   // return;

    wseq->insert(index,QString::number(sit.WordSequenceNumber));
    //everything else is blank
    post->insert(index,"");
    pre->insert(index,"");
    lang->insert(index,"");
    dict->insert(index,"");
    wformat->insert(index,"");
    word->insert(index,"");

    on_pbClear_clicked();// zero the fields on the form

    ui->leSeq->setText(QString::number(wordSequenceNumber));
    // indicate the verse has changed
    verseChanged=true;

    bibleDatabaseClass->InsertWordIntoDatabase(&binfo, /*Chapter*/_StartChapter, /*Verse*/_StartVerse,
                                               /* WordSequence*/wordSequenceNumber,
                                /*Word*/"", /*OrigWordType*/"", /*DictionaryEntry*/ "",
                                /*PreWord*/  "", /*PostWord*/ "");


    //on_pbUpdateNClose_clicked();

    ReloadData();// update list box

    ui->listWidget->setCurrentRow(wordSequenceNumber);
}
//------------------------------------------------------------------------------------------------------------------
// populate the list box
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::ReloadData() {
    ui->listWidget->clear();

    int16_t index;
    int Count;

    Count = word->count();

    for (index =0; index < Count ;index++) {
        ui->listWidget->addItem((*word)[index]);
    }
}
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_pbOK_clicked()
{
    close();
    delete this;
    devPtr=0;
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_listWidget_clicked(const QModelIndex &index)
{
     handleRowClick(index.row());
}
//------------------------------------------------------------------------------------------------------------------
// update the bible database
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_pbUpdate_clicked()
{

    if (!verseChanged){
        return;
    }
    verseChanged=false;

    BibleBookInfoStructType binfo = getBibleBookInfo(_Book);
    QString bookTableName = binfo.TableName;


    BibleDatabaseClass *bibleDatabaseClass =((BibleDatabaseClass*)(Books[_BibleNumber]->Database));
       // search info
       SEARCH_INFO_TYPE sit ;
       sit.BookTableName = bookTableName;
       sit.ChapterNumber = _StartChapter;
       sit.VerseNumber = _StartVerse;
       if (ui->leSeq->text().toInt()<0){
           MsgBox("Error in seq# field, update abandoned.");
           return;
       }

       sit.WordSequenceNumber = ui->leSeq->text().toInt();

  //     updateWordRecordInfo(SEARCH_INFO_TYPE*SIT,
  //                          /*usingWord*/QString Word,
  //                          /*andPre*/QString Pre,
  //                           * /*andPost*/QString Post,
  //                          /*andLang*/QString Lang,
  //                          /*andNumber*/ QString Number, QString Format)


      /*BOOL result =*/ bibleDatabaseClass->updateWordRecordInfo(&sit,
                                                             ui->teWord->toPlainText(),
                                                             ui->lePre->text(),
                                                             ui->lePost->text(),
                                                             ui->leLang->text(),
                                                             ui->leDict->text(),
                                                             ui->leFormat->text());

    int index = ui->listWidget->currentRow();
    getVerseRecord(); // pick up the new database info

    ui->listWidget->setCurrentRow(index);
    handleRowClick(index);
    ui->listWidget->setFocus();

   // Preferences->setCurrentBook(cvs.BookNumber);
    Preferences->setCurrentChapter(_StartChapter);
    Preferences->setCurrentVerse(_StartVerse);
    Preferences->currentPosition = 0;

    fvcp->doScrollToVerseAfterWebPageLoad=TRUE;
    fvcp->verseToScrollToAfterWebPageLoad=_StartVerse;

    fvcp->LoadWebPage(); // force main window to reload page

}
//------------------------------------------------------------
// overrides resizeEvent to allow button movement
//------------------------------------------------------------
void DialogEditVerse::resizeEvent1 ( QResizeEvent * event ) {

    Q_UNUSED(event);

    if (!canResize)
        return;// don't do anything until it stabilizes

    // move the webview
   QRect r = webView->geometry();

   // pick up the new x and y
   int newx,newy,resizex,resizey;
    newx = this->width();
    newy = this->height();

    resizex = newx - windowWidth;
    resizey = newy - windowHeight;

    windowWidth = newx;
    windowHeight = newy;

    int x = ui->pbOK->pos().x();
    int y = ui->pbOK->pos().y();
    this->ui->pbOK->move(x+resizex, y+resizey);

    // move the webview up and down (but don't make wider)
    r.setY(r.y()+resizey);
    r.setHeight(webView->geometry().height());
    webView->setGeometry(r);

    moveVerticalButton(ui->pbUpdate,resizey);
    moveVerticalButton(ui->pbClear,resizey);
    moveVerticalButton(ui->pbCPP,resizey);
    moveVerticalButton(ui->pbLC,resizey);
    moveVerticalButton(ui->pbUC,resizey);

    moveVerticalLBL(ui->lblDict,resizey);
    moveVerticalLBL(ui->lblLang,resizey);
    moveVerticalLBL(ui->lblPicture,resizey);

    moveVerticalLE(ui->leDict,resizey);
    moveVerticalLE(ui->leFormat,resizey);
    moveVerticalLE(ui->leLang,resizey);
    // move list up and down
     x = ui->listWidget->pos().x();
     y = ui->listWidget->pos().y();
    ui->listWidget->move(x,y+resizey);

    // make groupbox taller and wider
    r = ui->groupBox->geometry();

    r.setHeight(r.height()+resizey);
    r.setWidth(r.width()+resizex);
    ui->groupBox->setGeometry(r);

    // make text edit taller and wider
    r = ui->teWord->geometry();
    r.setHeight(r.height()+resizey);
    r.setWidth(r.width()+resizex);
    ui->teWord->setGeometry(r);


   #if 0

    // take care of the horizontal layout
    r = ui->horizontalLayoutWidget_2->geometry();
    r.setWidth(resizex +r.width());
    ui->horizontalLayoutWidget_2->setGeometry(r);



    // newy - 50;
     //activityIndicator->stopAnimation();


    r = ui->pbClose->geometry();
    int width = r.width();
    int height = r.height();
    r.setX(r.x()+resizex/2);
    r.setY(r.y()+resizey);
    r.setHeight(height);
    r.setWidth(width);
    ui->pbClose->setGeometry(r);
#endif
}

//------------------------------------------------------------
//------------------------------------------------------------
/*void DialogEditVerse::hideEvent(QHideEvent *e)
{
    Q_UNUSED(e)
    close();
}
*/
//------------------------------------------------------------
//------------------------------------------------------------
void DialogEditVerse::on_pbClear_clicked()
{
    ui->lePost->setText("");
    ui->lePre->setText("");
    ui->leDict->setText("");
    ui->leFormat->setText("");
    ui->leLang->setText("");
    ui->teWord->setPlainText("");
}

//------------------------------------------------------------
//------------------------------------------------------------
void DialogEditVerse::on_pbUC_clicked()
{
    ui->teWord->setPlainText( UpperCaseString(ui->teWord->toPlainText()));
    on_pbUpdate_clicked();
}

//------------------------------------------------------------
//------------------------------------------------------------
void DialogEditVerse::on_pbLC_clicked()
{
     ui->teWord->setPlainText( LowerCaseString(ui->teWord->toPlainText()));
     on_pbUpdate_clicked();
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_listWidget_currentRowChanged(int currentRow)
{
     handleRowClick(currentRow);
}

//------------------------------------------------------------------------------------------------------------------
// clear the pre and post fields
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_pbCPP_clicked()
{
    ui->lePost->setText("");
    ui->lePre->setText("");
    on_pbUpdate_clicked();
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_teWord_textChanged()
{
    verseChanged=true;
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_leLang_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    verseChanged=true;
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_leDict_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    verseChanged=true;
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_leFormat_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    verseChanged=true;
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_lePost_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    verseChanged=true;
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_lePre_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    verseChanged=true;
}

//------------------------------------------------------------------------------------------------------------------
// move through each verse record from the back and add 1 to the sequence number.
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_pbReSeq_clicked()
{
    insertSpaceInWordSeqNumbersAtCurrentIndex();
}
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_pbMoveToPost_clicked()
{
    // move last char in text to post
    QString s,s2;
     s= ui->teWord->toPlainText();
     s2= s.right(1);
     s = s.left(s.length()-1);
     ui->teWord->setPlainText(s);
     ui->lePost->setText(s2+ui->lePost->text());
         on_pbUpdate_clicked();
}
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_pbMoveToPre_clicked()
{
    // move last char in text to post
    QString s,s2;
     s= ui->teWord->toPlainText();
     s2= s.left(1);
     s = s.right(s.length()-1);
     ui->teWord->setPlainText(s);
     ui->lePre->setText(ui->lePre->text()+s2);
         on_pbUpdate_clicked();
}
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void DialogEditVerse::on_pbUpdateNClose_clicked()
{
    on_pbUpdate_clicked();
    on_pbOK_clicked();//close
}

void DialogEditVerse::on_pbQuoteRight_clicked()
{
    ui->lePost->setText(ui->lePost->text()+"&quot;");
}

void DialogEditVerse::on_pbQuoteLeft_clicked()
{
    ui->lePre->setText(ui->lePre->text()+"&quot;");
}

void DialogEditVerse::on_pbUpdate_2_clicked()
{
    on_pbUpdate_clicked();
}
