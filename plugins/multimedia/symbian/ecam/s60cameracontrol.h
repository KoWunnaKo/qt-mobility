/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef S60CAMERACONTROL_H
#define S60CAMERACONTROL_H

#include <qcameracontrol.h>

#include "s60cameraengineobserver.h"    // MCameraEngineObserver
#include "s60videocapturesession.h"     // TVideoCaptureState

#include <e32base.h>
#include <fbs.h>

QT_USE_NAMESPACE

class S60CameraService;
class S60ImageCaptureSession;
class S60VideoCaptureSession;
class S60CameraSettings;
class CCameraEngine;
class S60CameraViewfinderEngine;

/*
 * Control for controlling camera base operations (e.g. start/stop and capture
 * mode).
 */
class S60CameraControl : public QCameraControl, public MCameraEngineObserver
{
    Q_OBJECT
    
public: // Enums

    enum ViewfinderOutputType {
        VideoWidgetOutput,
        VideoRendererOutput,
        VideoWindowOutput
    };
    
public: // Constructors & Destructor
    
    S60CameraControl(QObject *parent = 0);
    S60CameraControl(S60VideoCaptureSession *videosession, 
                     S60ImageCaptureSession *imagesession, 
                     QObject *parent = 0);
    ~S60CameraControl();
    
public: // QCameraControl
    
    // State
    QCamera::State state() const;
    void setState(QCamera::State state);

    // Status
    QCamera::Status status() const;
    
    // Capture Mode
    QCamera::CaptureMode captureMode() const;
    void setCaptureMode(QCamera::CaptureMode);
    bool isCaptureModeSupported(QCamera::CaptureMode mode) const;
    
    // Property Setting
    bool canChangeProperty(QCameraControl::PropertyChangeType changeType, QCamera::Status status) const;

/*
Q_SIGNALS:
    void stateChanged(QCamera::State);
    void statusChanged(QCamera::Status);
    void error(int error, const QString &errorString);
    void captureModeChanged(QCamera::CaptureMode);
*/

public: // Internal
    
    void setError(const TInt error, const QString &description);
    CCameraEngine *resetCameraOrientation();
    
    // To provide QVideoDeviceControl info
    static int deviceCount();
    static QString name(const int index);
    static QString description(const int index);
    int defaultDevice() const;
    int selectedDevice() const;
    void setSelectedDevice(const int index);

    void setVideoOutput(QObject *output, ViewfinderOutputType type);
    
private Q_SLOTS: // Internal Slots

    void videoStateChanged(const S60VideoCaptureSession::TVideoCaptureState state);
    void advancedSettingsCreated();
    
protected: // MCameraEngineObserver

    void MceoCameraReady();
    void MceoHandleError(TCameraEngineError aErrorType, TInt aError);

private: // Internal
    
    QCamera::Error fromSymbianErrorToQtMultimediaError(int aError);
    
    void loadCamera();
    void unloadCamera();
    void startCamera();
    void stopCamera();
    
    void resetCamera();
    void setCameraHandles();

Q_SIGNALS: // Internal Signals

    void cameraReadyChanged(bool);
    void devicesChanged();

private: // Data
    
    CCameraEngine               *m_cameraEngine;
    S60CameraViewfinderEngine   *m_viewfinderEngine;
    S60ImageCaptureSession      *m_imageSession;
    S60VideoCaptureSession      *m_videoSession;
    S60CameraSettings           *m_advancedSettings;
    QObject                     *m_videoOutput;
    QCamera::CaptureMode        m_captureMode;
    QCamera::CaptureMode        m_requestedCaptureMode;
    QCamera::Status             m_internalState;
    QCamera::State              m_requestedState;
    int                         m_deviceIndex;
    mutable int                 m_error;
    bool                        m_changeCaptureModeWhenReady;
    S60VideoCaptureSession::TVideoCaptureState m_videoCaptureState;
};

#endif // S60CAMERACONTROL_H