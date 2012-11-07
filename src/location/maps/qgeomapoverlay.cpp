/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qgeomapoverlay.h"
#include "qgeomapoverlay_p.h"

#include "qgeomapdata.h"

QTM_BEGIN_NAMESPACE

/*!
    \class QGeoMapOverlay
    \brief The QGeoMapOverlay class is used to draw overlays on the map.

    \inmodule QtLocation
    \since 1.1

    \ingroup maps-mapping

    This allows for general drawing to occur in overlays above the map.
*/

/*!
    Constructs a new map overlay object.
*/
QGeoMapOverlay::QGeoMapOverlay()
    : d_ptr(new QGeoMapOverlayPrivate()) {}

/*!
    Destroys this map overlay object.
*/
QGeoMapOverlay::~QGeoMapOverlay()
{
    delete d_ptr;
}

/*!
\fn void QGeoMapOverlay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option)

Paints the overlay on \a painter, using the options \a option.
    \since 1.1
*/

/*!
    \internal
    \since 1.1
*/
void QGeoMapOverlay::setMapData(QGeoMapData * mapData)
{
    d_ptr->mapData = mapData;
}

/*!
    Returns the QGeoMapData instance that this overlay is associated, or 0
    if there is not such instance.

    The QGeoMapData instance provides access to information such as the
    zoom level and viewport position as well as methods to convert
    screen positions to coordinates and vice-versa.
    \since 1.1
*/
QGeoMapData const * QGeoMapOverlay::mapData() const
{
    return d_ptr->mapData;
}

/*******************************************************************************
*******************************************************************************/

QGeoMapOverlayPrivate::QGeoMapOverlayPrivate()
    : mapData(0) {}


QGeoMapOverlayPrivate::~QGeoMapOverlayPrivate() {}

/*******************************************************************************
*******************************************************************************/

QTM_END_NAMESPACE
