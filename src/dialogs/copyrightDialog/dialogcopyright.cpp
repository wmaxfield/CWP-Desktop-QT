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
*/
#include "dialogcopyright.h"
#include "ui_dialogcopyright.h"
#include "structs.h"
#include "preferencesmodule.h"
#include "bookhandlerclass.h"
#include "notesdbclass.h"
#include "WebEngineViewWebPage.h"

extern QList<BookHandlerClass *>Books;

//------------------------------------------------------------
//------------------------------------------------------------
DialogCopyright::DialogCopyright(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCopyright)
{
    ui->setupUi(this);
#ifdef WEBKIT_USED
    webView = new QWebView(this);
#endif
#ifdef WEBENGINE_USED
    webView = new QWebEngineView(this);
    webView->setPage(new MyWebPage());
    //connect(webView->page(),SIGNAL(linkClicked(QUrl)),this,SLOT(link_click(const QUrl &)));

#endif
    webView->setObjectName(QStringLiteral("webView"));
    webView->setGeometry(QRect(10, 10, 556, 321));
    webView->setUrl(QUrl(QStringLiteral("about:blank")));
    //-----------------------------------------------------------
    // clean up memory after close
    //-----------------------------------------------------------

    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close

}

//------------------------------------------------------------
//------------------------------------------------------------
DialogCopyright::~DialogCopyright()
{
    delete ui;
}

// todo finish copyright dialog
void DialogCopyright::showEvent(QShowEvent *){

    BookHandlerClass *bd;
    BibleDatabaseClass *bbld;

    switch (bookType) {
        case TYPE_BBL_TYPE:
        case TYPE_ABP_TYPE:
        case TYPE_MORPHOLOGY_TYPE:
            bd = Books[Preferences->SelectedBooks[bookNumber].Index];
            bbld = (BibleDatabaseClass *)bd->Database;

            sCopyrightText = bbld->getCopyrightText();

            break;

        case TYPE_COMMENTARY_TYPE:
        {
          bd = Books[bookNumber];
          bbld =(BibleDatabaseClass *) bd->Database;
            if (bbld) {
                sCopyrightText= bbld->getCopyrightText();
            }


            if (sCopyrightText.length()==0){
                sCopyrightText = "Public domain in the United States.";
            }

        }   break;

        case TYPE_NOTES_TYPE:
        {
            extern NotesDBClass *gNotes[MAX_NUM_BIBLES];

            bbld = gNotes[bookNumber];
            sCopyrightText= bbld->getCopyrightText();

            if (sCopyrightText.length()==0){
                sCopyrightText = "Public domain in the United States.";
            }
        }
            break;

        case TYPE_PICTURE_INFO_TYPE: // copyright text is expected to already be set
            break;

        default:
            break;
    }

    if (sCopyrightText.length()==0) {
        sCopyrightText = "No copyright information is available for this text.";
    }

    webView->setHtml(sCopyrightText,QUrl::fromLocalFile("/"));
    this->setWindowTitle("Copyright Information");

}

//------------------------------------------------------------
//------------------------------------------------------------
void DialogCopyright::on_pbOK_clicked()
{
    close();
}
