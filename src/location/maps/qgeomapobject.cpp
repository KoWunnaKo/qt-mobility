/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qgeomapobject.h"
#include "qgeomapobject_p.h"

#include "qgeomapdata.h"

#include <QtAlgorithms>

QTM_BEGIN_NAMESPACE

/*!
    \class QGeoMapObject
    \brief The QGeoMapObject class is graphical item for display in
    QGraphicsGeoMap instancse, that is specified in terms of coordinates and
    distances.

    \inmodule QtLocation

    \ingroup maps-mapping-objects

    QGeoMapObject instances can also be grouped into heirarchies in order to
    simplify the process of creating compound objects and managing groups of
    objects.
*/

/*!
    \enum QGeoMapObject::Type

    Describes the type of a map object.

    \value ContainerType
        A basic QGeoMapObject.
    \value RectangleType
        A QGeoMapObject used to display a rectangular region.
    \value CircleType
        A QGeoMapObject used to display a circular region.
    \value PolylineType
        A QGeoMapObject used to display a multi-segment line.
    \value PolygonType
        A QGeoMapObject used to display a polygonal region.
    \value PixmapType
        A QGeoMapObject used to display a pixmap on a map.
    \value TextType
        A QGeoMapObject used to display text on a map
    \value RouteType
        A QGeoMapObject used to display a route.
*/

/*!
    Constructs a new map object with the specified \a parent.
*/
QGeoMapObject::QGeoMapObject(QGeoMapObject *parent)
        : d_ptr(new QGeoMapObjectPrivate(this, parent)) {}

/*!
    Constructs a new root map object associated with \a mapData.
*/
QGeoMapObject::QGeoMapObject(QGeoMapData *mapData)
        : d_ptr(new QGeoMapObjectPrivate(this, mapData)) {}

/*!
  \internal
*/
QGeoMapObject::QGeoMapObject(QGeoMapObjectPrivate *dd)
        : d_ptr(dd) {}

/*!
    Destroys this map object.
*/
QGeoMapObject::~QGeoMapObject()
{
    Q_D(QGeoMapObject);
    delete d;
}

/*!
    Returns the type of this map object.
*/
QGeoMapObject::Type QGeoMapObject::type() const
{
    Q_D(const QGeoMapObject);
    return d->type;
}

/*!
    \property QGeoMapObject::zValue
    \brief This property holds the z-value of the map object.

    The z-value determines the order in which the objects are drawn on the
    screen.  Objects with the same value will be drawn in the order that
    they were added to the map or map object.

    This is the same behaviour as QGraphicsItem.
*/
void QGeoMapObject::setZValue(int zValue)
{
    Q_D(QGeoMapObject);
    if (d->zValue != zValue) {
        d->zValue = zValue;
        if (d->parent) {
            d->parent->removeChildObject(this);
            d->parent->addChildObject(this);
        }
        emit zValueChanged(d->zValue);
    }
}

int QGeoMapObject::zValue() const
{
    Q_D(const QGeoMapObject);
    return d->zValue;
}

/*!
    \property QGeoMapObject::visible
    \brief This property holds whether the map object is visible.

    If this map object is not visible then none of the childObjects() will
    be displayed either.
*/
void QGeoMapObject::setVisible(bool visible)
{
    Q_D(QGeoMapObject);
    if (d->isVisible != visible) {
        d->isVisible = visible;
        if (d->info)
            d->info->visibleChanged(d->isVisible);
        emit visibleChanged(d->isVisible);
    }
}

bool QGeoMapObject::isVisible() const
{
    Q_D(const QGeoMapObject);
    return d->isVisible;
}

/*!
    \property QGeoMapObject::selected
    \brief This property holds whether the map object is selected.
*/
void QGeoMapObject::setSelected(bool selected)
{
    Q_D(QGeoMapObject);
    if (d->isSelected != selected) {
        d->isSelected = selected;
        if (d->info)
            d->info->selectedChanged(d->isSelected);
        emit selectedChanged(d->isSelected);
    }
}

bool QGeoMapObject::isSelected() const
{
    Q_D(const QGeoMapObject);
    return d->isSelected;
}

/*!
    Returns a bounding box which contains this map object.

    If this map object has children, the bounding box will be large
    enough to contain both this map object and all of its children.
*/
QGeoBoundingBox QGeoMapObject::boundingBox() const
{
    Q_D(const QGeoMapObject);

    QGeoBoundingBox bounds = d_ptr->boundingBox();

    if (d->children.size() == 0)
        return bounds;

    for (int i = 0; i < d->children.size(); ++i)
        bounds = bounds.united(d->children.at(i)->boundingBox());

    return bounds;
}

/*!
    Returns whether \a coordinate is contained with the boundary of this
    map object.

    If this map object has children, this function will return whether
    \a coordinate is contained within the boundary of this map object or
    within the boundary of any of its children.
*/
bool QGeoMapObject::contains(const QGeoCoordinate &coordinate) const
{
    Q_D(const QGeoMapObject);

    if (d_ptr->contains(coordinate))
        return true;

    for (int i = 0; i < d->children.size(); ++i)
        if (d->children.at(i)->contains(coordinate))
            return true;

    return false;
}

/*!
    Returns the parent of this map object.

    Returns 0 if this map object has no parent.
*/
QGeoMapObject* QGeoMapObject::parentObject() const
{
    Q_D(const QGeoMapObject);
    return d->parent;
}

/*!
    Adds \a childObject to the list of children of this map object.

    The children objects are drawn in order of the QGeoMapObject::zValue()
    value.  Children objects having the same z value will be drawn
    in the order they were added.

    The map object will take ownership of \a childObject.
*/
void QGeoMapObject::addChildObject(QGeoMapObject *childObject)
{
    Q_D(QGeoMapObject);

    if (!childObject || d->children.contains(childObject))
        return;

    childObject->d_ptr->addToParent(this);

    //binary search
    QList<QGeoMapObject*>::iterator i = qUpperBound(d->children.begin(), d->children.end(), childObject);
    d->children.insert(i, childObject);
}

/*!
    Removes \a childObject from the list of children of this map object.

    The map object will release ownership of \a childObject.
*/
void QGeoMapObject::removeChildObject(QGeoMapObject *childObject)
{
    if (!childObject)
        return;

    Q_D(QGeoMapObject);

    if (d->children.removeAll(childObject) > 0)
        childObject->d_ptr->removeFromParent();
}

/*!
    Returns the children of this object.
*/
QList<QGeoMapObject*> QGeoMapObject::childObjects() const
{
    Q_D(const QGeoMapObject);
    return d->children;
}

/*!
    Clears the children of this object.

    The child objects will be deleted.
*/
void QGeoMapObject::clearChildObjects()
{
    Q_D(QGeoMapObject);

    for (int i = 0; i < d->children.size(); ++i)
        d->children.at(i)->d_ptr->removeFromParent();

    qDeleteAll(d->children);
    d->children.clear();
}

/*!
    \internal
*/
void QGeoMapObject::objectUpdated()
{
    Q_D(QGeoMapObject);
    d->objectUpdated();
}

/*!
   \internal
*/
void QGeoMapObject::mapUpdated()
{
    Q_D(QGeoMapObject);
    d->mapUpdated();
}

/*!
    \internal
*/
bool QGeoMapObject::operator<(const QGeoMapObject &other) const
{
    Q_D(const QGeoMapObject);
    return d->zValue < other.d_func()->zValue;
}

/*!
    \internal
*/
bool QGeoMapObject::operator>(const QGeoMapObject &other) const
{
    Q_D(const QGeoMapObject);
    return d->zValue > other.d_func()->zValue;
}

/*******************************************************************************
*******************************************************************************/

QGeoMapObjectPrivate::QGeoMapObjectPrivate(QGeoMapObject *impl, QGeoMapObject *parent, QGeoMapObject::Type type)
        : type(type),
        parent(parent),
        zValue(0),
        isVisible(true),
        isSelected(false),
        mapData(0),
        info(0),
        q_ptr(impl)
{
    if (parent) {
        mapData = parent->d_ptr->mapData;
        mapData->associateMapObject(q_ptr);
    }
}

QGeoMapObjectPrivate::QGeoMapObjectPrivate(QGeoMapObject *impl, QGeoMapData *mapData, QGeoMapObject::Type type)
        : type(type),
        parent(0),
        zValue(0),
        isVisible(true),
        isSelected(false),
        mapData(mapData),
        info(0),
        q_ptr(impl) {}

QGeoMapObjectPrivate::~QGeoMapObjectPrivate()
{
    if (parent) {
        Q_Q(QGeoMapObject);
        parent->removeChildObject(q);
    }

    for (int i = 0; i < children.size(); ++i) {
        children.at(i)->d_ptr->parent = 0;
        delete children.at(i);
    }

    children.clear();

    if (info)
        delete info;
}

void QGeoMapObjectPrivate::setMapData(QGeoMapData *mapData)
{
    this->mapData = mapData;
    if (mapData) {
        mapData->associateMapObject(q_ptr);
        if (info)
            info->addToParent();
    }
    int size = children.size();
    for (int i = 0; i < size; ++i)
        children[i]->d_ptr->setMapData(mapData);
}

void QGeoMapObjectPrivate::addToParent(QGeoMapObject *parent)
{
    if (this->parent && (this->parent != parent)) {
        this->parent->removeChildObject(q_ptr);
    }

    this->parent = parent;
    setMapData(parent->d_ptr->mapData);

//    if (info)
//        info->addToParent();
}

void QGeoMapObjectPrivate::removeFromParent()
{
    parent = 0;
    mapData = 0;
    if (info)
        info->removeFromParent();
}

void QGeoMapObjectPrivate::objectUpdated()
{
    if (info)
        info->objectUpdated();
}

void QGeoMapObjectPrivate::mapUpdated()
{
    if (info)
        info->mapUpdated();

    for (int i = 0; i < children.size(); ++i)
        children[i]->mapUpdated();
}

QGeoBoundingBox QGeoMapObjectPrivate::boundingBox() const
{
    if (!info)
        return QGeoBoundingBox();

    return info->boundingBox();
}

bool QGeoMapObjectPrivate::contains(const QGeoCoordinate &coord) const
{
    if (!info)
        return false;

    return info->contains(coord);
}

/*******************************************************************************
*******************************************************************************/

#include "moc_qgeomapobject.cpp"

QTM_END_NAMESPACE