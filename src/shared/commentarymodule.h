#ifndef COMMENTARYMODULE_H
#define COMMENTARYMODULE_H
//
//  CommentaryModule.h
//  CWP-Student
//
//  Created by WADE MAXFIELD on 12/2/10.
//  Copyright 2010 Wade Maxfield All rights reserved.
//

#include "commentarydatabaseclass.h"
#include "structs.h"
#include "misclibrary.h"

extern int gNumberOfCommentaries;





void    addCommentaryToSelectedArrayByName(QString CommentaryName);
BOOL    AddCommentaryToSelectedArrayByIndexNumber(int CommentaryNumber);

void    getSelectedCommentaries(void);
BOOL    isCommentaryNameInSelectedArray(QString CommentaryName);
BOOL    isCommentaryNameInSelectedArray(QString CommentaryName);

int     OpenPersonalCommentaryAndPutInSelectedArray(QString Name);

BOOL    removeCommentaryFromSelectedArrayByName(QString Commentary);

void    saveSelectedCommentaries(void);


#endif // COMMENTARYMODULE_H
