#ifndef HTMLMODULE_H
#define HTMLMODULE_H
/*
 *  HTMLModule.h
 *  CrosswordTry2
 *
 *  Created by WADE MAXFIELD on 9/22/10.
 *  Copyright 2010 Wade Maxfield. All rights reserved.
 *
 */



#define ASCII_COLON ":"
#define BLANK_LINES_24 "<br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br />"

#define CUSTOM_MENU_HREF_INTERNAL "class=\"menuanchorclass\" rel=\"anylinkmenu1\" "
#define CUSTOM_MENU_HREF_INTERNAL1 "class=\"menuanchorclass\" rel=\"anylinkmenu1\" "
#define CUSTOM_MENU_LINK "<p><a href=\"http://www.dynamicdrive.com\" class=\"menuanchorclass\" rel=\"anylinkmenu1\">Default Example</a></p>\n\n\n<!--2nd anchor link-->\n\n<p><a href=\"http://www.dynamicdrive.com\" class=\"menuanchorclass someotherclass\" rel=\"anylinkmenu2[click]\" rev=\"lr\">Right dropping menu</a> (click to reveal)</p>\n"
#define CUSTOM_MENU_LINK_AUTO "<a href=\"http://www.dynamicdrive.com\" class=\"menuanchorclass\" rel=\"anylinkmenu1\">Default Example</a>\n"
#define CUSTOM_MENU_STYLE "<style>\n.selectedanchor{ /*CSS class that gets added to the currently selected anchor link (assuming it\'s a text link)*/\nbackground: yellow;\n}\n\n/* ######### Default class for drop down menus ######### */\n\n.anylinkmenu{\nposition: absolute;\nleft: 0;\ntop: 0;\nvisibility: hidden;\nborder: 1px solid black;\nborder-bottom-width: 0;\nfont: normal 12px Verdana;\nline-height: 18px;\nz-index: 100; /* zIndex should be greater than that of shadow\'s below */\nbackground: lightyellow;\nwidth: 200px; /* default width for menu */\n}\n\n.anylinkmenu ul{\nmargin: 0;\npadding: 0;\nlist-style-type: none;\n}\n\n.anylinkmenu ul li a{\nwidth: 100%;\ndisplay: block;\ntext-indent: 3px;\nborder-bottom: 1px solid black;\npadding: 2px 0;\ntext-decoration: none;\nfont-weight: bold;\ntext-indent: 5px;\n}\n\n.anylinkmenu a:hover{ /*hover background color*/\nbackground: black;\ncolor: white;\n}\n\n/* ######### Alternate multi-column class for drop down menus ######### */\n\n\n.anylinkmenucols{\nposition: absolute;\nwidth: 350px;\nleft: 0;\ntop: 0;\nvisibility: hidden;\nborder: 1px solid black;\npadding: 10px;\nfont: normal 12px Verdana;\nz-index: 100; /*zIndex should be greater than that of shadow\'s below*/\nbackground: #E9E9E9;\n}\n\n.anylinkmenucols li{\npadding-bottom: 3px;\n}\n\n.anylinkmenucols .column{\nfloat: left;\npadding: 3px 8px;\nmargin-right: 5px;\nbackground: #E0E0E0;\n}\n\n.anylinkmenucols .column ul{\nmargin: 0;\npadding: 0;\nlist-style-type: none;\n}\n\n\n\n/* ######### class for shadow DIV ######### */\n\n\n.anylinkshadow{ /*CSS for shadow. Keep this as is */\nposition: absolute;\nleft: 0;\ntop: 0;\nz-index: 99; /*zIndex for shadow*/\nbackground: black;\nvisibility: hidden;\n}\n</style>\n"
#define CUSTOM_MENU_TEXT "<script type=\"text/javascript\">\nvar anylinkmenu1={divclass:\'anylinkmenu\', inlinestyle:\'\', linktarget:\'\'} //First menu variable. Make sure \"anylinkmenu1\" is a unique name!\nanylinkmenu1.items=[\n\t[\"Dynamic Drive\", \"http://www.dynamicdrive.com/\"],\n\t[\"CSS Drive\", \"http://www.cssdrive.com/\"],\n\t[\"JavaScript Kit\", \"http://www.javascriptkit.com/\"],\n\t[\"Coding Forums\", \"http://www.codingforums.com/\"],\n\t[\"JavaScript Reference\", \"http://www.javascriptkit.com/jsref/\"] //no comma following last entry!\n]\n\n\n\nvar anylinkmenu2={divclass:\'anylinkmenu\', inlinestyle:\'width:150px; background:#FDD271\', linktarget:\'_new\'} //Second menu variable. Same precaution.\nanylinkmenu2.items=[\n\t[\"CNN\", \"http://www.cnn.com/\"],\n\t[\"MSNBC\", \"http://www.msnbc.com/\"],\n\t[\"Google\", \"http://www.google.com/\"],\n\t[\"BBC News\", \"http://news.bbc.co.uk\"] //no comma following last entry!\n]\n\n\n\nvar anylinkmenu3={divclass:\'anylinkmenucols\', inlinestyle:\'\', linktarget:\'secwin\'} //Third menu variable. Same precaution.\nanylinkmenu3.cols={divclass:\'column\', inlinestyle:\'\'} //menu.cols if defined creates columns of menu links segmented by keyword \"efc\"\nanylinkmenu3.items=[\n\t[\"Dynamic Drive\", \"http://www.dynamicdrive.com/\"],\n\t[\"CSS Drive\", \"http://www.cssdrive.com/\"],\n\t[\"JavaScript Kit\", \"http://www.javascriptkit.com/\"],\n\t[\"Coding Forums\", \"http://www.codingforums.com/\"],\n\t[\"JavaScript Reference\", \"http://www.javascriptkit.com/jsref/\", \"efc\"],\n\t[\"CNN\", \"http://www.cnn.com/\"],\n\t[\"MSNBC\", \"http://www.msnbc.com/\"],\n\t[\"Google\", \"http://www.google.com/\"],\n\t[\"BBC News\", \"http://news.bbc.co.uk\", \"efc\"],\n\t[\"News.com\", \"http://www.news.com/\"],\n\t[\"SlashDot\", \"http://www.slashdot.com/\"],\n\t[\"Digg\", \"http://www.digg.com/\"],\n\t[\"Tech Crunch\", \"http://techcrunch.com\"] //no comma following last entry!\n]\n\nvar anylinkmenu4={divclass:\'anylinkmenu\', inlinestyle:\'width:150px; background:#DFFDF4\', linktarget:\'_new\'} //Second menu variable. Same precaution.\nanylinkmenu4.items=[\n\t[\"CNN\", \"http://www.cnn.com/\"],\n\t[\"MSNBC\", \"http://www.msnbc.com/\"],\n\t[\"Google\", \"http://www.google.com/\"],\n\t[\"BBC News\", \"http://news.bbc.co.uk\"] //no comma following last entry!\n]\n</script>\n\n\n"
#define DARK_BLUE "#003366"
#define GreekXLateString "a&#945;b&#946;g&#947;d&#948;e&#949;z&#950;h&#951;u&#952;i&#953;k&#954;l&#955;m&#956;n&#957;j&#958;o&#959;p&#960;r&#961;s&#963;t&#964;y&#965;f&#966;x&#967;c&#968;v&#969;w&#962;"
#define HTML_BOLD_END "</b>"
#define HTML_BOLD_START "<b>"
#define HTML_BRStr "<br />"
#define HTML_CENTER_END "</DIV>"
#define HTML_CENTER_START "<DIV ALIGN=CENTER>"
#define HTML_CITE_END "</cite>"
#define HTML_CITE_START "<cite>"
#define HTML_COLON_F "QQQ"
#define HTML_END_OF_SPECIAL_CHAR ";"
#define HTML_GENERIC_END_TAG ">"
#define HTML_GENERIC_END_TAGQ "\">"
#define HTML_GT "&gt;"
#define HTML_HEADER2_ALIGN_CENTER "<h2 align=\"center\">"
#define HTML_HEADER2_END "</h2>"
#define HTML_HREF_END "</a>"
#define HTML_HREF_ENDS "</a> "
#define HTML_HREF_START "<a href="
#define HTML_HREF_STARTQ "<a href=\"http://"
//#define HTML_HREF_STARTQ "<a href=\""

#define HTML_IMG_START "<img"
#define HTML_ITALIC_END "</i>"
#define HTML_ITALIC_START "<i>"
#define HTML_LIST_END "</LI>"
#define HTML_LIST_OL "<OL>"
#define HTML_LIST_START "<LI>"
#define HTML_LT "&lt;"
#define HTML_NBSP "&nbsp;"
#define HTML_NO_UNDERLINE_STYLE "<STYLE><!--\ra {text-decoration:none; white-space: pre-wrap;}//--></STYLE>"
#define HTML_PARAGRAPH_END "</p>"
#define HTML_PARAGRAPH_START "<p>"
#define HTML_QUOTE "&quot;"
#define HTML_ROW_END "</tr>\n"
#define HTML_ROW_START "\n<tr>"
#define HTML_SMALL_HEADER_END "</h2>"
#define HTML_SMALL_HEADER_START "<h2>"
#define HTML_SPACE "&nbsp;"
#define HTML_STANDARD_END "</body></html>"
#define HTML_STANDARD_HEADER "<html><head><title></title><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN \"http://www.w3.org/TR/html4/loose.dtd\"></head><body>  "
#define HTML_TABLE_CELL_END "</td>\n"
#define HTML_TABLE_CELL_START "<td>"
#define HTML_TABLE_CELL_START_RIGHT_PAD "<td style=\"padding-right:%dpx; \">"

#define HTML_TABLE_CELL_START_RIGHT_LEFT_PAD "<td style=\"padding-right:%dpx; padding-left:%dpx;  \">"
#define HTML_TABLE_CELL_START_LEFT_PAD "\n<td style=\"padding-left:%dpx; white-space: pre-wrap;  \">"
// these two solve some wrapping problems with newer webkit: white-space: pre-wrap; word-wrap: break-word;

#define HTML_TABLE_CELL_START_LEFT_PAD_ABP "\n<td style=\"padding-left:%dpx;\">"
#define HTML_PREWRAP " white-space: pre-wrap; "

#define HTML_TABLE_END "</table>\n"
#define HTML_TABLE_HEADER_END "</th>"
#define HTML_TABLE_HEADER_START "<th>"
#define HTML_TABLE_START "\n<table>"
#define HTML_UNDERLINE_END "</u>"
#define HTML_UNDERLINE_START "<u>"
#define QUOTE "\""
#define WINDOWS_BACKSLASH "\\"


#define UN_SPAN "</span>"
#define HTML_SUPSTART "<sup>"
#define HTML_SUPEND   "</sup>"
#define COLOR_LIGHT_BLUE "#0000FF"



//------------------------------------------------------------------------------------------------------------------------
// from web definition module
//------------------------------------------------------------------------------------------------------------------------

#define GTItalicLT "<i>"
#define GTSlashItalicLT "</i>"
#define GTThan "<"

#define ONE_ROW_END "</onerow>"
#define ONE_ROW_START "<onerow>"

#ifndef STYLE_FONT
#define STYLE_FONT "<style>\rp\r{\rfont-size: 500%;\r}\r.unit {float:left;padding-top:13px;margin:.30em;padding-right:13px;}\r.strongs {font-size:smaller;}\r.strongs a{text-decoration:none;color:green;}\r.english {font-weight:bold;color:#17235c;}\rspan.strongs {display:block;}\rspan.greek {display:block;}\rspan.english {display:block;}\rspan.unit:hover {background-color: #ffffe8;}\rsup {font-size:smaller;vertical-align:baseline;position:relative;bottom:0.33em;}\r\r\r\r</style>"
#endif
#define SUPend "</sup>"
#define SUPstart "<sup>"
#define SUPstartCAP "<SUP>"
#define SUPendCAP "</SUP>"




#define BibleDictionaryCSS  "<style type=\"text/css\">\
p{ white-space:pre-wrap;}\
p.p1 {margin: 0.0px 0.0px 0.0px 0.0px; font: 12.0pt Times}\
p.p2 {margin: 0.0px 0.0px 0.0px 0.0px; font: 12.0pt Times; min-height: 14.0px}\
p.p3 {margin: 0.0px 0.0px 7.0px 0.0px; font: 12.0pt Times}\
p.p4 {margin: 0.0px 0.0px 7.0px 0.0px; font: 12.0pt Times; min-height: 14.0px}\
li.li1 {margin: 0.0px 0.0px 0.0px 0.0px; font: 12.0pt Times}\
span.s1 {font: 12.0pt 'Lucida Grande'}\
span.s2 {font: 10.0pt Times}\
span.s3 {text-decoration: underline}\
table.t1 {width: 767.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000}\
table.t2 {width: 746.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000}\
table.t1 {width: 767.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000}\
table.t2 {width: 746.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000}\
td.td1 {width: 351.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td2 {width: 120.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td3 {width: 135.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td4 {width: 121.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td5 {width: 105.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td6 {width: 407.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td7 {width: 59.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td8 {width: 45.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td9 {width: 46.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td10 {width: 139.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td11 {width: 399.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td12 {width: 66.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td13 {width: 47.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td14 {width: 362.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td15 {width: 86.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td16 {width: 125.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td17 {width: 133.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td18 {width: 518.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td19 {width: 84.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td20 {width: 44.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td21 {width: 60.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td22 {width: 245.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td23 {width: 246.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td24 {width: 144.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
td.td25 {width: 143.0px; border-style: solid; border-width: 1.0px 1.0px 1.0px 1.0px; border-color: #000000 #000000 #000000 #000000; padding: 4.0px 4.0px 4.0px 4.0px}\
ol.ol1 {list-style-type: decimal}\
ol.ol2 {list-style-type: upper-roman}\
</style>"


#define BOOKMARK_INDICATOR_SHOW_WINDOW "I"

extern int CalcHeaderSize(void);
long changeHTMLtoUTF8(char *dest, const char *src);


#endif // HTMLMODULE_H
