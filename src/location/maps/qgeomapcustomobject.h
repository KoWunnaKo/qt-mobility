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

#ifndef QGEOMAPCUSTOMOBJECT_H
#define QGEOMAPCUSTOMOBJECT_H

#include "qgeomapobject.h"
#include "qgeocoordinate.h"

#include <QPoint>

#include <QGraphicsItem>

QTM_BEGIN_NAMESPACE

class QGeoMapCustomObjectPrivate;

class Q_LOCATION_EXPORT QGeoMapCustomObject : public QGeoMapObject
{
    Q_OBJECT
    Q_PROPERTY(QGraphicsItem* graphicsItem READ graphicsItem WRITE setGraphicsItem NOTIFY graphicsItemChanged)
    Q_PROPERTY(QPoint offset READ offset WRITE setOffset NOTIFY offsetChanged)

public:
    QGeoMapCustomObject();
    QGeoMapCustomObject(const QGeoCoordinate &coordinate, const QPoint &offset = QPoint(0, 0));
    ~QGeoMapCustomObject();

    QGeoMapObject::Type type() const;

    void update();

    QGraphicsItem* graphicsItem() const;
    void setGraphicsItem(QGraphicsItem *graphicsItem);

    QPoint offset() const;
    void setOffset(const QPoint &offset);

    void setOrigin(const QGeoCoordinate &origin);
    void setUnits(const CoordinateUnit &unit);
    void setTransformType(const TransformType &type);

Q_SIGNALS:
    void triggerUpdate();
    void graphicsItemChanged(QGraphicsItem *graphicsItem);
    void offsetChanged(const QPoint &offset);

private:
    QGeoMapCustomObjectPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QGeoMapCustomObject)
    Q_DISABLE_COPY(QGeoMapCustomObject)
};

QTM_END_NAMESPACE

#endif
