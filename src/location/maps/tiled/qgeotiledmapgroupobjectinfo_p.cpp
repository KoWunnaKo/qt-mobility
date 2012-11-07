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

#include "qgeotiledmapgroupobjectinfo_p.h"

#include "qgeotiledmapobjectinfo_p.h"
#include "qgeotiledmapdata.h"
#include "qgeotiledmapdata_p.h"

#include "qgeomapobject.h"
#include "qgeomapgroupobject.h"

#include "qgeomapobjectengine_p.h"

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsPathItem>

QTM_BEGIN_NAMESPACE

QGeoTiledMapGroupObjectInfo::QGeoTiledMapGroupObjectInfo(QGeoTiledMapData *mapData, QGeoMapObject *mapObject)
    : QGeoTiledMapObjectInfo(mapData, mapObject)
{

    group = static_cast<QGeoMapGroupObject*>(mapObject);

    connect(group,
            SIGNAL(childAdded(QGeoMapObject*)),
            this,
            SLOT(childAdded(QGeoMapObject*)));
    connect(group,
            SIGNAL(childUpdated(QGeoMapObject*)),
            this,
            SLOT(childUpdated(QGeoMapObject*)));
    connect(group,
            SIGNAL(childRemoved(QGeoMapObject*)),
            this,
            SLOT(childRemoved(QGeoMapObject*)));

    pathItem = new QGraphicsPathItem();
    graphicsItem = pathItem;
    pathItem->setPos(0.0, 0.0);

    QList<QGeoMapObject*> objects = group->childObjects();

    foreach(QGeoMapObject * object, objects) {
        QGeoTiledMapObjectInfo* info = static_cast<QGeoTiledMapObjectInfo*>(object->info());

        if (info)
            info->graphicsItem->setParentItem(graphicsItem);
    }

    updateItem();
}

QGeoTiledMapGroupObjectInfo::~QGeoTiledMapGroupObjectInfo()
{
}

void QGeoTiledMapGroupObjectInfo::childAdded(QGeoMapObject *childObject)
{
    if (!childObject)
        return;

    QGeoTiledMapObjectInfo* info = static_cast<QGeoTiledMapObjectInfo*>(childObject->info());
    if (info && info->graphicsItem) {
        // the child's z value will get updated in QGeoTiledMapGroupObjectInfo::childUpdated
        // we do this in order to keep the same order of operations that we had previously
        childObject->disconnect(childObject, SIGNAL(zValueChanged(int)), info, SLOT(zValueChanged(int)));
        info->graphicsItem->setParentItem(graphicsItem);
        tiledMapDataPrivate->update(mapObject());
        //tiledMapDataPrivate->update(childObject);
    }
}

void QGeoTiledMapGroupObjectInfo::childUpdated(QGeoMapObject *childObject)
{
    if (!childObject)
        return;

    QGeoTiledMapObjectInfo* info = static_cast<QGeoTiledMapObjectInfo*>(childObject->info());
    if (info && info->graphicsItem) {
        //info->graphicsItem->setParentItem(graphicsItem);
        tiledMapDataPrivate->update(mapObject());
        info->zValueChanged(childObject->zValue());
    }
}

void QGeoTiledMapGroupObjectInfo::childRemoved(QGeoMapObject *childObject)
{
    if (childObject && tiledMapDataPrivate->oe) {
        tiledMapDataPrivate->oe->removeObject(childObject);
        updateItem();
    }
}

#include "moc_qgeotiledmapgroupobjectinfo_p.cpp"

QTM_END_NAMESPACE
