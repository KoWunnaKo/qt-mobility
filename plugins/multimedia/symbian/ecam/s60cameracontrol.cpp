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

#include <QtCore/qstring.h>

#include "s60cameraservice.h"
#include "s60cameraengine.h"
#include "s60cameracontrol.h"
#include "s60imagecapturesession.h"
#include "s60videowidgetcontrol.h"
#include "s60cameraviewfinderengine.h"
#include "s60cameraconstants.h"

S60CameraControl::S60CameraControl(QObject *parent) :
    QCameraControl(parent)
{
}

S60CameraControl::S60CameraControl(S60VideoCaptureSession *videosession, 
                                   S60ImageCaptureSession *imagesession, 
                                   QObject *parent):
    QCameraControl(parent),
    m_cameraEngine(NULL),
    m_viewfinderEngine(NULL),
    m_imageSession(NULL),
    m_videoSession(NULL),
    m_advancedSettings(NULL),
    m_videoOutput(NULL),
    m_captureMode(QCamera::CaptureStillImage),  // Default CaptureMode
    m_requestedCaptureMode(QCamera::CaptureStillImage),
    m_internalState(QCamera::UnloadedStatus),   // Default Status
    m_requestedState(QCamera::UnloadedState),   // Default State
    m_deviceIndex(KDefaultCameraDevice),
    m_error(KErrNone),
    m_changeCaptureModeWhenReady(false),
    m_videoCaptureState(S60VideoCaptureSession::ENotInitialized)
{
    if (videosession)
        m_videoSession = videosession;
    else
        Q_ASSERT(true);
    
    if (imagesession)
        m_imageSession = imagesession;
    else
        Q_ASSERT(true);
    // From now on it's safe to assume ImageSession and VideoSession exist
    
    TRAPD(err, m_cameraEngine = CCameraEngine::NewL(m_deviceIndex, KECamCameraPriority, this));
    if (err) {
        m_error = err;
    }

    m_viewfinderEngine = new S60CameraViewfinderEngine(this, m_cameraEngine);
    if (m_viewfinderEngine == NULL) {
        m_error = KErrNoMemory;
    }
    connect(this, SIGNAL(statusChanged(QCamera::Status)),
        m_imageSession, SLOT(cameraStatusChanged(QCamera::Status)));
    connect(this, SIGNAL(statusChanged(QCamera::Status)),
        m_videoSession, SLOT(cameraStatusChanged(QCamera::Status)));
    connect(m_videoSession, SIGNAL(stateChanged(S60VideoCaptureSession::TVideoCaptureState)),
        this, SLOT(videoStateChanged(S60VideoCaptureSession::TVideoCaptureState)));
    connect(m_imageSession, SIGNAL(advancedSettingCreated()), this, SLOT(advancedSettingsCreated()));
    connect(this, SIGNAL(cameraReadyChanged(bool)), m_imageSession, SIGNAL(readyForCaptureChanged(bool)));
    connect(m_viewfinderEngine, SIGNAL(error(int, const QString&)), this, SIGNAL(error(int,const QString&)));
    connect(m_imageSession, SIGNAL(cameraError(int, const QString&)), this, SIGNAL(error(int, const QString&)));
    
    setCameraHandles();
}

S60CameraControl::~S60CameraControl()
{
    unloadCamera();

    if (m_viewfinderEngine) {
        delete m_viewfinderEngine;
        m_viewfinderEngine = NULL;
    }

    if (m_cameraEngine) {
        delete m_cameraEngine;
        m_cameraEngine = NULL;
    }
}

void S60CameraControl::setState(QCamera::State state)
{
    if (m_error) { // Most probably failure in contructor
        setError(m_error, QString("Unexpected camera error."));
        return;
    }
    
    if (m_requestedState == state) {
        return;
    }
    
    switch (state) {
        case QCamera::UnloadedState: // To UnloadedState - Release resources
            switch (m_internalState) {
                case QCamera::UnloadedStatus:
                    // Do nothing
                    break;
                case QCamera::LoadingStatus:
                case QCamera::StartingStatus:
                    // Release resources when ready  (setting state handles this)
                    return;
                case QCamera::LoadedStatus:
                case QCamera::StandbyStatus:
                    // Unload
                    unloadCamera();
                    break;
                case QCamera::ActiveStatus:
                    // Stop and Unload
                    stopCamera();
                    unloadCamera();
                    break;
                    
                default:
                    // Unrecognized internal state (Status)
                    setError(KErrGeneral, QString("Unexpected camera error."));
                    return;
            }
            m_requestedState = QCamera::UnloadedState;
            break;
            
        case QCamera::LoadedState: // To LoadedState - Reserve resources OR Stop ViewFinder and Cancel Capture
            switch (m_internalState) {
                case QCamera::UnloadedStatus:
                    // Load
                    loadCamera();
                    break;
                case QCamera::LoadingStatus:
                    // Discard, already moving to LoadedStatus
                    return;
                case QCamera::StartingStatus:
                    // Stop when ready  (setting state handles this)
                    return;
                case QCamera::LoadedStatus:
                case QCamera::StandbyStatus:
                    // Do nothing
                    break;
                case QCamera::ActiveStatus:
                    // Stop
                    stopCamera();
                    break;
                    
                default:
                    // Unregocnized internal state (Status)
                    setError(KErrGeneral, QString("Unexpected camera error."));
                    return;
            }
            m_requestedState = QCamera::LoadedState;
            break;
            
        case QCamera::ActiveState: // To ActiveState - (Reserve Resources and) Start ViewFinder
            switch (m_internalState) {
                case QCamera::UnloadedStatus:
                    // Load and Start (setting state handles starting)
                    loadCamera();
                    break;
                case QCamera::LoadingStatus:
                    // Start when loaded (setting state handles this)
                    break;
                case QCamera::StartingStatus:
                    // Discard, already moving to ActiveStatus
                    return;
                case QCamera::LoadedStatus:
                case QCamera::StandbyStatus:
                    // Start
                    startCamera();
                    break;
                case QCamera::ActiveStatus:
                    // Do nothing
                    break;
                    
                default:
                    // Unregocnized internal state (Status)
                    setError(KErrGeneral, QString("Unexpected camera error."));
                    return;
            }
            m_requestedState = QCamera::ActiveState;
            break;
            
        default:
            setError(KErrNotSupported, QString("Requested state is not supported."));
            return;
    }
    
    emit stateChanged(m_requestedState);    
}

QCamera::State S60CameraControl::state() const
{
    return m_requestedState;
}

QCamera::Status S60CameraControl::status() const
{
    return m_internalState;
}

QCamera::CaptureMode S60CameraControl::captureMode() const
{
    return m_captureMode;
}

void S60CameraControl::setCaptureMode(QCamera::CaptureMode mode)
{    
    if (m_error) { // Most probably failure in contructor
        setError(m_error, QString("Unexpected camera error."));
        return;
    }

    // If capturemode change was requested earlier, but was postponed due
    // to backend being busy, m_changeCaptureModeWhenReady flag is set when
    // trying to set the capturemode again internally.
    if (m_requestedCaptureMode == mode && !m_changeCaptureModeWhenReady) {
        return;
    }
    
    if (!isCaptureModeSupported(mode)) {
        setError(KErrNotSupported, QString("Requested capture mode is not supported."));
        return;
    }
    
    switch (m_internalState) {
        case QCamera::UnloadedStatus:
        case QCamera::LoadedStatus:
        case QCamera::StandbyStatus:
            switch (mode) {
                case QCamera::CaptureStillImage:
                    m_videoSession->releaseVideoRecording();
                    m_requestedCaptureMode = QCamera::CaptureStillImage;
                    m_captureMode = QCamera::CaptureStillImage;
                    break;
                case QCamera::CaptureVideo:
                    m_requestedCaptureMode = QCamera::CaptureVideo;
                    m_captureMode = QCamera::CaptureVideo;
                    if (m_internalState == QCamera::LoadedStatus || m_internalState == QCamera::StandbyStatus) {
                        int prepareSuccess = m_videoSession->initializeVideoRecording();
                        setError(prepareSuccess, QString("Loading video capture failed."));
                    }
                    break;
            }
            break;
        case QCamera::LoadingStatus:
        case QCamera::StartingStatus:
            m_requestedCaptureMode = mode;
            m_changeCaptureModeWhenReady = true;
            return;
        case QCamera::ActiveStatus:
            // Stop, Change Mode and Start again
            stopCamera();
            switch (mode) {
                case QCamera::CaptureStillImage:
                    m_videoSession->releaseVideoRecording();
                    m_requestedCaptureMode = QCamera::CaptureStillImage;
                    m_captureMode = QCamera::CaptureStillImage;
                    startCamera();
                    break;
                case QCamera::CaptureVideo:
                    m_requestedCaptureMode = QCamera::CaptureVideo;
                    m_captureMode = QCamera::CaptureVideo;
                    int prepareSuccess = m_videoSession->initializeVideoRecording();
                    setError(prepareSuccess, QString("Loading video capture failed."));
                    break;
            }
            break;

        default:
            // Unregocnized internal state (Status)
            setError(KErrNotSupported, QString("Requested capture mode is not supported."));
            break;
    }

    emit captureModeChanged(mode);
}

bool S60CameraControl::isCaptureModeSupported(QCamera::CaptureMode mode) const
{
    switch (mode) {
        case QCamera::CaptureStillImage:
            return true;
        case QCamera::CaptureVideo:
            return true;

        default:
            return false;
    }
}

bool S60CameraControl::canChangeProperty(QCameraControl::PropertyChangeType changeType, QCamera::Status status) const
{
    Q_UNUSED(status);

    bool returnValue = false;

    switch (changeType) {
        case QCameraControl::CaptureMode:
        case QCameraControl::VideoEncodingSettings:
        case QCameraControl::ImageEncodingSettings:
            returnValue = true;
            break;

        case QCameraControl::Viewfinder:
            returnValue = false;
            break;

        default:
            // Safer to revert state before the unknown operation
            returnValue = false;
            break;
    }
    return returnValue;
}

void S60CameraControl::setVideoOutput(QObject *output, ViewfinderOutputType type)
{
    switch (type) {
        case VideoWidgetOutput:
            m_viewfinderEngine->setVideoWidgetControl(output);
            break;
        case VideoRendererOutput:
            m_viewfinderEngine->setVideoRendererControl(output);
            break;
        case VideoWindowOutput:
            m_viewfinderEngine->setVideoWindowControl(output);
            break;

        default:
            break;
    }
}

void S60CameraControl::loadCamera()
{
    m_internalState = QCamera::LoadingStatus;
    emit statusChanged(m_internalState);

    m_cameraEngine->ReserveAndPowerOn();

    // Completion notified in MceoCameraReady()
}

void S60CameraControl::unloadCamera()
{
    m_internalState = QCamera::LoadingStatus;
    emit statusChanged(m_internalState);

    m_cameraEngine->ReleaseAndPowerOff();

    m_internalState = QCamera::UnloadedStatus;
    emit statusChanged(m_internalState);
}

void S60CameraControl::startCamera()
{
    m_internalState = QCamera::StartingStatus;
    emit statusChanged(m_internalState);

    emit cameraReadyChanged(true);
    if (m_viewfinderEngine)
        m_viewfinderEngine->startViewfinder();
    else
        setError(KErrGeneral, QString("Failed to start viewfinder."));

    m_internalState = QCamera::ActiveStatus;
    emit statusChanged(m_internalState);

#ifdef Q_CC_NOKIAX86 // Emulator
    MceoCameraReady(); // Signal that we are ready
#endif
}

void S60CameraControl::stopCamera()
{
    m_internalState = QCamera::StartingStatus;
    emit statusChanged(m_internalState);

    // Cancel ongoing operations if any
    m_imageSession->cancelCapture();
    m_videoSession->stopRecording();

    emit cameraReadyChanged(false);
    if (m_viewfinderEngine)
        m_viewfinderEngine->stopViewfinder();
    else
        setError(KErrGeneral, QString("Failed to stop viewfinder."));

    m_internalState = QCamera::LoadedStatus;
    emit statusChanged(m_internalState);
}

void S60CameraControl::videoStateChanged(const S60VideoCaptureSession::TVideoCaptureState state)
{
    // Save video state
    m_videoCaptureState = state;

    if (state == S60VideoCaptureSession::EInitialized) {
        // Don't downgrade state
        if (m_internalState == QCamera::StartingStatus || m_internalState == QCamera::ActiveStatus) {
            m_internalState = QCamera::ActiveStatus;
            if (m_internalState == QCamera::StartingStatus)
                emit statusChanged(m_internalState);
        }
        else {
            if (m_internalState != QCamera::LoadedStatus) {
                m_internalState = QCamera::LoadedStatus;
                emit statusChanged(m_internalState);
            }
        }

        switch (m_requestedState) {
            case QCamera::UnloadedState:
                stopCamera();
                unloadCamera();
                if (m_changeCaptureModeWhenReady) {
                    setCaptureMode(m_requestedCaptureMode);
                    m_changeCaptureModeWhenReady = false; // Reset
                }
                break;
            case QCamera::LoadedState:
                stopCamera();
                if (m_changeCaptureModeWhenReady) {
                    setCaptureMode(m_requestedCaptureMode);
                    m_changeCaptureModeWhenReady = false; // Reset
                }
                break;
            case QCamera::ActiveState:
                if (m_changeCaptureModeWhenReady) {
                    setCaptureMode(m_requestedCaptureMode);
                    m_changeCaptureModeWhenReady = false; // Reset
                }
                startCamera();
                break;

            default:
                setError(KErrGeneral, QString("Unexpected camera error."));
                return;
        }

        emit statusChanged(m_internalState);
    }
}

void S60CameraControl::advancedSettingsCreated()
{
    m_advancedSettings = m_imageSession->advancedSettings();

    if (m_advancedSettings)
        connect(m_advancedSettings, SIGNAL(error(int, const QString&)), this, SIGNAL(error(int, const QString&)));
}

void S60CameraControl::MceoCameraReady()
{
    if (m_internalState != QCamera::LoadedStatus) {

        switch (m_requestedState) {
            case QCamera::UnloadedState:
                m_internalState = QCamera::LoadedStatus;
                emit statusChanged(QCamera::LoadedStatus);

                stopCamera();
                unloadCamera();

                if (m_changeCaptureModeWhenReady) {
                    setCaptureMode(m_requestedCaptureMode);
                    m_changeCaptureModeWhenReady = false; // Reset
                }
                break;

            case QCamera::LoadedState:
                if (m_captureMode == QCamera::CaptureVideo) {
                    int prepareSuccess = m_videoSession->initializeVideoRecording();
                    setError(prepareSuccess, QString("Loading video capture failed."));

                    // State change signalled when reservation is complete (in videoStateChanged())
                    return;
                }
                m_internalState = QCamera::LoadedStatus;
                emit statusChanged(QCamera::LoadedStatus);

                if (m_changeCaptureModeWhenReady) {
                    setCaptureMode(m_requestedCaptureMode);
                    m_changeCaptureModeWhenReady = false; // Reset
                }
                break;

            case QCamera::ActiveState:
                if (m_captureMode == QCamera::CaptureVideo) {
                    int prepareSuccess = m_videoSession->initializeVideoRecording();
                    setError(prepareSuccess, QString("Loading video capture failed."));

                    // State change signalled when reservation is complete (in videoStateChanged())
                    return;
                }

                m_internalState = QCamera::LoadedStatus;
                emit statusChanged(QCamera::LoadedStatus);

                if (m_changeCaptureModeWhenReady) {
                    setCaptureMode(m_requestedCaptureMode);
                    m_changeCaptureModeWhenReady = false; // Reset
                }
                startCamera();
                break;

            default:
                setError(KErrGeneral, QString("Unexpected camera error."));
                return;
        }
    }
}

void S60CameraControl::MceoHandleError(TCameraEngineError aErrorType, TInt aError)
{
    Q_UNUSED(aErrorType);

    if (aError == KErrAccessDenied) {
        setError(KErrGeneral, QString("Access to camera device was rejected."));
    }
    else
        setError(aError, QString("Unexpected camera error."));
}

void S60CameraControl::setError(const TInt error, const QString &description)
{
    if (error == KErrNone)
        return;

    m_error = error;
    QCamera::Error cameraError = fromSymbianErrorToQtMultimediaError(m_error);

    emit this->error(cameraError, description);

    // Reset everything, if other than not supported error
    if ((QCamera::Error)cameraError != QCamera::NotSupportedFeatureError)
        resetCamera();
    else
        m_error = KErrNone; // Reset error
}

QCamera::Error S60CameraControl::fromSymbianErrorToQtMultimediaError(int aError)
{
    switch(aError) {
        case KErrNone:
            return QCamera::NoError; // No errors have occurred

        case KErrNotSupported:
            return QCamera::NotSupportedFeatureError; // The feature is not supported
        case KErrNotFound:
        case KErrBadHandle:
            return QCamera::ServiceMissingError; // No camera service available
        case KErrArgument:
        case KErrNotReady:
            return QCamera::InvalidRequestError; // Invalid parameter or state

        default:
            return QCamera::CameraError; // An error has occurred (i.e. General Error)
    }
}

// For S60CameraVideoDeviceControl
int S60CameraControl::deviceCount()
{
#ifdef Q_CC_NOKIAX86 // Emulator
    return 1;
#endif

    return CCameraEngine::CamerasAvailable();
}

int S60CameraControl::defaultDevice() const
{
    return KDefaultCameraDevice;
}

int S60CameraControl::selectedDevice() const
{
    return m_deviceIndex;
}

void S60CameraControl::setSelectedDevice(const int index)
{
    if (m_deviceIndex != index) {
        m_deviceIndex = index;
        resetCamera();
    }
}

QString S60CameraControl::name(const int index)
{
    QString cameraName;
    switch (index) {
        case 0:
            cameraName = QLatin1String("Primary camera");
            break;
        case 1:
            cameraName = QLatin1String("Secondary camera");
            break;
        case 2:
            cameraName = QLatin1String("Tertiary camera");
            break;

        default:
            cameraName = QLatin1String("Camera");
            break;
    }

    return cameraName;
}

QString S60CameraControl::description(const int index)
{
    QString cameraDesc;
    switch (index) {
        case 0:
            cameraDesc = QLatin1String("Device primary camera");
            break;
        case 1:
            cameraDesc = QLatin1String("Device secondary camera");
            break;
        case 2:
            cameraDesc = QLatin1String("Device tertiary camera");
            break;

        default:
            cameraDesc = QLatin1String("Camera");
            break;
    }

    return cameraDesc;
}

void S60CameraControl::resetCamera()
{
    // Cancel ongoing activity
    m_imageSession->cancelCapture();
    m_videoSession->stopRecording();

    // Advanced settings must be destructed before the camera
    m_imageSession->deleteAdvancedSettings();

    // Release resources
    stopCamera();
    unloadCamera();

    disconnect(m_viewfinderEngine, SIGNAL(error(int, const QString&)), this, SIGNAL(error(int,const QString&)));
    if (m_viewfinderEngine) {
        delete m_viewfinderEngine;
        m_viewfinderEngine = NULL;
    }

    if (m_cameraEngine) {
        delete m_cameraEngine;
        m_cameraEngine = NULL;
    }

    TRAPD(err, m_cameraEngine = CCameraEngine::NewL(m_deviceIndex, 0, this));
    if (err) {
        setError(err, QString("Camera device creation failed."));
        return;
    }

    // Notify list of available camera devices has been updated
    emit devicesChanged();

    m_viewfinderEngine = new S60CameraViewfinderEngine(this, m_cameraEngine);
    if (m_viewfinderEngine == NULL) {
        setError(KErrNoMemory, QString("Viewfinder device creation failed."));
    }
    connect(m_viewfinderEngine, SIGNAL(error(int, const QString&)), this, SIGNAL(error(int,const QString&)));

    setCameraHandles();

    // Reset state
    //setState(QCamera::UnloadedState);
    if (m_internalState != QCamera::UnloadedStatus) {
        m_internalState = QCamera::UnloadedStatus;
        emit statusChanged(m_internalState);
    }
    if (m_requestedState != QCamera::UnloadedState) {
        m_requestedState = QCamera::UnloadedState;
        emit stateChanged(m_requestedState);
    }

    // Reset error
    m_error = KErrNone;
}

/*
 * Reset everything else than viewfinder engine and errors.
 */
CCameraEngine *S60CameraControl::resetCameraOrientation()
{
    // Check if video is recording
    if (m_videoCaptureState == S60VideoCaptureSession::ERecording ||
        m_videoCaptureState == S60VideoCaptureSession::EPaused) {
        // ViewfinderEngine expects CameraEngine, so return the current one
        return m_cameraEngine;
    }

    QCamera::State originalState = m_requestedState;

    // Cancel ongoing activity
    m_imageSession->cancelCapture();
    m_videoSession->stopRecording();

    // Advanced settings must be destructed before the camera
    m_imageSession->deleteAdvancedSettings();

    // Release resources
    stopCamera();
    unloadCamera();

    if (m_cameraEngine) {
        delete m_cameraEngine;
        m_cameraEngine = NULL;
    }

    TRAPD(err, m_cameraEngine = CCameraEngine::NewL(m_deviceIndex, 0, this));
    if (err) {
        setError(err, QString("Camera device creation failed."));
        return NULL;
    }

    // Notify list of available camera devices has been updated
    emit devicesChanged();

    setCameraHandles();

    // Reset state
    if (m_internalState != QCamera::UnloadedStatus) {
        m_internalState = QCamera::UnloadedStatus;
        emit statusChanged(m_internalState);
    }
    if (m_requestedState != QCamera::UnloadedState) {
        m_requestedState = QCamera::UnloadedState;
        emit stateChanged(m_requestedState);
    }

    setState(originalState);

    return m_cameraEngine;
}

void S60CameraControl::setCameraHandles()
{
    m_imageSession->setCurrentDevice(m_deviceIndex);
    m_imageSession->setCameraHandle(m_cameraEngine);
    m_cameraEngine->SetImageCaptureObserver(m_imageSession);
    m_videoSession->setCameraHandle(m_cameraEngine);
}

// End of file