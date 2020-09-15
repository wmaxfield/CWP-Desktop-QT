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
//  StrongsModule.m
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 10/1/10.
//  Copyright 2010 Wade Maxfield. All rights reserved.
//converted

#include <QString>
#include "strongsmodule.h"
#include "HTMLModule.h"
#include "WebDefinitionModule.h"
#include "misclibrary.h"
#include "preferencesmodule.h"
#include "structs.h"
#include "bibleinfo.h"

#include <QStringList>

QString CreatePhonicTag( StrongsStructure *si)
{
    QString s="";
    QString s2;

    // build the speech href tag if speech is enabled
    if (Preferences->getSpeechEnabled()) { // Then
        s2 = si->Phonic;

        s2 = myReplace(s2,"'-","'");
        s2 = myReplace(s2,"-os","os");

        s =  SUPstart HTML_HREF_STARTQ SPEECH_INDICATOR_DIRECT +s2+ QUOTE HTML_GENERIC_END_TAG HTML_HREF_END SUPend;
        //s=s+ SUPstart+HTML_HREF_START+QUOTE+SPEECH_INDICATOR_DIRECT "/"
        // s = s + "/"+s2

        //   s = s + QUOTE+HTML_GENERIC_END_TAG
        //  s = s + SPEECH_INDICATOR_DIRECT + HTML_HREF_END+SUPend
    }

    return s;

}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
QString RemoveAllPunctuation(QString s)
{
    QString s2;
    QString s3="";

    s2 = myReplace(s,":",s3);
    s2 = myReplace(s2,"?",s3);
    s2 = myReplace(s2,";",s3);
    s2 = myReplace(s2,",",s3);
    s2 = myReplace(s2, QUOTE,s3);
    s2 = myReplace(s2,"'",s3);
    s2 = myReplace(s2,"!",s3);
    s2 = myReplace(s2,".",s3);
    s2 = myReplace(s2,"-",s3);
    s2 = myReplace(s2,"‘",s3);
    s2 = myReplace(s2,"–",s3);
    s2 = myReplace(s2,")",s3);
    s2 = myReplace(s2,"(",s3);
    s2= myReplace(s2,"[",s3);
    s2= myReplace(s2,"]",s3);
    s2 = myReplace(s2,"%5D",s3);
    return s2;
}


//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
QString GetStrongsHTML(StrongsStructure *si)
{
    // return an HTML page from the StrongsStructure

    QString OrigWord;//Dim OrigWord As  String
    QString StrongsHTML;// Dim StrongsHTML As String
    QString DictionaryStart  = "<a style=\"color:black\" href=\"" HTML_DICT_INDICATOR "_";

/*    OrigWord = str.sprintf("%s", si->OriginalWord];
    if (Instr(OrigWord, "&#")>0) {
        const char *s = [OrigWord UTF8String];
        char *dest = calloc(1,[OrigWord length]);
        int len;

       len = changeHTMLtoUTF8(dest, s);
        dest[len]=0;
        OrigWord = [NSString stringWithUTF8String: dest];
        OrigWord = [OrigWord stringByAppendingString:  HTML_BRStr ];
        free(dest);

        StrongsHTML = str.sprintf(HTML_CENTER_START "%" HTML_BRStr "%s%" HTML_CENTER_END HTML_BRStr ,OrigWord,si->Phonic,CreatePhonicTag(si)];


    } else {
    }
*/

    StrongsHTML = HTML_CENTER_START +QString(si->OriginalWord) +HTML_BRStr +QString(si->Phonic) +CreatePhonicTag(si)+ HTML_CENTER_END ;




    OrigWord = myReplace( QString(si->Definition), FROM_STRONGS_GREEK, HTML_BRStr HTML_BRStr FROM_STRONGS_GREEK);

    OrigWord = myReplace(OrigWord, HTML_BRStr," <br/> ");// make split below always work
    OrigWord = myReplace(OrigWord, ")", " )");


    //---------------------------------------------------------------------------------------------------------------------------------------
    // handle dictionary
    //---------------------------------------------------------------------------------------------------------------------------------------
    QStringList Sentence;//dim Sentence(-1) As String
    QStringList words;//dim words(-1) As String
    QStringList misc;//dim misc(-1) As String
    int index,index2;//dim index,index2 As Integer
    //static DictionaryStart As String
    QString s,s2,sentence;// dim s As String
    QString str;
    BOOL found=NO; //dim found As Boolean
    int limit;

    Sentence =  mySplit(OrigWord," ");
    limit = Sentence.count()-1;

   // words = [NSMutableArray arrayWithCapacity:limit+10];// give padding just in case


    for (index = 0; index <= limit; index++) {

        sentence =Sentence[index];

        if ((Instr("0123456789", Left(sentence,1)) > 0)) { // Then

            if (Instr(sentence, ":")) {
                goto NotABareNumber;
            }

            // this is a strong's number
            s = RemoveAllPunctuation(sentence);
            misc = mySplit(s,"<br/>");
            s = misc [0];

            if (index >= 2) { // Then
                for( index2 = index ; index2 >= index-2 ; index2--) {//step -1
                    s2 =Sentence[(index2)];
                    if (StringsAreEqual(s2, "hebrew")) { // Then
                        s =  LANGUAGE_HEBREW + s;
                        found = YES;
                        break;
                    }

                    if ( StringsAreEqual( s2 , "greek")) { // Then
                        s = LANGUAGE_GREEK+ s;//s = LANGUAGE_GREEK+s;
                        found = YES;
                        break;
                    }
                }
            }

            if (!found) { // Then
                s =   si->Language+s; // add the current language marker
            }

            found = NO;

           // s2 = str.sprintf("<a style=\"color:#%02x%02x%02x\" href=\"http://" STRONGS_INTERNAL_XREF
            s2 = "<a href=\"http://" STRONGS_INTERNAL_XREF;
                         // ,Preferences->red,Preferences->green,Preferences->blue);
            s2 = s2 + "_"+s+ HTML_GENERIC_END_TAGQ +sentence+ HTML_HREF_END;
            words.append(s2);
            //[s2 release];
            //                    words.Append "<a style="+QUOTE+"color:black"+QUOTE+"href="+QUOTE+STRONGS_INTERNAL_XREF+"_"+s+HTML_GENERIC_END_TAGQ+Sentence(index)+HTML_HREF_END
        }// if instr
        else {
        NotABareNumber:
            if (( StringsAreEqual( Left(sentence,1) , "H") || StringsAreEqual(Left(sentence,1),"G") )&& Instr("0123456789",Mid(sentence,1,1)) > 0 ) { // Then

                s = RemoveAllPunctuation(Sentence[(index) ]);
                str =str.sprintf( "<a style=\"color:#%02x%02x%02x\" href=\"http://" ,Preferences->red,Preferences->green,Preferences->blue);
                s2 =str+STRONGS_INTERNAL_XREF  +"_"+s+ HTML_GENERIC_END_TAGQ+ Sentence[index]+  HTML_HREF_END;

                words.append(s2);
                //[s2 release];
            }// strings are equal
            else
            {
                s = Sentence [(index)]; // wsm this was missing

                if (globalSTI.enableEnglishDictionary) { // Then
                    if (Instr(s,"<br/>") > 0) { // Then
                        misc = mySplit(s,"<br/>");

                        if (misc.count() > 2) { // Then
                            for (index2 = 0; index2 < misc.count(); index2++) {
                                s2 = DictionaryStart + RemoveAllPunctuation(misc[2]) + "\""+HTML_GENERIC_END_TAG+ HTML_HREF_END+misc[2]+" ";

                                words.append(s2);
                                //[s2 release];

                                if (index2 < misc.count()-1){
                                    words.append(HTML_BRStr);
                                }

                            } // for index2
                        }// misc count
                        else {
                            words.append(Sentence[index]);
                        }// misc count else
                    }// instr
                    else {
                        s2 = str.sprintf("%ls%ls\"" HTML_GENERIC_END_TAG "%ls" HTML_HREF_END, (wchar_t*)DictionaryStart.utf16(), (wchar_t*)RemoveAllPunctuation(sentence).utf16(),(wchar_t*)sentence.utf16());
                        words.append(s2);
                        // not needed, due to reference count being one at end of event loop for s2 [s2 release];
                    }//instr else
                }// showdictionary window
                else {
                    words.append(Sentence[index]);
                }

            }// else stringsarequal left


        }// else
    }// for index

    OrigWord = words.join(" ");
    OrigWord = myReplace(OrigWord," )",")"); // remove the formatting used to provide good splits around the ")"
   // [words release];


    //---------------------------------------------------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------------------------

    OrigWord = myReplace(OrigWord,"--", HTML_BRStr "=> " HTML_SPACE);

    int fontsize =Preferences->getFontSize();
    fontsize *= 0.7;
    if (fontsize<10) {
        fontsize+=2;
    }

    StrongsHTML =   HTML_STANDARD_HEADER "<STYLE><!--\ra {text-decoration:none}//--></STYLE>" "<center><font style=\"font-size: "+QString::number((int)(fontsize*1.5))+
            "pt\">"+StrongsHTML+"</font></center>" "<p style=\"font-size: "+QString::number(fontsize)+
            "pt\"" HTML_GENERIC_END_TAG +OrigWord+   "</p>" HTML_STANDARD_END;

    return StrongsHTML;
}




