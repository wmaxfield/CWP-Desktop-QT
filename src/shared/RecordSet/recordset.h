#ifndef RECORDSET_H
#define RECORDSET_H

//
//  ResultSet.h
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/21/10.
//  Copyright 2010,2014 Wade Maxfield. All rights reserved.
//
#include "cwpdefines.h"
#include "../shared/Database/database.h"

class RecordSet
{
public:
    RecordSet();
    RecordSet(QSqlQuery *query, /*NumRows:*/int nr, /* NumCols:*/ int nc, /*Database:*/ QSqlDatabase db);
    ~RecordSet();

    BOOL atEOF();
    BOOL atBOF();
    int  FieldCount();
    int RecordCount();
    void MoveFirst();  // the first record
    void MoveLast();   // the last record
    void MoveNext();   // move to the next record
    void MoveToRow(int RowNumber);

    QString IdxField(int index);

    QString Field(QString fieldName);


    void done(); // call when finished with record
    QString columnName(int columnNumber);
    void MovePrev(); // MOVE TO PREVIOUS ROW



    QSqlDatabase database;      // parent database  (not to be released)
    QSqlQuery    *Results;    // the results returned from
    int     NumberOfRows;
    int     NumberOfRowsSet;  // when non-zero, we know the number of rows in a query.
    int     NumberOfColumns;

    int     CurrentRow;     // the row we are at in the peeling off of the data
};

#endif // RECORDSET_H




