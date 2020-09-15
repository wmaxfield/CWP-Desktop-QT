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
//  ResultSet.m
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/21/10.
//  Copyright 2010,2014 Wade Maxfield. All rights reserved.
// converted more or less

#include "recordset.h"

#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
RecordSet::RecordSet()
{
    Results = NULL;
    NumberOfRowsSet=0;
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
RecordSet::RecordSet(QSqlQuery *query, /*NumRows:*/int nr, /* NumCols:*/ int nc, /*Database:*/ QSqlDatabase db)
{

        Results = query;
        NumberOfRows = nr;
        NumberOfColumns = nc;
        NumberOfRowsSet=0;
        database = db;
        CurrentRow =1 ;// start past the header

}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
RecordSet::~RecordSet() {
    done();
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL RecordSet::atEOF()
{
    return (Results->at() == QSql::AfterLastRow);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
BOOL RecordSet::atBOF()
{
    return (CurrentRow == 1);
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int RecordSet::FieldCount()
{
    return NumberOfColumns;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
int RecordSet::RecordCount()
{
    int index=0;
    Results->first();

    while (Results->next())
        index++;

    Results->first();
    NumberOfRows = index;
    NumberOfRowsSet=YES;

    return index;
}

// ------------------------------------------------------------------------
// get the column name for the index item starting at 1.
// ------------------------------------------------------------------------
QString RecordSet::columnName(int columnNumber) {
     QString data;

    if (columnNumber <= 0) {
        columnNumber =1;
    }

    data = IdxField( columnNumber);
    return data;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void RecordSet::MoveFirst()  // the first record
{
    CurrentRow =1;
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void RecordSet::MoveLast()   // the last record
{
    if (NumberOfRowsSet)
       CurrentRow = NumberOfRows;
    Results->last();
}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void RecordSet::MoveNext()   // move to the next record
{
    CurrentRow++;
    Results->next() ;

}
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void RecordSet::MovePrev() {
    CurrentRow --;
    if (CurrentRow< 1) {
        CurrentRow =0;
        return;
    }

    Results->previous();
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void RecordSet::MoveToRow(int RowNumber)
{
    CurrentRow = RowNumber;

    int index;

    if (Results)
        Results->first();

    if (RowNumber ==0 ) {

        return;
    }


    for (index = 0 ; index < RowNumber; index++) {
        Results->next();
    }
}
// ------------------------------------------------------------------------
// return a String,
// NOTE:  FOR BACKWARDS COMPATABILITY TO OLD CODE, THIS idxField() is
//  1 based, even though the underneath code is 0 based for QT
// ------------------------------------------------------------------------
QString RecordSet::IdxField(int index)
{
    //int CalcIndex;
     QString s="";

    if (index==0)
        return s;


    if(index > NumberOfColumns)
        return s;


    s = Results->value(index-1).toString();
    return s;
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
QString  RecordSet::Field(QString fieldName)
{
    int index;
    QString fName;

    for (index=0; index < NumberOfColumns; index++) {
        fName =Results->record().fieldName(index);
        if (fieldName ==fName ) {
            //this is the field, return it
            // index has the column index, increment the column index by 1
           // index++;
            fName = IdxField(index+1);
            return fName;
        }
    }
    return "";// not found
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void RecordSet::done()
{
    if (Results) {
        delete Results;
        Results = nil;
    }

}

