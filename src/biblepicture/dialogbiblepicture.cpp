#include <QListWidget>
#include <QDir>
#include "dialogbiblepicture.h"
#include "ui_dialogbiblepicture.h"
#include "cwpdefines.h"
#include "MediaModuleDatabase.h"
#include "WebEngineViewWebPage.h"

//------------------------------------------------------------
//------------------------------------------------------------
DialogBiblePicture::DialogBiblePicture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBiblePicture)
{
    ui->setupUi(this);

#ifdef WEBKIT_USED
    webView = new QWebView(this);
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);// or DelegateAllLinks
    //connect(webView->page() , SIGNAL( linkClicked(const QUrl &)), this, SLOT(link_click(const QUrl &)));
#endif
#ifdef WEBENGINE_USED
    webView = new QWebEngineView(this);
    webView->setPage(new MyWebPage());
    //connect(webView->page(),SIGNAL(linkClicked(QUrl)),this,SLOT(link_click(const QUrl &)));

#endif
    webView->setObjectName(QStringLiteral("webView"));
    webView->setGeometry(QRect(160, 65, 711, 520));
    webView->setStyleSheet(QStringLiteral(""));
    webView->setUrl(QUrl(QStringLiteral("about:blank")));


    mainwindowHeight = this->height();
    mainwindowWidth = this->width();
    rs = 0;
    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close
    pidb = new PictureInfoDatabase();

    this->setWindowTitle( tr("Image Viewer"));

    //ui->listWidget->setStyleSheet("QListWidget::item { border-bottom: 1px solid gray; border-left: 10px solid white; border-right: 10px solid white; border-top: 10px solid white; } ");

}
//------------------------------------------------------------
// overrides resizeEvent to allow button movement
//------------------------------------------------------------
void DialogBiblePicture::resizeEvent ( QResizeEvent * event ) {

    Q_UNUSED(event);

    QRect r = ui->listWidget->geometry();
    int newx,newy,resizex,resizey;
    newx = this->width();
    newy = this->height();
    resizex = newx - mainwindowWidth;
    resizey = newy - mainwindowHeight;
    r.setHeight(resizey + r.height());

    ui->listWidget->setGeometry(r);



    r = ui->pbClose->geometry();
    int width = r.width();
    int height = r.height();
    r.setX(r.x()+resizex/2);
    r.setY(r.y()+resizey);
    r.setHeight(height);
    r.setWidth(width);
    ui->pbClose->setGeometry(r);



            r = ui->labelPictureInfo->geometry();
             width = r.width();
             height = r.height();
            //r.setX(r.x()+resizex);
            //r.setY(r.y()+resizey);
            r.setHeight(height);
            r.setWidth(width+ resizex);
            ui->labelPictureInfo->setGeometry(r);


    r = ui->labeTip->geometry();
     width = r.width();
     height = r.height();
    r.setX(r.x()+resizex);
    r.setY(r.y()+resizey);
    r.setHeight(height);
    r.setWidth(width);
    ui->labeTip->setGeometry(r);


     r = webView->geometry();
    newx = this->width();
    newy = this->height();
    r.setWidth(resizex +r.width());
    r.setHeight(resizey + r.height());

    webView->setGeometry(r);

    // newy - 50;
    mainwindowWidth = newx;
    mainwindowHeight = newy;
    //activityIndicator->stopAnimation();

}
//------------------------------------------------------------
//------------------------------------------------------------
void DialogBiblePicture::showEvent(QShowEvent *){

    // load the picture
    reloadData();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogBiblePicture::reloadData() {
    ui->listWidget->clear();

    //int index;
    //int Count = rs->NumberOfRows;
    QString t,t2;
    QStringList a;
    QString cvs;
    QString Strnu="";
    QString lastStrnu="1";
    QString lastMenuName;

    rs->MoveFirst();
    while (!rs->atEOF()) {

        Strnu = rs->Field(STRONGS_NUMBER_FIELD_NAME);
        cvs = rs->Field(MENU_TEXT_FIELD_NAME);

        if (Strnu == lastStrnu && cvs == lastMenuName) {
            rs->MoveNext();
            continue;
        }
        lastStrnu = Strnu;
        lastMenuName = cvs;

        QListWidgetItem* pItem =new QListWidgetItem();
        pItem->setSizeHint(QSize(0,89));// ??
        ui->listWidget->addItem(pItem);

        QLabel *label = new QLabel(ui->listWidget);




       // t = "<font size=\"+1\" face=\"helvetica\" color=\"dark blue\"><b>&nbsp;&nbsp;&nbsp;&nbsp;";
        //t = t +  info.BookName + " "+QString::number(cvs.Chapter)+":"+QString::number(cvs.Verse);
       // t = t + "</b></font><br /><br /><font face=\"helvetica\"  color=\"purple\">&nbsp;&nbsp;";
        //t = t + t2.left(300);//t2.left(100);

        //This is the text.  ";
        //t = t + "This is the second part of the text how long is it and how many lines of label does it occupy.  Let's make it a little longer....</font>";
        label->setText(cvs);
        label->setWordWrap(true);
        label->setEnabled(false);
        //t = QString::number(cvs.BookNumber)+":"+QString::number(cvs.Chapter)+":"+QString::number(cvs.Verse)+":"+QString::number(cvs.Posn);
        t = rs->Field(FILENAME_FIELD_NAME);
        t = t + "===" + rs->Field(DB_FILENAME_FIELD_NAME) ;

        pItem->setData(Qt::UserRole,t);

        ui->listWidget->setItemWidget(pItem,label);
        rs->MoveNext();
    }
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
DialogBiblePicture::~DialogBiblePicture()
{
    delete ui;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogBiblePicture::on_pbClose_clicked()
{
    close();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void DialogBiblePicture::on_listWidget_clicked(const QModelIndex &index)
{
        Q_UNUSED(index);

    QListWidgetItem *itemPtr = ui->listWidget->currentItem();
    QString t,t1;

    t = itemPtr->data(Qt::UserRole).toString();
    QStringList l;

    l = t.split("===");
     t1 = gMediaModuleDatabase->pathComponent;

    t1 = t1 + MEDIA_PICTURES_SUBDIR"/"+l[0];
    // now load the file into the web page

    //webView;
    QString w ;
   // t1.replace(QString("/"),QString("\\\\"));
//    w =  "<img src=\"file://"+ t1 +"\" />";
    // wsm -- the following worked on windows.  I suspect also linux.
    // file:// must be file:///
    w =  "<img src=\"file:///"+ t1 +"\" />";
    w= "<html><head><content=\"width=device-width\"/><style> \
           img{max-width:100%;max-height:device-height;} \
           </style> \
           </head><body style=\"color:black\"> " +w;
    /*w = w + "<script text = \"javascript\">\
            \
            window.onresize = function(event) {\
            \
                var newWidth = window.innerWidth;\
                var newHeight = window.innerHeight;\
            \
                // Code to adjust your contents..\
            }\
            \
            </script>";
    */
    //w = w + "<img src=\"file://"+ t1 +"\" />";
    w = w + "</body></html>";
    webView->setHtml("<html><head></head><body></body></html>");
    webView->setHtml(w ,QUrl::fromLocalFile("/"));
    webView->setZoomFactor(4.0);

    pidb->databaseName = l[1];
    pidb->pathComponent = gMediaModuleDatabase->pathComponent +MEDIA_PICTURES_SUBDIR"/";

    pidb->Open();
    QString Query = "select " COPYRIGHT_FIELD_NAME ", " DESCRIPTION_FIELD_NAME " from Info ";
    RecordSet *rs;
    rs = pidb->SQLSelect(Query);
    if (rs->NumberOfRows) {
        rs->MoveFirst();
        w = rs->Field(COPYRIGHT_FIELD_NAME) + "\n" + rs->Field(DESCRIPTION_FIELD_NAME);
    } else {
        w = "No info found for this image.";
    }
    //   w = l[1] + "\n" + l[2];
    ui->labelPictureInfo->setText(w );
    pidb->Close();
}
