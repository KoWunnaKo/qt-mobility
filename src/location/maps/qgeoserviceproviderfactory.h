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

#ifndef QGEOSERVICEPROVIDERFACTORY_H
#define QGEOSERVICEPROVIDERFACTORY_H

#include "qmobilityglobal.h"
#include "qgeoserviceprovider.h"

#include <QtPlugin>
#include <QMap>
#include <QString>

QTM_BEGIN_NAMESPACE

class Q_LOCATION_EXPORT QGeoServiceProviderFactory
{
public:
    virtual ~QGeoServiceProviderFactory() {}

    virtual QString providerName() const = 0;
    virtual int providerVersion() const = 0;

    virtual QGeoSearchManagerEngine* createSearchManagerEngine(const QMap<QString, QVariant> &parameters,
            QGeoServiceProvider::Error *error,
            QString *errorString) const;
    virtual QGeoMappingManagerEngine* createMappingManagerEngine(const QMap<QString, QVariant> &parameters,
            QGeoServiceProvider::Error *error,
            QString *errorString) const;
    virtual QGeoRoutingManagerEngine* createRoutingManagerEngine(const QMap<QString, QVariant> &parameters,
            QGeoServiceProvider::Error *error,
            QString *errorString) const;
};

QTM_END_NAMESPACE

QT_BEGIN_NAMESPACE
#define QT_GEOSERVICE_BACKEND_INTERFACE "com.nokia.qt.mobility.geoservice.serviceproviderfactory/1.0"
Q_DECLARE_INTERFACE(QtMobility::QGeoServiceProviderFactory, QT_GEOSERVICE_BACKEND_INTERFACE);
QT_END_NAMESPACE

#endif
