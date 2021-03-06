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

#include <qirproximitysensor.h>
#include "qirproximitysensor_p.h"

QTM_BEGIN_NAMESPACE

IMPLEMENT_READING(QIRProximityReading)

/*!
    \class QIRProximityReading
    \ingroup sensors_reading
    \inmodule QtSensors

    \brief The QIRProximityReading class holds readings from the IR proximity sensor.

    The IR (infra-red) proximity sensor detects proximity by beaming out infra-red light
    and detecting how much of the light returns.

    The biggest limitation of this technology is that there is no reliable way to turn the
    reflectance values into distances unless both the item being detected and the ambient
    conditions are known.

    \section2 QIRProximityReading Units

    The sensor reports reflectance as a percentage. That is, 0 indicates nothing was detected
    within the range of the sensor and 100 indicates the infra-red signal returned at the full
    power level that it was sent at.

    With some IR sensors, it is quite uncommon to reach the top and the bottom of the
    value range, and some parts of the range ends might not be obtainable at all. This is due to the
    behavior of the sensor hardware. With these sensors, the absolute value of reflectance should never
    be used directly. Instead, applications should react to the relative change of the reading values. Use
    QProximitySensor if it is only necessary to check if something is close to the device or not.
*/

/*!
    \property QIRProximityReading::reflectance
    \brief Holds the reflectance value.

    The reflectance is a percentage (from 0 to 100) indicating how much of the transmitted
    infra-red light was returned.

    \sa {QIRProximityReading Units}
*/
qreal QIRProximityReading::reflectance() const
{
    return d->reflectance;
}

/*!
    Sets the reflectance value to \a reflectance.
*/
void QIRProximityReading::setReflectance(qreal reflectance)
{
    d->reflectance = reflectance;
}

// =====================================================================

/*!
    \class QIRProximityFilter
    \ingroup sensors_filter
    \inmodule QtSensors

    \brief The QIRProximityFilter class is a convenience wrapper around QSensorFilter.

    The only difference is that the filter() method features a pointer to QIRProximityReading
    instead of QSensorReading.
*/

/*!
    \fn QIRProximityFilter::filter(QIRProximityReading *reading)

    Called when \a reading changes. Returns false to prevent the reading from propagating.

    \sa QSensorFilter::filter()
*/

char const * const QIRProximitySensor::type("QIRProximitySensor");

/*!
    \class QIRProximitySensor
    \ingroup sensors_type
    \inmodule QtSensors

    \brief The QIRProximitySensor class is a convenience wrapper around QSensor.

    The only behavioural difference is that this class sets the type properly.

    This class also features a reading() function that returns a QIRProximityReading instead of a QSensorReading.

    For details about how the sensor works, see \l QIRProximityReading.

    \sa QIRProximityReading
*/

/*!
    \fn QIRProximitySensor::QIRProximitySensor(QObject *parent)

    Construct the sensor as a child of \a parent.
*/

/*!
    \fn QIRProximitySensor::~QIRProximitySensor()

    Destroy the sensor. Stops the sensor if it has not already been stopped.
*/

/*!
    \fn QIRProximitySensor::reading() const

    Returns the reading class for this sensor.

    \sa QSensor::reading()
*/


#include "moc_qirproximitysensor.cpp"
QTM_END_NAMESPACE
