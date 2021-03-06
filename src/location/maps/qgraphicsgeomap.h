/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
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

#ifndef QGRAPHICSGEOMAP_H
#define QGRAPHICSGEOMAP_H

#include "qmobilityglobal.h"

#include <QGraphicsWidget>

QTM_BEGIN_NAMESPACE

class QGeoCoordinate;
class QGeoMappingManager;
class QGeoBoundingBox;
class QGeoMapObject;
class QGeoMapOverlay;
class QGraphicsGeoMapPrivate;

class Q_LOCATION_EXPORT QGraphicsGeoMap : public QGraphicsWidget
{
    Q_OBJECT
    Q_ENUMS(MapType)
    Q_ENUMS(ConnectivityMode)

    Q_PROPERTY(qreal minimumZoomLevel READ minimumZoomLevel CONSTANT)
    Q_PROPERTY(qreal maximumZoomLevel READ maximumZoomLevel CONSTANT)
    Q_PROPERTY(qreal zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)
    Q_PROPERTY(MapType mapType READ mapType WRITE setMapType NOTIFY mapTypeChanged)
    Q_PROPERTY(QGeoCoordinate center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(ConnectivityMode connectivityMode READ connectivityMode WRITE setConnectivityMode NOTIFY connectivityModeChanged)
    Q_PROPERTY(qreal minimumTilt READ minimumTilt CONSTANT)
    Q_PROPERTY(qreal maximumTilt READ maximumTilt CONSTANT)
    Q_PROPERTY(qreal tilt READ tilt WRITE setTilt NOTIFY tiltChanged)
    Q_PROPERTY(bool supportsTilting READ supportsTilting CONSTANT)
    Q_PROPERTY(qreal bearing READ bearing WRITE setBearing NOTIFY bearingChanged)
    Q_PROPERTY(bool supportsBearing READ supportsBearing CONSTANT)

public:
    enum MapType {
        NoMap,
        StreetMap,
        SatelliteMapDay,
        SatelliteMapNight,
        TerrainMap,
        HybridMap,
        TransitMap,
        GrayStreetMap,
        MobileStreetMap,
        MobileTerrainMap,
        MobileHybridMap,
        MobileTransitMap,
        MobileGrayStreetMap
    };

    enum ConnectivityMode {
        NoConnectivity,
        OfflineMode,
        OnlineMode,
        HybridMode
    };

    QGraphicsGeoMap(QGeoMappingManager *manager, QGraphicsItem *parent = 0);
    virtual ~QGraphicsGeoMap();

    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *parent);

    qreal minimumZoomLevel() const;
    qreal maximumZoomLevel() const;

    void setZoomLevel(qreal zoomLevel);
    qreal zoomLevel() const;

    bool supportsBearing() const;
    void setBearing(qreal bearing);
    qreal bearing() const;

    bool supportsTilting() const;
    void setTilt(qreal tilt);
    qreal tilt() const;
    qreal minimumTilt() const;
    qreal maximumTilt() const;

    void setCenter(const QGeoCoordinate &center);
    QGeoCoordinate center() const;

    QList<MapType> supportedMapTypes() const;
    void setMapType(MapType mapType);
    MapType mapType() const;

    QList<ConnectivityMode> supportedConnectivityModes() const;
    void setConnectivityMode(ConnectivityMode connectivityMode);
    ConnectivityMode connectivityMode() const;

    bool supportsCustomMapObjects() const;

    QList<QGeoMapObject*> mapObjects() const;
    void addMapObject(QGeoMapObject *mapObject);
    void removeMapObject(QGeoMapObject *mapObject);
    void clearMapObjects();

    QList<QGeoMapOverlay*> mapOverlays() const;
    void addMapOverlay(QGeoMapOverlay *overlay);
    void removeMapOverlay(QGeoMapOverlay *overlay);
    void clearMapOverlays();

    QGeoBoundingBox viewport() const;
    void fitInViewport(const QGeoBoundingBox &bounds, bool preserveViewportCenter = false);

    QList<QGeoMapObject*> mapObjectsAtScreenPosition(const QPointF &screenPosition) const;
    QList<QGeoMapObject*> mapObjectsInScreenRect(const QRectF &screenRect) const;
    QList<QGeoMapObject*> mapObjectsInViewport() const;

    QPointF coordinateToScreenPosition(const QGeoCoordinate &coordinate) const;
    QGeoCoordinate screenPositionToCoordinate(QPointF screenPosition) const;

public Q_SLOTS:
    void pan(int dx, int dy);

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);

Q_SIGNALS:
    void zoomLevelChanged(qreal zoomLevel);
    void centerChanged(const QGeoCoordinate &coordinate);
    void mapTypeChanged(QGraphicsGeoMap::MapType mapType);
    void connectivityModeChanged(QGraphicsGeoMap::ConnectivityMode connectivityMode);
    void bearingChanged(qreal bearing);
    void tiltChanged(qreal tilt);

private Q_SLOTS:
    void updateMapDisplay(const QRectF& target);

private:
    QGraphicsGeoMapPrivate *d_ptr;
    Q_DISABLE_COPY(QGraphicsGeoMap)
};

QTM_END_NAMESPACE

#endif
