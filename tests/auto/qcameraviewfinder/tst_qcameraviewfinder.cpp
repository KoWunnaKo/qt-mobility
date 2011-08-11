/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
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



#include <QtTest/QtTest>
#include <QDebug>
#include "qcameraviewfinder.h"
#include "qcamera.h"
#include "qmediaobject.h"

class tst_QCameraViewFinder : public QObject
{
    Q_OBJECT
public slots:
    void initTestCase();
    void cleanupTestCase(){}

private slots:
    void testConstructor();
    void testMediaObject();

private:
    QCamera *camera;
    QCameraViewfinder *viewFinder;
};

void tst_QCameraViewFinder::initTestCase()
{
    camera = new QCamera();
    viewFinder = new QCameraViewfinder();
}

void tst_QCameraViewFinder::testConstructor()
{
    /* Verify whether the object is created or not */
    QVERIFY(viewFinder != NULL);
    QCOMPARE(viewFinder->isVisible(),false);
    QCOMPARE(viewFinder->isEnabled(),true);
    viewFinder->show();
}

void tst_QCameraViewFinder::testMediaObject()
{
    QVERIFY(viewFinder != NULL);
    viewFinder->show();
    /* Sets the QVideoWidget  based camera viewfinder.*/
    camera->setViewfinder(viewFinder);
    QCOMPARE(viewFinder->isVisible(),true);

    /* Return the currently attached media object.*/
    QMediaObject *media = viewFinder->mediaObject();

    /* Verifying the object */
    QCOMPARE(media, camera);
}

QTEST_MAIN(tst_QCameraViewFinder)
#include "tst_qcameraviewfinder.moc"
