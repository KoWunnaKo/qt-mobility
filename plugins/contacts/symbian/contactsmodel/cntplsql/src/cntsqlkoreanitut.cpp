/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


/**
 @file
 @internalComponent
 @released
*/
#include "cntsqlkoreanitut.h"
#include "cntsqlsearchinterface.h"
#include "cntsqlsearchbase.h"
#include "c12keykeymap.h"
#include "koreaninput.h"
#include <QString>
#include <QStringList>


const int KMinimumSearchPatternLength = 1;
const QString KColumn = "nbr";
const int KColumnLimit = 4;


/**
CCntPplViewSession first phase constructor.
*/
CntSqlKoreanItuT::CntSqlKoreanItuT(const C12keyKeyMap* twelveKeyKeyMap)
              :CntSqlSearchBase(twelveKeyKeyMap)
    {
    mKoreaninput = new KoreanInput();
    }
  
/**
CCntPplViewSession destructor.
*/
CntSqlKoreanItuT::~CntSqlKoreanItuT()
    {
    delete mKoreaninput;
    }
   
QString CntSqlKoreanItuT::createInputSpecificSearch(const QString &pattern)
    {
    int type = getSQLQueryType(pattern);
    QString sqlString;
    switch (type) {
        case AllFromOneTable:
            sqlString = returnAllcontacts(pattern);
            break;
        case ExactMatchFromOneTable:
            sqlString = exactMatchSearch(pattern);
            break;
        case KoreanBasicSearch:
            sqlString = basicKoreanSearch(pattern);
            break;
        default: 
            ;
        }
    return sqlString;
    }

CntSqlSearchInterface::QueryType CntSqlKoreanItuT::getQueryType()
    {
    return CntSqlSearchInterface::KoreaITUT;
    }


QString CntSqlKoreanItuT::basicKoreanSearch(const QString &pattern)
    {
    QStringList tokens = getSearchPattern(pattern);
    if(tokens.count() > 1 )
        {
        return "SELECT contact_id FROM " + selectTable(tokens.at(0)) +
                " WHERE (" + compareColumnsInOrder(tokens) + 
                ") OR " + compareColumnsFromMidleInOrder(tokens) +
                " ORDER BY first_name, last_name ASC;";
        }
    else
        {
        return QString();
        }
    }

QString CntSqlKoreanItuT::compareColumnsInOrder(QStringList &tokens) const
    {
    int count = tokens.count();
    int andSatement = 0;
    QString query = QString();
    QString columns = QString();
    //At least two columns to compare.
    if( count > 1 )
        {
        //Curent column limitation is 4.
        for( int i = 0; i < count && i < KColumnLimit; i++ )
            {
            columns = getSearchColumns(tokens.at(i),i );
            if (!columns.isEmpty())
                {
                ++andSatement;
                query.append(columns);
                if(andSatement < count && i < 3)
                    {
                    query.append(" AND ");
                    }
                }
            }
        }
    return query;
    }

QString CntSqlKoreanItuT::compareColumnsFromMidleInOrder( QStringList &tokens) const
    {
    int count = tokens.count();
    int andSatement = 0;
    int i = 0;
    QString query = QString();
    QString columns = QString();
    //At least two columns to compare.
    if( count > 1 )
        {
        query.append("(");
        //Curent column limitation is 4.
        for( ; i < count && i < KColumnLimit; i++ )
            {
            if(count > KColumnLimit -1)
                {
                count = KColumnLimit -1;
                }
            columns = getSearchColumns(tokens.at(i),(i + 1));
            if (!columns.isEmpty())
                {
                ++andSatement;
                query.append(columns);
                if(andSatement < count && i < 3)
                    {
                    query.append(" AND ");
                    }
                else{
                    query.append(")");
                    }
                }
            }
        query.append(" OR (");
        //Curent column limitation is 4.
        i = 0;
        andSatement = 0;
        if(count > KColumnLimit - 2)
            {
            count = KColumnLimit - 2;
            }
        for( ; i < count && i < KColumnLimit; i++ )
            {
            columns = getSearchColumns(tokens.at(i),(i + 2) );
            if (!columns.isEmpty())
                {
                ++andSatement;
                query.append(columns);
                if(andSatement < count && i < 3)
                    {
                    query.append(" AND ");
                    }
                else{
                    query.append(")");
                    }
                }
            }
        query.append(" OR ");
        columns = getSearchColumns(tokens.at(0),3 );
        if (!columns.isEmpty())
            {
            query.append(columns);
            }
        }
    return query;
    }

QString CntSqlKoreanItuT::getSearchColumns(const QString& token, int position) const
    {
    QString statement = QString();
    QString column;
    QString lower;
    QString upper;
    QString num;
    
    if(position < KColumnLimit )
        {
        int err = mTwelveKeyMap->GetNumericLimits(token, lower, upper);
        if(err)
           {
           return QString("");
           }
        if(position > 0 )
            {
            num.setNum(position + 1);
            }  
        column = KColumn;
        column.append(num);
        statement = QString(  "(" + column + ">" + lower + 
                                    " AND " + column + "<" + upper + ")" );
        }
    return statement; 
    }

QStringList CntSqlKoreanItuT::getSearchPattern(const QString &pattern)
    {
    //return pattern.split("0", QString::SkipEmptyParts);
    return mKoreaninput->Tokenize(pattern);
    }

CntSqlKoreanItuT::SqlQueryType CntSqlKoreanItuT::getSQLQueryType(const QString &pattern)
    {
    if (pattern.length() == KMinimumSearchPatternLength )
        {
        return AllFromOneTable;
        }
    else if (pattern.length() == KMinimumSearchPatternLength || pattern.startsWith('3') || pattern.startsWith('6') || pattern.startsWith('9')) //TODO: Change this after tokens are detected!!!
        {
        return ExactMatchFromOneTable;
        }
    //TODO: Change this after Korean tokens are working
    else if (pattern.length() > KMinimumSearchPatternLength && getSearchPattern(pattern).count() > 1) //TODO: Change this after tokens are detected!!!
        {
        return KoreanBasicSearch;
        }
    else
        {
        return NA;
        }
    }

