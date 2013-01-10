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

#include "qrotationsensor.h"
#include "qrotationsensor_p.h"

QTM_BEGIN_NAMESPACE

IMPLEMENT_READING(QRotationReading)

/*!
    \class QRotationReading
    \ingroup sensors_reading
    \inmodule QtSensors
    \since 1.0

    \brief The QRotationReading class represents one reading from the
           rotation sensor.

    \section2 QRotationReading Units

    The rotation reading contains 3 angles, measured in degrees that define
    the orientation of the device in three-dimensional space. These angles
    are similar to yaw, pitch and roll but are defined using only right hand
    rotation with axes as defined by the right hand cartesian coordinate system.

    \image sensors-rotation.jpg

    The three angles are applied to the device in the following order.

    \list
    \o Right-handed rotation z (-180, 180]. Starting from the y-axis and
     incrementing in the counter-clockwise direction.
    \o Right-handed rotation x [-90, 90]. Starting from the new
     (once-rotated) y-axis and incrementing towards the z-axis.
    \o Right-handed rotation y (-180, 180]. Starting from the new
     (twice-rotated) z-axis and incrementing towards the x-axis.
    \endlist

    Here is a visualization showing the order in which angles are applied.

    \image sensors-rotation-anim.gif

    The 0 point for the z angle is defined as a fixed, external entity and
    is device-specific. While magnetic North is typically used as this
    reference point it may not be. Do not attempt to compare values
    for the z angle between devices or even on the same device if it has
    moved a significant distance.

    If the device cannot detect a fixed, external entity the z angle will
    always be 0 and the QRotationSensor::hasZ property will be set to false.

    The 0 point for the x and y angles are defined as when the x and y axes
    of the device are oriented towards the horizon. Here is an example of
    how the x value will change with device movement.

    \image sensors-rotation2.jpg

    Here is an example of how the y value will change with device movement.

    \image sensors-rotation3.jpg

    Note that when x is 90 or -90, values for z and y achieve rotation around
    the same axis (due to the order of operations). In this case the y
    rotation will be 0.
*/

/*!
    \property QRotationReading::x
    \brief the rotation around the x axis.

    Measured as degrees.
    \sa {QRotationReading Units}
    \since 1.0
*/

qreal QRotationReading::x() const
{
    return d->x;
}

/*!
    Sets the rotation around the x axis to \a x.
    \since 1.0
*/
void QRotationReading::setX(qreal x)
{
    d->x = x;
}

/*!
    \property QRotationReading::y
    \brief the rotation around the y axis.

    Measured as degrees.
    \sa {QRotationReading Units}
    \since 1.0
*/

qreal QRotationReading::y() const
{
    return d->y;
}

/*!
    Sets the rotation around the y axis to \a y.
    \since 1.0
*/
void QRotationReading::setY(qreal y)
{
    d->y = y;
}

/*!
    \property QRotationReading::z
    \brief the rotation around the z axis.

    Measured as degrees.
    \sa {QRotationReading Units}
    \since 1.0
*/

qreal QRotationReading::z() const
{
    return d->z;
}

/*!
    Sets the rotation around the z axis to \a z.
    \since 1.0
*/
void QRotationReading::setZ(qreal z)
{
    d->z = z;
}

// =====================================================================

/*!
    \class QRotationFilter
    \ingroup sensors_filter
    \inmodule QtSensors

    \brief The QRotationFilter class is a convenience wrapper around QSensorFilter.

    The only difference is that the filter() method features a pointer to QRotationReading
    instead of QSensorReading.
    \since 1.0
*/

/*!
    \fn QRotationFilter::filter(QRotationReading *reading)

    Called when \a reading changes. Returns false to prevent the reading from propagating.

    \sa QSensorFilter::filter()
    \since 1.0
*/

char const * const QRotationSensor::type("QRotationSensor");

/*!
    \class QRotationSensor
    \ingroup sensors_type
    \inmodule QtSensors

    \brief The QRotationSensor class is a convenience wrapper around QSensor.

    The only behavioural difference is that this class sets the type properly.

    This class also features a reading() function that returns a QRotationReading instead of a QSensorReading.

    For details about how the sensor works, see \l QRotationReading.

    \sa QRotationReading
    \since 1.0
*/

/*!
    \fn QRotationSensor::QRotationSensor(QObject *parent)

    Construct the sensor as a child of \a parent.
    \since 1.0
*/

/*!
    \fn QRotationSensor::~QRotationSensor()

    Destroy the sensor. Stops the sensor if it has not already been stopped.
    \since 1.0
*/

/*!
    \fn QRotationSensor::reading() const

    Returns the reading class for this sensor.

    \sa QSensor::reading()
    \since 1.0
*/

/*!
    \property QRotationSensor::hasZ
    \brief a value indicating if the z angle is available.

    Returns true if z is available.
    Returns false if z is not available.
    \since 1.0
*/

#include "moc_qrotationsensor.cpp"
QTM_END_NAMESPACE

