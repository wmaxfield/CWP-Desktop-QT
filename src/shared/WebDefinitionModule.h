#ifndef WEBDEFINITIONMODULE_H
#define WEBDEFINITIONMODULE_H
/*
 *  WebDefinitionModule.h
 *  CrosswordTry2
 *
 *  Created by WADE MAXFIELD on 9/27/10.
 *  Copyright 2010 Wade Maxfield. All rights reserved.
 * converted
 */

#define  GTItalicLT "<i>"
#define  GTSlashItalicLT "</i>"
#define  GTThan "<"
#ifndef  STYLE_FONT
#define  STYLE_FONT "<style>\rp\r{\rfont-size: 500%;\r}\r.unit {float:left;padding-top:13px;margin:.30em;}\r.strongs {font-size:smaller;}\r.strongs a{text-decoration:none;color:green;}\r.english {font-weight:bold;color:#17235c;}\rspan.strongs {display:block;}\rspan.greek {display:block;}\rspan.english {display:block;}\rspan.unit:hover {background-color: #ffffe8;}\rsup {font-size:smaller;vertical-align:baseline;position:relative;bottom:0.33em;}\r\r\r\r</style>"
#endif
#ifndef SUPend
#define  SUPend "</sup>"
#endif
#ifndef SUPstart
#define  SUPstart "<sup>"
#endif
//padding-top was 13px

#define  TABLE_STYLE_ABP "<style type=\"text/css\">\
.unit {float:left;padding-top:0px;margin:.30em;}\
.strongs {font-size:smaller;}\
.strongs a{text-decoration:none;color:green;}\
.english {font-weight:bold;color:#17235c;}\
span.strongs {display:block;}\rspan.greek {display:block;}\
span.english {display:block;}\
span.unit:hover {background-color: #ffffe8;}\
sup {font-size:smaller;vertical-align:baseline;position:relative;bottom:0.33em;}\
</style>"//vertical-align was baseline
#define  TABLE_STYLE_EMBOSSED_CELLS "<style type=\"text/css\">\rtable.sample {\r\tborder-width: 1px 1px 1px 1px;\r\tborder-spacing: 3px;\r\tborder-style: solid solid solid solid;\r\tborder-color: gray gray gray gray;\r\tborder-collapse: separate;\r\tbackground-color: white;\r}\rtable.sample th {\r\tborder-width: 4px 4px 4px 4px;\r\tpadding: 4px 4px 4px 4px;\r\tborder-style: ridge ridge ridge ridge;\r\tborder-color: gray gray gray gray;\r\tbackground-color: white;\r\t-moz-border-radius: 0px 0px 0px 0px;\r}\rtable.sample td {\r\tborder-width: 4px 4px 4px 4px;\r\tpadding: 4px 4px 4px 4px;\r\tborder-style: ridge ridge ridge ridge;\r\tborder-color: gray gray gray gray;\r\tbackground-color: white;\r\twidth: zz%;\r\tfont-size: ffpt;\r\t-moz-border-radius: 0px 0px 0px 0px;\r}\rp\r{\rfont-size: 500%;\r}\r</style>"
/*
#define  TABLE_STYLE_NO_HEADER_LINES "<style type=\"text/css\">\
table.sample {\
\tborder-width: 0px 0px 0px 0px;\
\tborder-spacing: 5px;\
\tborder-style: outset outset outset outset;\
\tborder-color: red red red red;\
\tborder-collapse: separate;\
\tbackground-color: rgb(255, 250, 250);\
\
}\
table.sample th {\
\tborder-width: 0px 0px 0px 0px;\
\tpadding: 1px 1px 1px 1px;\
\tborder-style: inset inset inset inset;\r\tborder-color: white white white white;\
\tbackground-color: white;\
\
\t-moz-border-radius: 0px 0px 0px 0px;\
}\
table.sample td {\
\tborder-width: 0px 0px 0px 0px;\
\tpadding: 1px 1px 1px 1px;\
\tborder-style: inset inset inset inset;\
\tborder-color: white white white white;\
\tbackground-color: white;\r\twidth: zz%;\
\tfont-size: ffpt;\
\t-moz-border-radius: 0px 0px 0px 0px;\
\
}\
p\
{\
font-size: 500%;\
}\
</style>"
*/

// padding: top, right, bottom, left
#define  TABLE_STYLE_NO_LINES "<style type=\"text/css\">\
table.sample {\
\tborder-width: 0px 0px 0px 0px;\
\tborder-spacing: 0px;\
\tborder-style: outset outset outset outset;\
\tborder-color: gray gray gray gray;\
\tborder-collapse: separate;\
\tbackground-color: white;\
\
}\
table.sample th {\
\tborder-width: 0px 0px 0px 0px;\
\tpadding: 1px 13px btm%px 1px;\
\tborder-style: inset inset inset inset;\
\tborder-color: gray gray gray gray;\
\tbackground-color: white;\
\t-moz-border-radius: 0px 0px 0px 0px;\
\
}\
table.sample td {\
\tvertical-align: top;\
\tborder-width: 0px 0px 0px 0px;\
\tpadding-top: 3px;\
\tpadding-bottom: 3px;\
\tpadding-right: 13px;\
\tpadding-left: %leftpad%px;\
\tborder-style: inset inset inset inset;\
\tborder-color: gray gray gray gray;\
\tbackground-color: white;\
\twidth: zz%;\
\tfont-size: ffpt;\
\tline-height: 1.2;\
\t-moz-border-radius: 0px 0px 0px 0px;\
}\
p\
{\
font-size: 500\%;\
}\
\
</style>" // was line-height 2.0

#define  TABLE_STYLE_SINGLE_LINE_BORDERS "<style type=\"text/css\">\
body { margin: 0; padding: 0; }\
table.sample {\
\tborder-width: 1px 0px 1px 0px;\
\tborder-spacing: 13px;\
\tborder-style: outset outset outset solid;\
\tborder-color: gray gray gray gray;\
\tborder-collapse: collapse;\
\tbackground-color: white;\
\tpadding: 3px 13px 3px 3px;\
\t-moz-border-radius: 0px 0px 0px 0px;\
}\
table.sample th {\
\tborder-width: 1px 0px 1px 0px;\
\tpadding: 3px 13px btm%px 3px;\
\tborder-style: inset inset inset solid;\
\tborder-color: gray gray gray gray;\
\tbackground-color: white;\
\t-moz-border-radius: 0px 0px 0px 0px;\
}\
table.sample td {\
\tborder-width: 1px 0px 1px 0px;\
\tpadding: 3px 13px 3px %leftpad%px;\
\tborder-style: inset inset inset solid;\
\tborder-color: gray gray gray gray;\
\tbackground-color: white;\
\twidth: zz%;\
\tfont-size: ffpt;\
\t-moz-border-radius: 0px 0px 0px 0px;\
\
}\
p\
{\
font-size: 500%;\
}\
</style>"

#define  TABLE_STYLE_SINGLE_LINE_BORDERS_ABP "<style type=\"text/css\">\
body { margin: 0; padding: 0; }\
\rtable.sample {\
table-layout: fixed;\
\tborder-width: 1px 0px 1px 0px;\
\tborder-spacing: 1px;\
\tborder-style: outset outset outset solid;\
\tborder-color: gray gray gray gray;\
\tborder-collapse: collapse;\
\tbackground-color: white;\
\tpadding: 3px 13px 3px %leftpad%px;\
width: 100%;\
}\
table.sample th {\
\tborder-width: 1px 0px 1px 0px;\
\tpadding: 3px 13px 3px 3px;\
\tborder-style: inset inset inset solid;\
\tborder-color: gray gray gray gray;\
\tbackground-color: white;\
\t-moz-border-radius: 0px 0px 0px 0px;\
width: 100%;\
}\
table.sample td {\
\tborder-width: 1px 0px 1px 0px;\
\tpadding: 3px 13px 3px 3px;\
\tborder-style: inset inset inset solid;\
\tborder-color: gray gray gray gray;\
\tbackground-color: white;\
\twidth: zz%;\
\tfont-size: ffpt;\
\t-moz-border-radius: 0px 0px 0px 0px;\
\
}\
p\
{\
font-size: 500\%;\
}\
.unit {float:left;padding-top:13px;margin:.30em;}\
.strongs {font-size:smaller;}\
.strongs a{text-decoration:none;color:green;}\
.english {font-weight:bold;color:#17235c;}\
span.strongs {display:block;}\
span.greek {display:block;}\
span.english {display:block;}\
span.unit:hover {background-color: #ffffe8;}\
sup {font-size:smaller;vertical-align:baseline;position:relative;bottom:0.33em;}\
\r\r\r</style>"



extern int numberOfBooksShownChanged; //main.cpp
extern int changeWasDueToUserInteraction;//main.cpp
extern int currentScrollPosition;
extern int verseBeforeNumberOfBooksChanged;

#endif // WEBDEFINITIONMODULE_H
