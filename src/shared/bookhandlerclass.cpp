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
//
//  BookHandlerClass.m
//  CrosswordTry2
//
//  Created by WADE MAXFIELD on 9/24/10.
//  Copyright 2010 CrosswordProject. All rights reserved.
// converted

#include "bookhandlerclass.h"
#include "../shared/Database/database.h"

BookHandlerClass::BookHandlerClass()
{
    Database= 0;
}

BookHandlerClass::~BookHandlerClass()
{
    if (Database) {
        switch ( ((SQliteDatabase*)Database)->getDbType()) {
            case TYPE_BBL_TYPE:
                case TYPE_ABP_TYPE:
                delete ((BibleDatabaseClass*) Database);
                break;

            case TYPE_COMMENTARY_TYPE:
                delete ((CommentaryDatabaseClass*)Database);
                break;
        }
    }


}

/* failure don't use
-(NSString *)name
{
    if (!Database) {
        return @"no database";
    }

    switch ( [(SQLiteDatabase*)Database dbType]) {
        case TYPE_BBL_TYPE:
        case TYPE_ABP_TYPE:
        case TYPE_COMMENTARY_TYPE:
        case TYPE_NOTES_TYPE:
        case TYPE_MORPHOLOGY_TYPE:
            return ((BibleDatabaseClass*) Database)._BibleName;
            break;

        default:
                return @"unknown database type";
            break;
    }

}
-(void)setName:(NSString *)lname
{
    if (!Database) {
        return;
    }

    switch ( [(SQLiteDatabase*)Database dbType]) {
        case TYPE_BBL_TYPE:
        case TYPE_ABP_TYPE:
        case TYPE_COMMENTARY_TYPE:
        case TYPE_NOTES_TYPE:
        case TYPE_MORPHOLOGY_TYPE:
            ((BibleDatabaseClass*) Database)._BibleName = [lname copy];
            break;

        default:

            break;
    }

}
*/

