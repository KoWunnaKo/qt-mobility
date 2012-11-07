/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qgeotiledmappixmapobjectinfo_p.h"

#include "qgeotiledmapdata.h"
#include "qgeotiledmapdata_p.h"

#include "qgeomappixmapobject.h"

QTM_BEGIN_NAMESPACE

QGeoTiledMapPixmapObjectInfo::QGeoTiledMapPixmapObjectInfo(QGeoTiledMapData *mapData, QGeoMapObject *mapObject)
    : QGeoTiledMapObjectInfo(mapData, mapObject)
{
    pixmap = static_cast<QGeoMapPixmapObject*>(mapObject);

    connect(pixmap,
            SIGNAL(pixmapChanged(QPixmap)),
            this,
            SLOT(pixmapChanged(QPixmap)));
    connect(pixmap,
            SIGNAL(offsetChanged(QPoint)),
            this,
            SLOT(offsetChanged(QPoint)));

    pixmapItem = new QGraphicsPixmapItem();
    graphicsItem = pixmapItem;

    originChanged(this->pixmap->origin());
    pixmapChanged(this->pixmap->pixmap());
    offsetChanged(this->pixmap->offset());
}

QGeoTiledMapPixmapObjectInfo::~QGeoTiledMapPixmapObjectInfo() {}

void QGeoTiledMapPixmapObjectInfo::pixmapChanged(const QPixmap &/*pixmap*/)
{
    pixmapItem->setPixmap(this->pixmap->pixmap());
    pixmapItem->setScale(1.0);
    updateItem();
}

void QGeoTiledMapPixmapObjectInfo::offsetChanged(const QPoint &/*offset*/)
{
    QPoint offset = pixmap->offset();

    QTransform trans;
    trans.translate(offset.x(), offset.y());
    pixmapItem->setTransform(trans);

    updateItem();
}

#include "moc_qgeotiledmappixmapobjectinfo_p.cpp"

QTM_END_NAMESPACE

