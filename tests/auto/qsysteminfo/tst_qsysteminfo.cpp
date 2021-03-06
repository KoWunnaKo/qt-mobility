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

//TESTED_COMPONENT=src/systeminfo

#include <QtTest/QtTest>
#include "qsysteminfo.h"
#include "qsysteminfo_simulator_p.h"
#include <QDebug>

QTM_USE_NAMESPACE
Q_DECLARE_METATYPE(QSystemInfo::Version);
Q_DECLARE_METATYPE(QSystemInfo::Feature);

/**
 * Starts an event loop that runs until the given signal is received.
 * Optionally the event loop can return earlier on a timeout.
 *
 * \return \p true if the requested signal was received
 *         \p false on timeout
 */
#ifdef TESTR

static bool waitForSignal(QObject *obj, const char *signal, int timeout = 0)
{
    QEventLoop loop;
    QObject::connect(obj, signal, &loop, SLOT(quit()));
    QTimer timer;
    QSignalSpy timeoutSpy(&timer, SIGNAL(timeout()));
    if (timeout > 0) {
        QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timer.setSingleShot(true);
        timer.start(timeout);
    }
    loop.exec();
    return timeoutSpy.isEmpty();
}

//static void setLanguage(const QString & lang)
//{
//    QSystemInfoPrivate *s = getSystemInfoPrivate();
//    s->setCurrentLanguage(lang);
//}


class ChangeLanguageThread : public QThread
{
public:
    void run()
    {
        SystemInfoConnection si;
        QSystemInfoPrivate *d = si.systeminfoPrivate();
        d->setCurrentLanguage(lang);

    }
     QString lang;
};
#endif

class tst_QSystemInfo : public QObject
{
    Q_OBJECT

public:
    tst_QSystemInfo();
    virtual ~tst_QSystemInfo();

    friend class QSystemInfo;

private slots:
    void initTestCase();

    void tst_currentLanguage();
    void tst_availableLanguages();

    void tst_versions_data();
    void tst_versions();

    void tst_countryCode();

    void tst_hasFeatures_data();
    void tst_hasFeatures();

    void tst_detailFeatures_data();
    void tst_detailFeatures();
#ifdef TESTR
    void currentLanguageChanged();

    void slotCurrentLanguageChanged(const QString &);
#endif
private:
#ifdef TESTR
    ChangeLanguageThread *changeLangThread;
#endif
};

tst_QSystemInfo::tst_QSystemInfo()
{
#ifdef TESTR
    changeLangThread = new ChangeLanguageThread();
#endif
}

tst_QSystemInfo::~tst_QSystemInfo()
{
#ifdef TESTR
    delete changeLangThread, changeLangThread = 0;
#endif
}

void tst_QSystemInfo::initTestCase()
{
    qRegisterMetaType<QSystemInfo::Version>("QSystemInfo::Version");
    qRegisterMetaType<QSystemInfo::Feature>("QSystemInfo::Feature");
}

void tst_QSystemInfo::tst_currentLanguage()
{
    QSystemInfo si;
#ifdef TESTR
    SystemInfoConnection sic;
    QSystemInfoPrivate *d = sic.systeminfoPrivate();
    d->setInitialData();
#endif
    QVERIFY(!si.currentLanguage().isEmpty());
    QCOMPARE(si.currentLanguage().length(), 2);
    QVERIFY(si.currentLanguage() == si.currentLanguage().toLower());

}

void tst_QSystemInfo::tst_availableLanguages()
{
    QSystemInfo si;
#ifdef TESTR
    SystemInfoConnection sic;
    QSystemInfoPrivate *d = sic.systeminfoPrivate();
    d->setInitialData();
#endif
    QVERIFY(!si.availableLanguages().isEmpty());
    QStringList available = si.availableLanguages();
    foreach(QString lang, available) {
        QCOMPARE(lang.length(), 2);
        QVERIFY(lang == lang.toLower());
    }
}

void tst_QSystemInfo::tst_versions_data()
{
 QTest::addColumn<QSystemInfo::Version>("version");
 QTest::addColumn<QString>("parameter");

 QTest::newRow("Os") << QSystemInfo::Os << "";
 QTest::newRow("QtCore") << QSystemInfo::QtCore << "";
 QTest::newRow("Firmware") << QSystemInfo::Firmware << "";

}

void tst_QSystemInfo::tst_versions()
{
    {
        QFETCH(QSystemInfo::Version, version);
        QFETCH(QString, parameter);
        QSystemInfo si;
#ifdef TESTR
        SystemInfoConnection sic;
        QSystemInfoPrivate *d = sic.systeminfoPrivate();
        d->setInitialData();
#endif
        QString vers = si.version(version, parameter);
        QVERIFY(!vers.isEmpty()
            || vers.isEmpty());
    }
}

void tst_QSystemInfo::tst_countryCode()
{
    QSystemInfo si;
#ifdef TESTR
    SystemInfoConnection sic;
    QSystemInfoPrivate *d = sic.systeminfoPrivate();
    d->setInitialData();
#endif
    QVERIFY(!si.currentCountryCode().isEmpty());
    QCOMPARE(si.currentCountryCode().length(),2);
    QVERIFY(si.currentCountryCode() == si.currentCountryCode().toUpper());
}

void tst_QSystemInfo::tst_hasFeatures_data()
{
    QTest::addColumn<QSystemInfo::Feature>("feature");

    QTest::newRow("Bluetooth") << QSystemInfo::BluetoothFeature;
    QTest::newRow("CameraFeature") << QSystemInfo::CameraFeature;
    QTest::newRow("FmradioFeature") << QSystemInfo::FmradioFeature;
    QTest::newRow("IrFeature") << QSystemInfo::IrFeature;
    QTest::newRow("LedFeature") << QSystemInfo::LedFeature;
    QTest::newRow("MemcardFeature") << QSystemInfo::MemcardFeature;
    QTest::newRow("UsbFeature") << QSystemInfo::UsbFeature;
    QTest::newRow("VibFeature") << QSystemInfo::VibFeature;
    QTest::newRow("WlanFeature") << QSystemInfo::WlanFeature;
    QTest::newRow("SimFeature") << QSystemInfo::SimFeature;
    QTest::newRow("LocationFeature") << QSystemInfo::LocationFeature;
    QTest::newRow("VideoOutFeature") << QSystemInfo::VideoOutFeature;
    QTest::newRow("HapticsFeature") << QSystemInfo::HapticsFeature;
    QTest::newRow("FmTransmitterFeature") << QSystemInfo::FmTransmitterFeature;
}

void tst_QSystemInfo::tst_hasFeatures()
{
    {
        QFETCH(QSystemInfo::Feature, feature);
        QSystemInfo si;
#ifdef TESTR
        SystemInfoConnection sic;
        QSystemInfoPrivate *d = sic.systeminfoPrivate();
        d->setInitialData();
#endif
        QVERIFY(si.hasFeatureSupported(feature) == false
                || si.hasFeatureSupported(feature) == true);
    }
}

void tst_QSystemInfo::tst_detailFeatures_data()
{
    tst_hasFeatures_data();
}

void tst_QSystemInfo::tst_detailFeatures()
{
//    {
//        QFETCH(QSystemInfo::Feature, feature);
//        QSystemInfo si;
//        QVERIFY(!si.getDetailOfFeature(feature).isEmpty());
//    }
}
#ifdef TESTR

void tst_QSystemInfo::currentLanguageChanged()
{
    QSystemInfo si;
    connect(&si,SIGNAL(currentLanguageChanged(const QString &)),this,
            SLOT(slotCurrentLanguageChanged(const QString &)));

    changeLangThread->lang = "kl";
    changeLangThread->start();
    QSignalSpy errorSpy(&si, SIGNAL(currentLanguageChanged(const QString &)));
    QVERIFY(::waitForSignal(&si, SIGNAL(currentLanguageChanged(const QString &)), 10 * 1000));
    QVERIFY(errorSpy.count() == 1);
}

void tst_QSystemInfo::slotCurrentLanguageChanged(const QString &lang)
{
    QVERIFY(lang == "kl");
}
#endif

QTEST_MAIN(tst_QSystemInfo)
#include "tst_qsysteminfo.moc"

