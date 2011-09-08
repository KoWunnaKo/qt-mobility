/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef S60CAMERAFLASHCONTROL_H
#define S60CAMERAFLASHCONTROL_H

#include <qcameraflashcontrol.h>

#include "s60cameraadvsettings.h"

QT_USE_NAMESPACE

QT_FORWARD_DECLARE_CLASS(S60CameraService)
QT_FORWARD_DECLARE_CLASS(S60ImageCaptureSession)

/*
 * Control to setup Flash related camera settings.
 */
class S60CameraFlashControl : public QCameraFlashControl
{
    Q_OBJECT

public: // Constructors & Destructor

    S60CameraFlashControl(QObject *parent = 0);
    S60CameraFlashControl(S60ImageCaptureSession *session, QObject *parent = 0);
    ~S60CameraFlashControl();

public: // QCameraExposureControl

    // Flash Mode
    QCameraExposure::FlashModes flashMode() const;
    void setFlashMode(QCameraExposure::FlashModes mode);
    bool isFlashModeSupported(QCameraExposure::FlashModes mode) const;

    bool isFlashReady() const;

/*
Q_SIGNALS: // QCameraExposureControl
    void flashReady(bool);
*/

private slots: // Internal Slots

    void resetAdvancedSetting();

private: // Data

    S60ImageCaptureSession          *m_session;
    S60CameraService                *m_service;
    S60CameraAdvSettings            *m_advancedSettings;
    QCameraExposure::FlashModes     m_flashMode;
};

#endif // S60CAMERAFLASHCONTROL_H
