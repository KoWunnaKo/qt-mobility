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

//TESTED_COMPONENT=src/multimedia

#include <QtTest/QtTest>
#include <QtGui/QImageReader>
#include <QDebug>

#include <qabstractvideosurface.h>
#include <qcameracontrol.h>
#include <qcameralockscontrol.h>
#include <qcameraexposurecontrol.h>
#include <qcameraflashcontrol.h>
#include <qcamerafocuscontrol.h>
#include <qcameraimagecapturecontrol.h>
#include <qimageencodercontrol.h>
#include <qcameraimageprocessingcontrol.h>
#include <qcameracapturebufferformatcontrol.h>
#include <qcameracapturedestinationcontrol.h>
#include <qmediaservice.h>
#include <qcamera.h>
#include <qcameraimagecapture.h>
#include <qgraphicsvideoitem.h>
#include <qvideorenderercontrol.h>
#include <qvideowidget.h>
#include <qvideowindowcontrol.h>

QT_USE_NAMESPACE

// Eventually these will make it into qtestcase.h
// but we might need to tweak the timeout values here.
#ifndef QTRY_COMPARE
#define QTRY_COMPARE(__expr, __expected) \
    do { \
        const int __step = 50; \
        const int __timeout = 10000; \
        if ((__expr) != (__expected)) { \
            QTest::qWait(0); \
        } \
        for (int __i = 0; __i < __timeout && ((__expr) != (__expected)); __i+=__step) { \
            QTest::qWait(__step); \
        } \
        QCOMPARE(__expr, __expected); \
    } while(0)
#endif

#ifndef QTRY_VERIFY
#define QTRY_VERIFY(__expr) \
        do { \
        const int __step = 50; \
        const int __timeout = 10000; \
        if (!(__expr)) { \
            QTest::qWait(0); \
        } \
        for (int __i = 0; __i < __timeout && !(__expr); __i+=__step) { \
            QTest::qWait(__step); \
        } \
        QVERIFY(__expr); \
    } while(0)
#endif


#define QTRY_WAIT(code, __expr) \
        do { \
        const int __step = 50; \
        const int __timeout = 10000; \
        if (!(__expr)) { \
            QTest::qWait(0); \
        } \
        for (int __i = 0; __i < __timeout && !(__expr); __i+=__step) { \
            do { code } while(0); \
            QTest::qWait(__step); \
        } \
    } while(0)


/*
 This is the backend conformance test.

 Since it relies on platform media framework and sound hardware
 it may be less stable.
*/


Q_DECLARE_METATYPE(QtMultimediaKit::MetaData)
Q_DECLARE_METATYPE(QVideoFrame)

class tst_QCameraBackend: public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void testAvailableDevices();
    void testDeviceDescription();
    void testCtorWithDevice();

    void testCameraStates();
    void testCaptureMode();
    void testCameraCapture();
    void testCaptureToBuffer();
    void testCameraCaptureMetadata();
    void testExposureCompensation();
    void testExposureMode();
private:
};

void tst_QCameraBackend::initTestCase()
{
    qRegisterMetaType<QtMultimediaKit::MetaData>("QtMultimediaKit::MetaData");

    QCamera camera;
    if (!camera.isAvailable())
        QSKIP("Camera is not available", SkipAll);
}

void tst_QCameraBackend::cleanupTestCase()
{
}

void tst_QCameraBackend::testAvailableDevices()
{
    int deviceCount = QMediaServiceProvider::defaultServiceProvider()->devices(QByteArray(Q_MEDIASERVICE_CAMERA)).count();
    QCOMPARE(QCamera::availableDevices().count(), deviceCount);
}

void tst_QCameraBackend::testDeviceDescription()
{
    int deviceCount = QMediaServiceProvider::defaultServiceProvider()->devices(QByteArray(Q_MEDIASERVICE_CAMERA)).count();

    if (deviceCount == 0)
        QVERIFY(QCamera::deviceDescription(QByteArray("random")).isNull());
    else {
        foreach (const QByteArray &device, QCamera::availableDevices())
            QVERIFY(QCamera::deviceDescription(device).length() > 0);
    }
}

void tst_QCameraBackend::testCtorWithDevice()
{
    int deviceCount = QMediaServiceProvider::defaultServiceProvider()->devices(QByteArray(Q_MEDIASERVICE_CAMERA)).count();
    QCamera *camera = 0;

    if (deviceCount == 0) {
        camera = new QCamera("random");
        QCOMPARE(camera->error(), QCamera::ServiceMissingError);
    }
    else {
        camera = new QCamera(QCamera::availableDevices().first());
        QCOMPARE(camera->error(), QCamera::NoError);
    }

    delete camera;
}

void tst_QCameraBackend::testCameraStates()
{
    QCamera camera;
    QCameraImageCapture imageCapture(&camera);

    QSignalSpy errorSignal(&camera, SIGNAL(error(QCamera::Error)));
    QSignalSpy stateChangedSignal(&camera, SIGNAL(stateChanged(QCamera::State)));
    QSignalSpy statusChangedSignal(&camera, SIGNAL(statusChanged(QCamera::Status)));

    QCOMPARE(camera.state(), QCamera::UnloadedState);
    QCOMPARE(camera.status(), QCamera::UnloadedStatus);

    camera.load();
    QCOMPARE(camera.state(), QCamera::LoadedState);
    QCOMPARE(stateChangedSignal.count(), 1);
    QCOMPARE(stateChangedSignal.last().first().value<QCamera::State>(), QCamera::LoadedState);
    QVERIFY(stateChangedSignal.count() > 0);

    QTRY_COMPARE(camera.status(), QCamera::LoadedStatus);
    QCOMPARE(statusChangedSignal.last().first().value<QCamera::Status>(), QCamera::LoadedStatus);

    camera.unload();
    QCOMPARE(camera.state(), QCamera::UnloadedState);
    QCOMPARE(stateChangedSignal.last().first().value<QCamera::State>(), QCamera::UnloadedState);
    QTRY_COMPARE(camera.status(), QCamera::UnloadedStatus);
    QCOMPARE(statusChangedSignal.last().first().value<QCamera::Status>(), QCamera::UnloadedStatus);

#ifdef Q_WS_MAEMO_6
    //resource policy doesn't work correctly when resource is released and immediately requested again.
    QTest::qWait(250);
#endif

    camera.start();
    QCOMPARE(camera.state(), QCamera::ActiveState);
    QCOMPARE(stateChangedSignal.last().first().value<QCamera::State>(), QCamera::ActiveState);
    QTRY_COMPARE(camera.status(), QCamera::ActiveStatus);
    QCOMPARE(statusChangedSignal.last().first().value<QCamera::Status>(), QCamera::ActiveStatus);

    camera.stop();
    QCOMPARE(camera.state(), QCamera::LoadedState);
    QCOMPARE(stateChangedSignal.last().first().value<QCamera::State>(), QCamera::LoadedState);
    QTRY_COMPARE(camera.status(), QCamera::LoadedStatus);
    QCOMPARE(statusChangedSignal.last().first().value<QCamera::Status>(), QCamera::LoadedStatus);

    camera.unload();
    QCOMPARE(camera.state(), QCamera::UnloadedState);
    QCOMPARE(stateChangedSignal.last().first().value<QCamera::State>(), QCamera::UnloadedState);
    QTRY_COMPARE(camera.status(), QCamera::UnloadedStatus);
    QCOMPARE(statusChangedSignal.last().first().value<QCamera::Status>(), QCamera::UnloadedStatus);

    QCOMPARE(camera.errorString(), QString());
    QCOMPARE(errorSignal.count(), 0);
}

void tst_QCameraBackend::testCaptureMode()
{
    QCamera camera;

    QSignalSpy errorSignal(&camera, SIGNAL(error(QCamera::Error)));
    QSignalSpy stateChangedSignal(&camera, SIGNAL(stateChanged(QCamera::State)));
    QSignalSpy captureModeSignal(&camera, SIGNAL(captureModeChanged(QCamera::CaptureMode)));

    QCOMPARE(camera.captureMode(), QCamera::CaptureStillImage);

    if (!camera.isCaptureModeSupported(QCamera::CaptureVideo)) {
        camera.setCaptureMode(QCamera::CaptureVideo);
        QCOMPARE(camera.captureMode(), QCamera::CaptureStillImage);
        QSKIP("Video capture not supported", SkipAll);
    }

    camera.setCaptureMode(QCamera::CaptureVideo);
    QCOMPARE(camera.captureMode(), QCamera::CaptureVideo);
    QTRY_COMPARE(captureModeSignal.size(), 1);
    QCOMPARE(captureModeSignal.last().first().value<QCamera::CaptureMode>(), QCamera::CaptureVideo);
    captureModeSignal.clear();

    camera.load();
    QTRY_COMPARE(camera.status(), QCamera::LoadedStatus);
    //capture mode should still be video
    QCOMPARE(camera.captureMode(), QCamera::CaptureVideo);

    //it should be possible to switch capture mode in Loaded state
    camera.setCaptureMode(QCamera::CaptureStillImage);
    QTRY_COMPARE(captureModeSignal.size(), 1);
    QCOMPARE(captureModeSignal.last().first().value<QCamera::CaptureMode>(), QCamera::CaptureStillImage);
    captureModeSignal.clear();

    camera.setCaptureMode(QCamera::CaptureVideo);
    QTRY_COMPARE(captureModeSignal.size(), 1);
    QCOMPARE(captureModeSignal.last().first().value<QCamera::CaptureMode>(), QCamera::CaptureVideo);
    captureModeSignal.clear();

    camera.start();
    QTRY_COMPARE(camera.status(), QCamera::ActiveStatus);
    //capture mode should still be video
    QCOMPARE(camera.captureMode(), QCamera::CaptureVideo);

    stateChangedSignal.clear();
    //it should be possible to switch capture mode in Active state
    camera.setCaptureMode(QCamera::CaptureStillImage);
    //camera may leave Active status, but should return to Active
    QTest::qWait(10); //camera may leave Active status async
    QTRY_COMPARE(camera.status(), QCamera::ActiveStatus);
    QCOMPARE(camera.captureMode(), QCamera::CaptureStillImage);
    QVERIFY2(stateChangedSignal.isEmpty(), "camera should not change the state during capture mode changes");

    QCOMPARE(captureModeSignal.size(), 1);
    QCOMPARE(captureModeSignal.last().first().value<QCamera::CaptureMode>(), QCamera::CaptureStillImage);
    captureModeSignal.clear();

    camera.setCaptureMode(QCamera::CaptureVideo);
    //camera may leave Active status, but should return to Active
    QTest::qWait(10); //camera may leave Active status async
    QTRY_COMPARE(camera.status(), QCamera::ActiveStatus);
    QCOMPARE(camera.captureMode(), QCamera::CaptureVideo);

    QVERIFY2(stateChangedSignal.isEmpty(), "camera should not change the state during capture mode changes");

    QCOMPARE(captureModeSignal.size(), 1);
    QCOMPARE(captureModeSignal.last().first().value<QCamera::CaptureMode>(), QCamera::CaptureVideo);
    captureModeSignal.clear();

    camera.stop();
    QCOMPARE(camera.captureMode(), QCamera::CaptureVideo);
    camera.unload();
    QCOMPARE(camera.captureMode(), QCamera::CaptureVideo);

    QVERIFY2(errorSignal.isEmpty(), QString("Camera error: %1").arg(camera.errorString()).toLocal8Bit());
}

void tst_QCameraBackend::testCameraCapture()
{
    QCamera camera;
    QCameraImageCapture imageCapture(&camera);
    //prevents camera to flash during the test
    camera.exposure()->setFlashMode(QCameraExposure::FlashOff);

    QVERIFY(!imageCapture.isReadyForCapture());

    QSignalSpy capturedSignal(&imageCapture, SIGNAL(imageCaptured(int,QImage)));
    QSignalSpy savedSignal(&imageCapture, SIGNAL(imageSaved(int,QString)));
    QSignalSpy errorSignal(&imageCapture, SIGNAL(error(int, QCameraImageCapture::Error,QString)));

    imageCapture.capture();
    QTRY_COMPARE(errorSignal.size(), 1);
    QCOMPARE(imageCapture.error(), QCameraImageCapture::NotReadyError);
    QCOMPARE(capturedSignal.size(), 0);
    errorSignal.clear();

    camera.start();

    QTRY_VERIFY(imageCapture.isReadyForCapture());
    QCOMPARE(camera.status(), QCamera::ActiveStatus);
    QCOMPARE(errorSignal.size(), 0);

    int id = imageCapture.capture();

    QTRY_VERIFY(!savedSignal.isEmpty());

    QCOMPARE(capturedSignal.size(), 1);
    QCOMPARE(capturedSignal.last().first().toInt(), id);
    QCOMPARE(errorSignal.size(), 0);
    QCOMPARE(imageCapture.error(), QCameraImageCapture::NoError);

    QCOMPARE(savedSignal.last().first().toInt(), id);
    QString location = savedSignal.last().last().toString();
    QVERIFY(!location.isEmpty());
    QVERIFY(QFileInfo(location).exists());
    QImageReader reader(location);
    reader.setScaledSize(QSize(320,240));
    QVERIFY(!reader.read().isNull());

    QFile(location).remove();
}


void tst_QCameraBackend::testCaptureToBuffer()
{
    QCamera camera;
    QCameraImageCapture imageCapture(&camera);
    camera.exposure()->setFlashMode(QCameraExposure::FlashOff);

    camera.load();

#ifdef Q_WS_MAEMO_6
    QVERIFY(imageCapture.isCaptureDestinationSupported(QCameraImageCapture::CaptureToBuffer));
#endif

    if (!imageCapture.isCaptureDestinationSupported(QCameraImageCapture::CaptureToBuffer))
        QSKIP("Buffer capture not supported", SkipAll);

    QTRY_COMPARE(camera.status(), QCamera::LoadedStatus);

    QCOMPARE(imageCapture.bufferFormat(), QVideoFrame::Format_Jpeg);

    QVERIFY(imageCapture.isCaptureDestinationSupported(QCameraImageCapture::CaptureToFile));
    QVERIFY(imageCapture.isCaptureDestinationSupported(QCameraImageCapture::CaptureToBuffer));
    QVERIFY(imageCapture.isCaptureDestinationSupported(
                QCameraImageCapture::CaptureToBuffer | QCameraImageCapture::CaptureToFile));

    QSignalSpy destinationChangedSignal(&imageCapture, SIGNAL(captureDestinationChanged(QCameraImageCapture::CaptureDestinations)));

    QCOMPARE(imageCapture.captureDestination(), QCameraImageCapture::CaptureToFile);
    imageCapture.setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    QCOMPARE(imageCapture.captureDestination(), QCameraImageCapture::CaptureToBuffer);
    QCOMPARE(destinationChangedSignal.size(), 1);
    QCOMPARE(destinationChangedSignal.first().first().value<QCameraImageCapture::CaptureDestinations>(),
             QCameraImageCapture::CaptureToBuffer);

    QSignalSpy capturedSignal(&imageCapture, SIGNAL(imageCaptured(int,QImage)));
    QSignalSpy imageAvailableSignal(&imageCapture, SIGNAL(imageAvailable(int,QVideoFrame)));
    QSignalSpy savedSignal(&imageCapture, SIGNAL(imageSaved(int,QString)));
    QSignalSpy errorSignal(&imageCapture, SIGNAL(error(int, QCameraImageCapture::Error,QString)));

    camera.start();
    QTRY_VERIFY(imageCapture.isReadyForCapture());

    int id = imageCapture.capture();
    QTRY_VERIFY(!imageAvailableSignal.isEmpty());

    QVERIFY(errorSignal.isEmpty());
    QVERIFY(!capturedSignal.isEmpty());
    QVERIFY(!imageAvailableSignal.isEmpty());

    QTest::qWait(2000);
    QVERIFY(savedSignal.isEmpty());

    QCOMPARE(capturedSignal.first().first().toInt(), id);
    QCOMPARE(imageAvailableSignal.first().first().toInt(), id);

    QVideoFrame frame = imageAvailableSignal.first().last().value<QVideoFrame>();
    QVERIFY(frame.isValid());
    QCOMPARE(frame.pixelFormat(), QVideoFrame::Format_Jpeg);
    QVERIFY(!frame.size().isEmpty());
    QVERIFY(frame.map(QAbstractVideoBuffer::ReadOnly));
    QByteArray data((const char *)frame.bits(), frame.mappedBytes());
    frame.unmap();
    frame = QVideoFrame();

    QVERIFY(!data.isEmpty());
    QBuffer buffer;
    buffer.setData(data);
    buffer.open(QIODevice::ReadOnly);
    QImageReader reader(&buffer, "JPG");
    reader.setScaledSize(QSize(640,480));
    QImage img(reader.read());
    QVERIFY(!img.isNull());

    capturedSignal.clear();
    imageAvailableSignal.clear();
    savedSignal.clear();

    //Capture to yuv buffer
#ifdef Q_WS_MAEMO_6
    QVERIFY(imageCapture.supportedBufferFormats().contains(QVideoFrame::Format_UYVY));
#endif

    if (imageCapture.supportedBufferFormats().contains(QVideoFrame::Format_UYVY)) {
        imageCapture.setBufferFormat(QVideoFrame::Format_UYVY);
        QCOMPARE(imageCapture.bufferFormat(), QVideoFrame::Format_UYVY);

        id = imageCapture.capture();
        QTRY_VERIFY(!imageAvailableSignal.isEmpty());

        QVERIFY(errorSignal.isEmpty());
        QVERIFY(!capturedSignal.isEmpty());
        QVERIFY(!imageAvailableSignal.isEmpty());
        QVERIFY(savedSignal.isEmpty());

        QTest::qWait(2000);
        QVERIFY(savedSignal.isEmpty());

        frame = imageAvailableSignal.first().last().value<QVideoFrame>();
        QVERIFY(frame.isValid());

        qDebug() << frame.pixelFormat();
        QCOMPARE(frame.pixelFormat(), QVideoFrame::Format_UYVY);
        QVERIFY(!frame.size().isEmpty());
        frame = QVideoFrame();

        capturedSignal.clear();
        imageAvailableSignal.clear();
        savedSignal.clear();

        imageCapture.setBufferFormat(QVideoFrame::Format_Jpeg);
        QCOMPARE(imageCapture.bufferFormat(), QVideoFrame::Format_Jpeg);
    }

    //Try to capture to both buffer and file
#ifdef Q_WS_MAEMO_6
    QVERIFY(imageCapture.isCaptureDestinationSupported(QCameraImageCapture::CaptureToBuffer | QCameraImageCapture::CaptureToFile));
#endif
    if (imageCapture.isCaptureDestinationSupported(QCameraImageCapture::CaptureToBuffer | QCameraImageCapture::CaptureToFile)) {
        imageCapture.setCaptureDestination(QCameraImageCapture::CaptureToBuffer | QCameraImageCapture::CaptureToFile);

        int oldId = id;
        id = imageCapture.capture();
        QVERIFY(id != oldId);
        QTRY_VERIFY(!savedSignal.isEmpty());

        QVERIFY(errorSignal.isEmpty());
        QVERIFY(!capturedSignal.isEmpty());
        QVERIFY(!imageAvailableSignal.isEmpty());
        QVERIFY(!savedSignal.isEmpty());

        QCOMPARE(capturedSignal.first().first().toInt(), id);
        QCOMPARE(imageAvailableSignal.first().first().toInt(), id);

        frame = imageAvailableSignal.first().last().value<QVideoFrame>();
        QVERIFY(frame.isValid());
        QCOMPARE(frame.pixelFormat(), QVideoFrame::Format_Jpeg);
        QVERIFY(!frame.size().isEmpty());

        QString fileName = savedSignal.first().last().toString();
        QVERIFY(QFileInfo(fileName).exists());
    }
}

void tst_QCameraBackend::testCameraCaptureMetadata()
{
#ifndef Q_WS_MAEMO_6
    QSKIP("Capture metadata is supported only on harmattan", SkipAll);
#endif

    QCamera camera;
    QCameraImageCapture imageCapture(&camera);
    camera.exposure()->setFlashMode(QCameraExposure::FlashOff);

    QSignalSpy metadataSignal(&imageCapture, SIGNAL(imageMetadataAvailable(int,QtMultimediaKit::MetaData,QVariant)));
    QSignalSpy savedSignal(&imageCapture, SIGNAL(imageSaved(int,QString)));

    camera.start();

    QTRY_VERIFY(imageCapture.isReadyForCapture());

    int id = imageCapture.capture(QString::fromLatin1("/dev/null"));
    QTRY_VERIFY(!savedSignal.isEmpty());
    QVERIFY(!metadataSignal.isEmpty());
    QCOMPARE(metadataSignal.first().first().toInt(), id);
}

void tst_QCameraBackend::testExposureCompensation()
{
#if !defined(Q_WS_MAEMO_6) && !defined(Q_WS_MAEMO_5) && !defined(Q_OS_SYMBIAN)
    QSKIP("Capture exposure parameters are supported only on mobile platforms", SkipAll);
#endif

    QCamera camera;
    QCameraExposure *exposure = camera.exposure();

    QSignalSpy exposureCompensationSignal(exposure, SIGNAL(exposureCompensationChanged(qreal)));

#ifdef Q_OS_SYMBIAN
    // Camera needs to be started, see: QTMOBILITY-1566
    camera.load();
    QTRY_COMPARE(camera.status(), QCamera::LoadedStatus);
#endif // Q_OS_SYMBIAN

    //it should be possible to set exposure parameters in Unloaded state
    QCOMPARE(exposure->exposureCompensation()+1.0, 1.0);
    exposure->setExposureCompensation(1.0);
    QCOMPARE(exposure->exposureCompensation(), 1.0);
    QTRY_COMPARE(exposureCompensationSignal.count(), 1);
    QCOMPARE(exposureCompensationSignal.last().first().toReal(), 1.0);

    //exposureCompensationChanged should not be emitted when value is not changed
    exposure->setExposureCompensation(1.0);
    QTest::qWait(50);
    QCOMPARE(exposureCompensationSignal.count(), 1);

    //exposure compensation should be preserved during load/start
    camera.load();
    QTRY_COMPARE(camera.status(), QCamera::LoadedStatus);

    QCOMPARE(exposure->exposureCompensation(), 1.0);

    exposureCompensationSignal.clear();
    exposure->setExposureCompensation(-1.0);
    QCOMPARE(exposure->exposureCompensation(), -1.0);
    QTRY_COMPARE(exposureCompensationSignal.count(), 1);
    QCOMPARE(exposureCompensationSignal.last().first().toReal(), -1.0);

    camera.start();
    QTRY_COMPARE(camera.status(), QCamera::ActiveStatus);

    QCOMPARE(exposure->exposureCompensation(), -1.0);

    exposureCompensationSignal.clear();
    exposure->setExposureCompensation(1.0);
    QCOMPARE(exposure->exposureCompensation(), 1.0);
    QTRY_COMPARE(exposureCompensationSignal.count(), 1);
    QCOMPARE(exposureCompensationSignal.last().first().toReal(), 1.0);
}

void tst_QCameraBackend::testExposureMode()
{
#if !defined(Q_WS_MAEMO_6) && !defined(Q_WS_MAEMO_5) && !defined(Q_OS_SYMBIAN)
    QSKIP("Capture exposure parameters are supported only on mobile platforms", SkipAll);
#endif

    QCamera camera;
    QCameraExposure *exposure = camera.exposure();

#ifdef Q_WS_MAEMO_6
    QEXPECT_FAIL("", "Camerabin reports Manual exposure instead of Auto", Continue);
#endif
    QCOMPARE(exposure->exposureMode(), QCameraExposure::ExposureAuto);

#ifdef Q_OS_SYMBIAN
    // Camera needs to be started, see: QTMOBILITY-1566
    camera.load();
    QTRY_COMPARE(camera.status(), QCamera::LoadedStatus);
#endif // Q_OS_SYMBIAN

    // Night
    exposure->setExposureMode(QCameraExposure::ExposureNight);
    QCOMPARE(exposure->exposureMode(), QCameraExposure::ExposureNight);
    camera.start();
    QTRY_COMPARE(camera.status(), QCamera::ActiveStatus);
    QCOMPARE(exposure->exposureMode(), QCameraExposure::ExposureNight);

    camera.unload();
    QTRY_COMPARE(camera.status(), QCamera::UnloadedStatus);

#ifdef Q_OS_SYMBIAN
    // Camera needs to be started, see: QTMOBILITY-1566
    camera.load();
    QTRY_COMPARE(camera.status(), QCamera::LoadedStatus);
#endif // Q_OS_SYMBIAN

#ifdef Q_WS_MAEMO_6
    //resource policy doesn't work correctly when resource is released and immediately requested again.
    QTest::qWait(250);
#endif

    // Auto
    exposure->setExposureMode(QCameraExposure::ExposureAuto);
    QCOMPARE(exposure->exposureMode(), QCameraExposure::ExposureAuto);
    camera.start();
    QTRY_COMPARE(camera.status(), QCamera::ActiveStatus);
    QCOMPARE(exposure->exposureMode(), QCameraExposure::ExposureAuto);
}

QTEST_MAIN(tst_QCameraBackend)

#include "tst_qcamerabackend.moc"
