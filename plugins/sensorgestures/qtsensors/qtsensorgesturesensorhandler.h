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

#ifndef QTSENSORGESTURESENSORHANDLER_H
#define QTSENSORGESTURESENSORHANDLER_H

#include <QObject>

#include <qaccelerometer.h>
#include <qsensor.h>
#include <qorientationsensor.h>
#include <qproximitysensor.h>
//#include <qirproximitysensor.h>
//#include <QTapSensor>

QTM_BEGIN_NAMESPACE

class QtSensorGestureSensorHandler : public QObject
{
    Q_OBJECT
    Q_ENUMS(SensorGestureSensors)
public:
    explicit QtSensorGestureSensorHandler(QObject *parent = 0);

    enum SensorGestureSensors {
        Accel = 0,
        Orientation,
        Proximity,
        IrProximity,
        Tap
    };
    static QtSensorGestureSensorHandler *instance();
    qreal accelRange;

public slots:
    void accelChanged();
    void orientationChanged();
    void proximityChanged();
//    void irProximityChanged();
    void doubletap();

    bool startSensor(SensorGestureSensors sensor);
    void stopSensor(SensorGestureSensors sensor);

Q_SIGNALS:
    void accelReadingChanged(QAccelerometerReading *reading);
    void orientationReadingChanged(QOrientationReading *reading);
    void proximityReadingChanged(QProximityReading *reading);
//    void irProximityReadingChanged(QIRProximityReading *reading);
//    void dTabReadingChanged(QTapReading *reading);

private:
    QAccelerometer *accel;
    QOrientationSensor *orientation;
    QProximitySensor *proximity;
//    QIRProximitySensor *irProx;
//    QTapSensor *tapSensor;

    QMap<SensorGestureSensors, int> usedSensorsMap;

};

QTM_END_NAMESPACE

#endif // QTSENSORGESTURESENSORHANDLER_H