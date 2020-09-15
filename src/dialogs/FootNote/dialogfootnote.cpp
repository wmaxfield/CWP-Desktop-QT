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
*/#include "dialogfootnote.h"
#include "ui_dialogfootnote.h"
#include "bookmanipulationlibrary.h"
#include "bookmanipulationclass.h"
#include "bookpreferencesdbclass.h"

extern BookManipulationClass *bmc;
extern BookPreferencesDBClass *gTextBookPreferences;

DialogFootNote::DialogFootNote(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFootNote)
{
    ui->setupUi(this);

#ifdef WEBKIT_USED
    webView = new QWebView(this);
#endif
#ifdef WEBENGINE_USED
    webView = new QWebEngineView(this);
#endif
    webView->setObjectName(QStringLiteral("webView"));
    webView->setGeometry(QRect(0, 0, 401, 251));
    webView->setUrl(QUrl(QStringLiteral("about:blank")));

    bibleNumber
        = bookNumber
        = chapter
        = verse
        = sequenceNumber
        = 0;
    // we aren't showing books yet
    isBook=false;


    this->setAttribute(Qt::WA_DeleteOnClose);// delete after close

}
void DialogFootNote::showEvent(QShowEvent *) {
    if (isBook) {
            if (!bmc) {
               // bmc = [[BookManipulationClass alloc] init];
                bmc = new BookManipulationClass();
            }
        FootNoteText = bmc->getFootNoteForBook(bibleNumber,chapter,verse,sequenceNumber);
//            FootNoteText = [bmc getFootNoteForBook:bibleNumber andChapter:chapter andVerse:verse andFootNoteNumber:sequenceNumber];

            this->setWindowTitle("Footnote");
        } else {
            BookHandlerClass *bd;
            BibleDatabaseClass *bbld;

            bd = Books[Preferences->SelectedBooks[bibleNumber].Index];
//            bd = [Books objectAtIndex:Preferences.SelectedBooks[bibleNumber].Index];
            bbld = (BibleDatabaseClass *)bd->Database;
            FootNoteText = bbld->getFootNoteTextFromBook(bookNumber,chapter,verse,sequenceNumber);
//            FootNoteText = [bbld getFootNoteTextFromBook:bookNumber Chapter:chapter Verse:verse SeqNumber:sequenceNumber];
           // self.title = [NSString stringWithFormat:@"Footnote for Chapter %d Verse %d",chapter,verse];
            this->setWindowTitle("Footnote for Chapter"+QString::number(chapter)+" Verse "+QString::number(verse));
        }


        webView->setHtml(FootNoteText,QUrl::fromLocalFile("/"));
       // [webviewFootNote loadHTMLString:FootNoteText baseURL:[NSURL URLWithString:@""]];


}

DialogFootNote::~DialogFootNote()
{
    delete ui;
}

void DialogFootNote::on_pbOK_clicked()
{
    close();;
}
