#ifndef INDEXPATH_H
#define INDEXPATH_H
#include <QString>

struct indexAndSectionStruct {
    int section;
    int row;
    int type;// TYPE of item
};

typedef indexAndSectionStruct CWPIndexPath;

struct VerseCellStructure {
    QString VerseNumberLabel;
    QString searchResultLabel;
    QString searchingStatusLabel;
    QString VerseTextLabel;
};

typedef VerseCellStructure VerseCellStruct;

#define NSIndexPath CWPIndexPath
#endif // INDEXPATH_H
