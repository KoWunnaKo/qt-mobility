/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qmagnetometer.h"
#include "qmagnetometer_p.h"

QTM_BEGIN_NAMESPACE

IMPLEMENT_READING(QMagnetometerReading)

/*!
    \class QMagnetometerReading
    \ingroup sensors_reading
    \inmodule QtSensors
    \since 1.0

    \brief The QMagnetometerReading class represents one reading from the
           magnetometer.

    \section2 QMagnetometerReading Units
    The magnetometer returns magnetic flux density values along 3 axes.
    The scale of the values is teslas. The axes are arranged as follows.

    \image sensors-coordinates2.jpg

    The magnetometer can report on either raw magnetic flux values or geomagnetic flux values.
    By default it returns raw magnetic flux values. The QMagnetometer::returnGeoValues property
    must be set to return geomagnetic flux values.

    The primary difference between raw and geomagnetic values is that extra processing
    is done to eliminate local magnetic interference from the geomagnetic values so they
    represent only the effect of the Earth's magnetic field. This process is not perfect
    and the accuracy of each reading may change.

    The image below shows the difference between geomagnetic (on the left) and raw (on the right)
    readings for a phone that is being subjected to magnetic interference.

    \image sensors-geo-vs-raw-magnetism.jpg

    The accuracy of each reading is measured as a number from 0 to 1.
    A value of 1 is the highest level that the device can support and 0 is
    the worst.
    \sa {http://wiki.forum.nokia.com/index.php/CS001671_-_Calibrating_the_magnetometer_sensor}{CS001671 - Calibrating the magnetometer sensor}
*/

/*!
    \property QMagnetometerReading::x
    \brief the raw magnetic flux density on the X axis.

    Measured as telsas.
    \sa {QMagnetometerReading Units}
    \since 1.0
*/

qreal QMagnetometerReading::x() const
{
    return d->x;
}

/*!
    Sets the raw magnetic flux density on the X axis to \a x.
    \since 1.0
*/
void QMagnetometerReading::setX(qreal x)
{
    d->x = x;
}

/*!
    \property QMagnetometerReading::y
    \brief the raw magnetic flux density on the Y axis.

    Measured as telsas.
    \sa {QMagnetometerReading Units}
    \since 1.0
*/

qreal QMagnetometerReading::y() const
{
    return d->y;
}

/*!
    Sets the raw magnetic flux density on the Y axis to \a y.
    \since 1.0
*/
void QMagnetometerReading::setY(qreal y)
{
    d->y = y;
}

/*!
    \property QMagnetometerReading::z
    \brief the raw magnetic flux density on the Z axis.

    Measured as telsas.
    \sa {QMagnetometerReading Units}
    \since 1.0
*/

qreal QMagnetometerReading::z() const
{
    return d->z;
}

/*!
    Sets the raw magnetic flux density on the Z axis to \a z.
    \since 1.0
*/
void QMagnetometerReading::setZ(qreal z)
{
    d->z = z;
}

/*!
    \property QMagnetometerReading::calibrationLevel
    \brief the accuracy of the reading.

    Measured as a value from 0 to 1 with higher values being better.

    Note that this only changes when measuring geomagnetic flux density.
    Raw magnetic flux readings will always have a value of 1.
    \sa {QMagnetometerReading Units}, {http://wiki.forum.nokia.com/index.php/CS001671_-_Calibrating_the_magnetometer_sensor}{CS001671 - Calibrating the magnetometer sensor}
    \since 1.0
*/

qreal QMagnetometerReading::calibrationLevel() const
{
    return d->calibrationLevel;
}

/*!
    Sets the accuracy of the reading to \a calibrationLevel.
    \since 1.0
*/
void QMagnetometerReading::setCalibrationLevel(qreal calibrationLevel)
{
    d->calibrationLevel = calibrationLevel;
}

// =====================================================================

/*!
    \class QMagnetometerFilter
    \ingroup sensors_filter
    \inmodule QtSensors

    \brief The QMagnetometerFilter class is a convenience wrapper around QSensorFilter.

    The only difference is that the filter() method features a pointer to QMagnetometerReading
    instead of QSensorReading.
    \since 1.0
*/

/*!
    \fn QMagnetometerFilter::filter(QMagnetometerReading *reading)

    Called when \a reading changes. Returns false to prevent the reading from propagating.

    \sa QSensorFilter::filter()
    \since 1.0
*/

char const * const QMagnetometer::type("QMagnetometer");

/*!
    \class QMagnetometer
    \ingroup sensors_type
    \inmodule QtSensors

    \brief The QMagnetometer class is a convenience wrapper around QSensor.

    The only behavioural difference is that this class sets the type properly.

    This class also features a reading() function that returns a QMagnetometerReading instead of a QSensorReading.

    For details about how the sensor works, see \l QMagnetometerReading.

    \sa QMagnetometerReading
    \since 1.0
*/

/*!
    \fn QMagnetometer::QMagnetometer(QObject *parent)

    Construct the sensor as a child of \a parent.
    \since 1.0
*/

/*!
    \fn QMagnetometer::~QMagnetometer()

    Destroy the sensor. Stops the sensor if it has not already been stopped.
    \since 1.0
*/

/*!
    \fn QMagnetometer::reading() const

    Returns the reading class for this sensor.

    \sa QSensor::reading()
    \since 1.0
*/

/*!
    \property QMagnetometer::returnGeoValues
    \brief a value indicating if geomagnetic values should be returned.

    Set to true to return geomagnetic flux density.
    Set to false (the default) to return raw magnetic flux density.

    Note that you must access this property via QObject::property() and QObject::setProperty().
    The property must be set before calling start().
    \since 1.0
*/

#include "moc_qmagnetometer.cpp"
QTM_END_NAMESPACE

