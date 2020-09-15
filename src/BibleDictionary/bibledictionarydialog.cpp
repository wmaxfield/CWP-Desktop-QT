
#include "bibledictionarydialog.h"
#include "ui_bibledictionarydialog.h"
#include "preferencesmodule.h"
#include "misclibrary.h"
#include "HTMLModule.h"
#include "dialogenglishdictionary.h"
#include "userhistoryclass.h"
#include "dictionarydb.h"
#include "noteswindow.h"
#include "timerclass.h"
#include "../shared/bibleinfo.h"
#include "WebEngineViewWebPage.h"

static NotesWindow *dnw;
static TimerClass *nwTimer;
//int urlChanged;

BibleDictionaryDialog *bibleDictionaryWindowPtr;

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
static void handleDebugWebWindowShow(QString vp) {
    Q_UNUSED(vp);


    if (!dnw)
        dnw = new NotesWindow();

    if (dnw) {
        dnw->dontReloadText=YES;
        dnw->show();
        dnw->setPlainText(vp);
    }
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDictionaryDialog::handleCurrentWebEngine() {
    #ifdef WEBKIT_USED
        webView = new QWebView(this);
        webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);// or DelegateAllLinks
        connect( webView->page() , SIGNAL( linkClicked(const QUrl &)), this, SLOT(link_click(const QUrl &)));
        connect(webView->page(),SIGNAL(downloadRequested(QNetworkRequest)),this,SLOT(myDownloadRequested(QNetworkRequest)));
    #endif
    #ifdef WEBENGINE_USED
        webView = new QWebEngineView(this);

    #endif
        webView->setObjectName(QStringLiteral("webView"));
        webView->setGeometry(QRect(-5, 45, 681, 501));
        webView->setStyleSheet(QStringLiteral("white-space: pre-wrap;"));
        webView->setUrl(QUrl(QStringLiteral("about:blank")));


    #ifdef WEBENGINE_USED
        webView->setPage(new MyWebPage());
        connect(webView->page(),SIGNAL(linkClicked(QUrl)),this,SLOT(link_click(const QUrl &)));
        connect(webView,SIGNAL(loadStarted()),this,SLOT(webPageLoadStarted()));
        connect( webView, SIGNAL(loadFinished(bool)),this,SLOT(webPageFinishedLoading(bool)));
        connect(webView,SIGNAL(urlChanged(QUrl)),this,SLOT(webPageURLChanged(const QUrl &)));

    #endif

        connect( webView, SIGNAL(loadFinished(bool)),this,SLOT(webPageFinishedLoading(bool)));
        connect(webView,SIGNAL(loadStarted()),this,SLOT(webPageLoadStarted()));
        connect(webView,SIGNAL(urlChanged(QUrl)),this,SLOT(webPageURLChanged(const QUrl &)));
        //connect(webView->page(),SIGNAL(downloadRequested(QNetworkRequest)),this,SLOT(myDownloadRequested(QNetworkRequest r)));

        webView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding );

        connect(&webTimer,SIGNAL(timeout()),this,SLOT(processLink()));

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BibleDictionaryDialog::BibleDictionaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BibleDictionaryDialog)
{
    ui->setupUi(this);

    handleCurrentWebEngine();

    //this->setStyleSheet("background-color: white;");
    mainwindowWidth = this->width();
    mainwindowHeight = this->height();

    connect(&searchTimer,SIGNAL(timeout()),this,SLOT(updateDisplayWithSearchWordDefinition()));


    numberOfDefinitions = 20;
    firstTime=TRUE;
    savedURL="";

    //speakHelp("This is Smith's 1884 dictionary, with images scanned from a copy of the original book.");
    //speakNext(" Enter your search text at the to
    QRect r=this->geometry();
    QString g = Preferences->getBibleDictionaryGeometry();
    QStringList a = g.split(",");
    if (a.length()>1){

        // x y w h
        r.setX(a[0].toInt());
        r.setY(a[1].toInt());
        r.setWidth(a[2].toInt());
        r.setHeight(a[3].toInt());

        if (r.x()<0){
            r.setX(10);
        }
        if (r.y()<0)
            r.setY(10);
        if (r.height()<100)
            r.setHeight(600);
        if (r.width()<50)
            r.setWidth(800);
    }else {
            r.setX(10);
            r.setY(10);
            r.setHeight(600);
            r.setWidth(800);
    }

    this->setGeometry(r);
    move(r.x(),r.y());

    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDictionaryDialog::doDataIndexBasedLoadWebPage(BOOL previous) {
    if (!gBibleDictionaryDB){
        openBibleDictionary();
    }

    if (gBibleDictionaryDB) {
       int startDataIndex=4;
       int numDefinitions = numberOfDefinitions;

       if (!previous) {
               if (gBibleDictionaryDB->iStartDataIndex <= 3) {
                   if (gBibleDictionaryDB->iStartDataIndex != 3) {
                       numDefinitions =1;
                   }

                   startDataIndex = gBibleDictionaryDB->iCurrentDataIndex+1;
               } else {
                   startDataIndex = gBibleDictionaryDB->iCurrentDataIndex;
               }
       } else {
           if (gBibleDictionaryDB->iStartDataIndex <= 4) {
               numDefinitions =1;
               startDataIndex = gBibleDictionaryDB->iStartDataIndex - numDefinitions;
           } else {
               startDataIndex = gBibleDictionaryDB->iStartDataIndex - numDefinitions+1;
           }

       }

       if (startDataIndex<=0) {
           startDataIndex=1;
           numDefinitions = 1;
       }
      LoadWebPage(startDataIndex,/* withNumberOfItems:*/numDefinitions);
   }
}
//------------------------------------------------------------
// overrides resizeEvent to allow button movement
//------------------------------------------------------------
void BibleDictionaryDialog::resizeEvent ( QResizeEvent * event ) {

    Q_UNUSED(event);
    /* QMessageBox msgBox;
       msgBox.setText("Resized");
       msgBox.exec();
       */
    //(void)event;
  // return;
    // move the quit button
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

    // take care of the horizontal layout
    webView->setGeometry(r);
    r = ui->horizontalLayoutWidget_2->geometry();
    r.setWidth(resizex +r.width());
    ui->horizontalLayoutWidget_2->setGeometry(r);



    // newy - 50;
    mainwindowWidth = newx;
    mainwindowHeight = newy;
    //activityIndicator->stopAnimation();


    r = ui->pbClose->geometry();
    int width = r.width();
    int height = r.height();
    r.setX(r.x()+resizex/2);
    r.setY(r.y()+resizey);
    r.setHeight(height);
    r.setWidth(width);
    ui->pbClose->setGeometry(r);

}
static bool shownBefore=true;// set to false to make first show always be the Front Cover
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDictionaryDialog::showEvent(QShowEvent *e) {
    Q_UNUSED(e)
    doDataIndexBasedLoadWebPage(NO);

    if (shownBefore){
        QString s = Preferences->getLastBibleDictionarySearchText();
        if (s.length()){
            ui->leSearchText->setText(s);
            lookUpWordAndShowOnWebPage(s);
        }
    }
    shownBefore=true;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BibleDictionaryDialog::~BibleDictionaryDialog()
{
    delete ui;
}

// ------------------------------------------------------------------------
// show the english dictionary definition of the word
// ------------------------------------------------------------------------
void BibleDictionaryDialog::showDictionary() {
    DialogEnglishDictionary *dedp = new DialogEnglishDictionary(nullptr);
// set the dictionary word to look up here
    dedp->mainwindow = nullptr;// so we can set its focus
    dedp->calledFromStrongs = YES;
    dedp->dictionarySearchWord = dictionarySearchWord;// pick up from global
    dedp->setModal(YES);
    dedp->show();

}


//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::link_click(const QUrl &url) {
//
    savedURL = urlString = url.toString();

    //BibleDictionaryDialog::processLink();
    webTimer.start(50);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDictionaryDialog::loadWebPageWithString(QString vp)
{
    vp = myReplace(vp,"ismith/","file:///"+imagePath);
    vp = myReplace(vp," )",")");
    vp = myReplace(vp,"( ","(");
    vp = myReplace(vp,"%d","2");


    vp =   HTML_STANDARD_HEADER HTML_NO_UNDERLINE_STYLE javascriptScrollOffset +vp+ HTML_STANDARD_END;

    webView->setHtml(vp, QUrl::fromLocalFile("/"));

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
void BibleDictionaryDialog::lookUpWordAndShowOnWebPage(QString word) {
    QString newWord=word;
    QStringList array;

    if (!gBibleDictionaryDB) {
      openBibleDictionary();
    if (!gBibleDictionaryDB)
        return;
    }

    word = myReplace(word,"[","");
    word = myReplace(word,"]","");
    word = myReplace(word,"%5D","");
    if (word.endsWith(","))
        word.chop(1);
    if (word.endsWith("."))
        word.chop(1);

    newWord = myReplace(word,"%5B","");


    if (Instr(newWord, " ")) {
        array = mySplit(newWord, " ");

        if (array.count()) {
            int index;
            for (index=0; index<array.count(); index++) {
                if (!gBibleDictionaryDB->isWordInBibleDictionary(array[index])) {
                    return;
                }
            }
        }
    } else {
        if (NO==gBibleDictionaryDB->isWordInBibleDictionary(newWord)) {
            return;
        }
    }

//lookUpWord:
    bibleDictionarySearchWord = newWord;
    ui->leSearchText->setText(newWord);// update the displayed text
    updateDisplayWithSearchWordDefinition();

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDictionaryDialog::reject()
{
    cleanUp();
    QDialog::reject();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDictionaryDialog::LoadWebPage(int startDataIndex,int numDefinitions)
{
    if (!gBibleDictionaryDB) {
        openBibleDictionary();
        if (!gBibleDictionaryDB)
            return;
    }

    QString vp= gBibleDictionaryDB->getBibleDictionaryDefinitionsForIndex(startDataIndex,/* andNumberOfDefinitions:*/numDefinitions,/* thenScrollToFieldID:*/startDataIndex);

    if (startDataIndex <= 3) {
        vp = myReplace(vp,"img", "img width=\"100%\"");
       // vp = myReplace(vp,"src=\"","src=\"file:///");
    }
    loadWebPageWithString(vp);
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::ReloadWebPage(){
    if (!gBibleDictionaryDB) {
        openBibleDictionary();
        if (!gBibleDictionaryDB)
            return;
    }
    LoadWebPage(gBibleDictionaryDB->iCurrentDataIndex,numberOfDefinitions);
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::myDownloadRequested(QNetworkRequest r) {
   // urlChanged++;
    Q_UNUSED(r)
}
// ------------------------------------------------------------------------
// search the directory locations and use the first smith's dictionary found
// ------------------------------------------------------------------------
void BibleDictionaryDialog::openBibleDictionary(){


    // user's directory takes precedence (usually in user's docments directory)
    if (!gBibleDictionaryDB) {
        QStringList dirArray =getDirectoryListing(getFullPathOfUserDocumentsCWPDictionaryDirectory());
        QString DictionaryDBName="";
        BOOL found=NO;
        int index;
        if (dirArray.count()>0) {
            // Bible Dictionary
            for (index=0; index < dirArray.count();index++) {
                if (dirArray[index]== BIBLE_DICTIONARY_DB_NAME) {
                    found = YES;
                    imagePath = getFullPathOfUserDocumentsCWPDictionaryDirectory() + "/" +"/ismith/";
                    break;
                }
            }
        }

        if (found){
             DictionaryDBName = getFullPathOfUserDocumentsCWPDictionaryDirectory() +"/"+ BIBLE_DICTIONARY_DB_NAME;
        } else {
            //----------------------------------------------------------------
            // where the user said the bibles would be (Preferences directory)
            dirArray = getDirectoryListing(getFullPathOfUserPrefsDictionaryDirectory());

            for (index = 0; index < dirArray.count(); index++)
                if (dirArray[index]==BIBLE_DICTIONARY_DB_NAME) {
                    found=YES;
                    imagePath = getFullPathOfUserPrefsDictionaryDirectory() + "/" +"/ismith/";
                    break;
                }

        }

        if (found){
             DictionaryDBName = getFullPathOfUserPrefsDictionaryDirectory() +"/"+ BIBLE_DICTIONARY_DB_NAME;
        } else {
            //----------------------------------------------------------------
            // now look in the bundle directory
            dirArray = getDirectoryListing(getFullPathOfAppBundleCWPDictionaryDirectory());
            //dirArray = getDirectoryListing(getiTunesDocumentStorageDirectory()+"/"+DICTIONARIES_DIRECTORY);

            for (index = 0; index < dirArray.count(); index++)
                if (dirArray[index]==BIBLE_DICTIONARY_DB_NAME) {
                    found=YES;
                    imagePath = getFullPathOfAppBundleCWPDictionaryDirectory() + "/" +"/ismith/";
                    break;
                }

            if (found)
                DictionaryDBName =getFullPathOfAppBundleCWPDictionaryDirectory()+"/"+BIBLE_DICTIONARY_DB_NAME;
        }


        if (found && DictionaryDBName.length()>0) {
            gBibleDictionaryDB = new dictionaryDB();
            gBibleDictionaryDB->initWithDBName( DictionaryDBName,/* andType*/TYPE_DICTIONARY_TYPE, /*andDictionaryType:*/DICTIONARY_TYPE_BIBLE_DICTIONARY);
        } else {
            QString StorageLoc;
            StorageLoc = getFullPathOfUserDocumentsCWPDictionaryDirectory()+"/"+BIBLE_DICTIONARY_DB_NAME;
            MsgBox("Could not find " BIBLE_DICTIONARY_DB_NAME ", Please fix.  Expected in \n '"+StorageLoc+"'");
        }
}
    // ------------------------------------------------------------------------
    // end of Smith's
    // ------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::processLink() {

    webTimer.stop();

    QString  lchapter="1";
    QString  lverse="1";
    QString  BibleNumber;
    QString  BibleType;
    QString BOOKNumber ;

checkUrlString:
        if (Instr(urlString,"_"))
        {
            if (Instr(urlString,"http://") || Instr(urlString, "file://")) {
                urlString = Right(urlString, 7);// remove http://
                while (Mid(urlString,0,1)=="/") {
                    urlString = Right(urlString,1);
                }

                while (Mid(urlString,urlString.length()-1,1)=="/")
                   urlString = urlString.left(urlString.length()-1);
            }

            QString Number;
            QStringList array;
            array = mySplit(urlString,"_");
            if (array.count()) {

                QChar c=0;

                Number = array[0];
                if (array[0].length())
                    c = Number.at(0).toUpper().toLatin1();

                if (c == 'X') {
                    if (Number.at(1) == 'n') {
                        Number = myReplace(Number,"xn--","");
                        Number = Number.toUpper();

                        array[0]=Number;
                        c = Number.at(0);
                    }
                }


                switch (c.toLatin1()) {

                    case 'D':
                    EnglishDictionary:
                        // english dictionary
                        if (array.count()>1) {
                            //[self startWaitIndicator];

                            dictionarySearchWord = array[1];
                            if (Instr(dictionarySearchWord,"[") || Instr(dictionarySearchWord,"]")){
                                lookUpWordAndShowOnWebPage(dictionarySearchWord);
                                return;
                            }
                            showDictionary();
                           // handleElementID(array[2]);

                            return ;
                        }
                        break;





                    case BOOKMARK_INDICATOR_DIRECT_C: // do a bookmark
                        if (array.count()>2) {
                        //    handleElementID(array[2]);
                        }
                        BOOKNumber= array[0];

                        array = mySplit(array[1], HTML_COLON_F);
                        if (array.count() >1) {
                            lchapter = array[0];
                            lverse = array[1];
                            BOOKNumber = BOOKNumber.right(BOOKNumber.length()-1);
                            //showBookMarkAdd(BOOKNumber, /*Chapter:*/lchapter, /*Verse:*/lverse);
                        }
                        break;

                    case FOOTNOTE_INDICATOR_C : {// show footnote
                        if (array.count()>4) {
                            //handleElementID(array[4]);
                        }

                        array = mySplit(urlString,"-");

                        // check array so we don't crash
                        if (array.count() < 3) {
                            break;
                        }

                        BibleNumber = array[1];
                        urlString  = array[2];

                        array = mySplit(urlString,"_");

                        if (array.count() == 0)// then
                            break;

                        //-------------------------------

                        Number = array[0];
                        lchapter = array[1];
                        lverse = array[2];
                        QString lSeqNumber=array[3];
                        //showFootNoteForBible(BibleNumber, /*Book:*/Number, /*Chapter:*/lchapter,/* Verse:*/lverse,/* SeqNumber:*/lSeqNumber);
                      }  break;

                    case COPYRIGHT_INDICATOR_SHOW_WINDOW_C: // copyright info was clicked
                                                            // check array so we don't crash
                        if (array.count() < 3) {
                            break;
                        }

                        BibleNumber = array[1];
                        BibleType = array[2];

                        if (array.count()>3) {
                            //handleElementID(array[3]);
                        }


                        //showCopyrightForBookNumber(BibleNumber, /*ofType:*/BibleType);
                        break;

                    case NOTES_INDICATOR_DIRECT_C: //NOTES_INDICATOR_DIRECT:
                    {
                        QString noteIndex;

                        Number = Right(array[0] ,1);
                        if (array.count()>2) {
                            //handleElementID(array[2]);
                        }

                        if (array.count()>3) {
                            noteIndex = array[3];
                        } else {
                            noteIndex = "0";
                        }

                        array = mySplit(array[1], HTML_COLON_F);
                        if (array.count()>1) {
                            lchapter = array[0];
                            lverse = array[1];
                            //showNotesWindowForNoteIndex(noteIndex.toInt(),/*Book:*/Number.toInt(),/* Chapter:*/lchapter.toInt(),/* Verse:*/lverse.toInt());
                        }
                    }   break;


                    case SPEECH_INDICATOR_DIRECT_C:
                    {
                        QStringList   array2 = mySplit(array[0], "-");
                        if (array2.count()>3) {
                            Number = array2[3];
                            lverse = array[2];
                            lchapter = array[1];

                            //speakFromBible(array2[1].toInt(),/* Book:*/Number.toInt(), /*Chapter:*/lchapter.toInt(),/* Verse:*/lverse.toInt());
                            //     [self showNotesWindowForNoteIndex:[noteIndex intValue] Book:Number.toInt() Chapter:lchapter.toInt() Verse:lverse.toInt()];
                        }
                    }   break;

                  /*  case 'Z': // current Y location of the screen (scroll offset)
                    {
                        // check array so we don't crash
                        if (array.count() < 2) {
                            break;
                        }

                        Number=  array[1];
                        fvcp->currentScrollPosition = Number.toInt();
                        setCurrentVerseByScrollPosition(fvcp->currentScrollPosition);
                    }
                        break; */
                case CHAPTER_XREF_C://'C'
                        int n;
                        Number = Right(array[0] ,1);
                        n = Number.toInt();
                        if (n== 0) {
processBookName:
                            extern BibleBookInfoStructType getBibleBookInfoByName(QString b);
                            BibleBookInfoStructType bis =  getBibleBookInfoByName(Number);
                            Number = QString::number(bis.BookNumber);
                            goto processChapterXRefC;
                        } else {
                            if ((n==1 || n==2 || n == 3)&& array.count()==3 ){
                                Number = Number + " "+array[1];
                                array.removeFirst(); // pull off the book #, i.e. 1 Corinthians
                                goto processBookName;
                            }
                        }
                #if FALLTHROUGH_ALLOWED == 1
                [[fallthrough]];
                #endif
                case USER_XREF_C://'U' // a reference in a user's notes
                        Number = Right(array[0] ,1);
processChapterXRefC:
                        array = mySplit(array[1], HTML_COLON_F);
                        if (array.count()>1) {
                            lchapter = array[0];
                            lverse = array[1];
                            // sometimes verses have continuations in them. remove the continuation and look for the first verse
                            if (Instr(lverse,"-")) {
                                array = mySplit(array[1],"-");
                                lverse = array[0];
                            }
#ifdef WEBKIT_USED
                          //  fvcp->setCurrentVerseByScrollPosition(fvcp->currentScrollPosition=fvcp->getPageScrollOffset());
#endif
#ifdef WEBENGINE_USED
                            // will already have been done by now
                            //fvcp->setCurrentVerseByScrollPosition();
#endif
                            userHistory->pushHistoryItemWithBook(Preferences->getCurrentBook(),Preferences->getCurrentChapter(),Preferences->getCurrentVerse(),fvcp->currentScrollPosition);
                            fvcp->saveCurrentLocation( DONT_WRITE_LOCATION_TO_DISK );

                            Preferences->setCurrentBook( Number.toInt());
                            Preferences->setCurrentChapter(lchapter.toInt());
                            Preferences->setCurrentVerse( lverse.toInt());
                            // make sure we scroll to the exact verse
                            globalSTI.DoScrollToVerse = YES;
                            globalSTI.VerseToScrollTo = lverse.toInt();

                            fvcp->LoadWebPage();
//#ifdef Q_OS_WIN
                            if (savedURL.length()) {
                                // cause link to be processed again
                                urlString = savedURL;
                                savedURL="";
                                webTimer.start(50);

                            }
//#endif
                        }
                        break;

                    case '/':
                    {
                        c = array[0].at(1);


                        if (c == 'D') {
                            goto EnglishDictionary;
                        }


                        if (c == CHAPTER_XREF_C) {
                            extern BibleBookInfoStructType getBibleBookInfoByName(QString b);

                            QString bookName;
                            bookName = Right(array[0], 2);

                            if (bookName.toInt()>0) {
                                if (bookName.length()==1) {
                                    bookName = bookName+array[1];
                                    array.removeAt(1);// we don't need this one any more
                                }
                            }

                            // this is a vector using a short book name
                            BibleBookInfoStructType bis =  getBibleBookInfoByName(bookName);
                            // Preferences.currentBook = bis.BookNumber;

                            // if the booknumber is -1, then this might be a problem with the
                            // offset into the array.  let's try with the next item in the
                            // array, if the array count is > 1
                            if (bis.BookNumber == -1) {
                                if (array.count()>1) {
                                    bookName = array[1];
                                    bis = getBibleBookInfoByName(bookName);
                                    if (bis.BookNumber==-1) {
                                        break; // we could not figure it out, don't do anything
                                    }
                                }
                            }

                            if (array.count()>1) {

                                if (array.count()== 4) {
                                    array = mySplit(array[3],HTML_COLON_F);
                                } else {
                                    array = mySplit(array[1], HTML_COLON_F);
                                }

                                if (array.count()>1) {
                                    lchapter = array[0];
                                    lverse = array[1];

                                    fvcp->setCurrentVerseByScrollPosition();

                                    userHistory->pushHistoryItemWithBook(Preferences->getCurrentBook(),Preferences->getCurrentChapter(),Preferences->getCurrentVerse(),fvcp->currentScrollPosition);
                                    fvcp->saveCurrentLocation(DONT_WRITE_LOCATION_TO_DISK );

                                    Preferences->setCurrentChapter( lchapter.toInt());
                                    Preferences->setCurrentBook(bis.BookNumber);
                                    Preferences->setCurrentVerse( lverse.toInt());
                                    // make sure we scroll to the exact verse
                                    globalSTI.DoScrollToVerse = YES;
                                    globalSTI.VerseToScrollTo = Preferences->getCurrentVerse();
                                }
                                fvcp->LoadWebPage();
                            }
                        }
                    }   break;

                    default:
                        break;
                }

            }

          return;  //not ok
        } else {
            if (Instr(urlString,"%3A"))
                urlString = myReplace(urlString,"%3A",HTML_COLON_F);
            // see if this is a previous version's LINK to another book/chapter
            if (Instr(urlString,"+")) {
                urlString = myReplace(urlString,"+","_");
                goto checkUrlString;
            }
        }

        return ;//ok


}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void BibleDictionaryDialog::updateDisplayWithSearchWordDefinition() {
    webTimer.stop();

    if (!gBibleDictionaryDB) {
      openBibleDictionary();
    if (!gBibleDictionaryDB)
        return;
    }

    QString vp;
    int numDefinitions = 10;

    if (bibleDictionarySearchWord.length()==0) {
        bibleDictionarySearchWord =  "0";
        numDefinitions = 1;
    }


    if (Instr(bibleDictionarySearchWord, "%20")) {
        bibleDictionarySearchWord = myReplace(bibleDictionarySearchWord,"%20"," ");
    }

    vp = gBibleDictionaryDB->getDefinitionsForWord(bibleDictionarySearchWord,/* limitedToNumberOfDefinitions:*/numDefinitions);

    if(vp.length())
        loadWebPageWithString(vp);
}


//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::webPageLoadStarted(){
#ifdef WEBKIT_USED
    //loadStarted++;
   static QUrl newUrl= webView->url();
   QString s ;//= newUrl.toString();
   QWebPage *p = webView->page();
   QWebFrame *wf= p->currentFrame();

   s = wf->baseUrl().toString();
  // s = wf->url().toString();
   if (s== QString("about:blank")) {
       // this "stop" worked in this location (webPageLoadStarted)
      //webView->stop();
      return;
   }
#endif
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::webPageFinishedLoading(bool b) {
    Q_UNUSED(b);

    // ------------------------------------------------------------------------
    // have the java script interpreter on the page scroll to position 0
    // ------------------------------------------------------------------------
    #ifdef WEBKIT_USED
        webView->page()->currentFrame()->evaluateJavaScript("scrollTo(0,0);");
    #endif
    #ifdef WEBENGINE_USED
        webView->page()->runJavaScript("scrollTo(0,0);");
    #endif
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::webPageURLChanged(const QUrl &url) {
#ifdef WEBKIT_USED
    //
    QString s = url.toString();
    QWebPage *p = webView->page();
    QWebFrame *wf= p->currentFrame();

   // s = wf->baseUrl().toString();
    s = wf->url().toString();

// we get the "correct" url here
    if (s== QString("about:blank")) {
    //   webView->stop();
       return;
    }
#endif
    Q_UNUSED(url)
        //urlChanged++;
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::cleanUp() {
    // do a hide() and save the current window information
    QRect r = geometry();
    QString s = QString::number(r.x()) + ","+QString::number(r.y()) + "," + QString::number(r.width()) +","+QString::number(r.height());
    Preferences->setBibleDictionaryGeometry(s);

   // if (gBibleDictionaryDB)
   //     gBibleDictionaryDB->Close();
    delete gBibleDictionaryDB;
    bibleDictionaryWindowPtr=nullptr;
    gBibleDictionaryDB=nullptr;
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::on_pbClose_clicked()
{
    cleanUp();
    close();
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::on_pbSearch_clicked()
{
    Preferences->setLastBibleDictionarySearchText(ui->leSearchText->text());
   lookUpWordAndShowOnWebPage(ui->leSearchText->text());
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::on_toolButtonLeft_clicked()
{
    // left
    doDataIndexBasedLoadWebPage(YES);

}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
void BibleDictionaryDialog::on_toolButtonRight_clicked()
{
    // right
    doDataIndexBasedLoadWebPage(NO);
}
