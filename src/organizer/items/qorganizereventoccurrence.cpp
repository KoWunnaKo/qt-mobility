/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qorganizereventoccurrence.h"

QTM_USE_NAMESPACE

void QOrganizerEventOccurrence::setStartDateTime(const QDateTime& startDateTime)
{
    QOrganizerItemEventTimeRange etr = detail<QOrganizerItemEventTimeRange>();
    etr.setStartDateTime(startDateTime);
    saveDetail(&etr);
}

QDateTime QOrganizerEventOccurrence::startDateTime() const
{
    QOrganizerItemEventTimeRange etr = detail<QOrganizerItemEventTimeRange>();
    return etr.startDateTime();
}

void QOrganizerEventOccurrence::setEndDateTime(const QDateTime& endDateTime)
{
    QOrganizerItemEventTimeRange etr = detail<QOrganizerItemEventTimeRange>();
    etr.setEndDateTime(endDateTime);
    saveDetail(&etr);
}

QDateTime QOrganizerEventOccurrence::endDateTime() const
{
    QOrganizerItemEventTimeRange etr = detail<QOrganizerItemEventTimeRange>();
    return etr.endDateTime();
}

void QOrganizerEventOccurrence::setParentItemId(const QOrganizerItemId& parentId) const
{
    Q_UNUSED(parentId);
}

QOrganizerItemId QOrganizerEventOccurrence::parentItemId() const
{
    return QOrganizerItemId();
}

void QOrganizerEventOccurrence::setOriginalDateTime(const QDateTime& dateTime)
{
    // XXX TODO: is this operation allowed?  require backends to fail?
    // ie, read only detail.
    Q_UNUSED(dateTime);
}

QDateTime QOrganizerEventOccurrence::originalDateTime() const
{
    return QDateTime();
}

void QOrganizerEventOccurrence::setPriority(QOrganizerItemPriority::Priority priority)
{
    QOrganizerItemPriority pd = detail<QOrganizerItemPriority>();
    pd.setPriority(priority);
    saveDetail(&pd);
}

QOrganizerItemPriority::Priority QOrganizerEventOccurrence::priority() const
{
    QOrganizerItemPriority pd = detail<QOrganizerItemPriority>();
    return pd.priority();
}

QString QOrganizerEventOccurrence::locationName() const
{
    QOrganizerItemLocation ld = detail<QOrganizerItemLocation>();
    return ld.locationName();
}
void QOrganizerEventOccurrence::setLocationName(const QString& locationName)
{
    QOrganizerItemLocation ld = detail<QOrganizerItemLocation>();
    ld.setLocationName(locationName);
    saveDetail(&ld);
}

QString QOrganizerEventOccurrence::locationAddress() const
{
    QOrganizerItemLocation ld = detail<QOrganizerItemLocation>();
    return ld.address();
}
void QOrganizerEventOccurrence::setLocationAddress(const QString& locationAddress)
{
    QOrganizerItemLocation ld = detail<QOrganizerItemLocation>();
    ld.setAddress(locationAddress);
    saveDetail(&ld);
}

QString QOrganizerEventOccurrence::locationGeoCoordinates() const
{
    // XXX TODO: consistency with QOILocation API ?
    QOrganizerItemLocation ld = detail<QOrganizerItemLocation>();
    return ld.geoLocation();
}
void QOrganizerEventOccurrence::setLocationGeoCoordinates(const QString& locationCoordinates)
{
    QOrganizerItemLocation ld = detail<QOrganizerItemLocation>();
    ld.setGeoLocation(locationCoordinates);
    saveDetail(&ld);
}
