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

#include "qnearfieldmanager_maemo6_p.h"
#include "qnearfieldtarget_maemo6_p.h"
#include "manager_interface.h"
#include "maemo6/adapter_interface_p.h"
#include "maemo6/target_interface_p.h"
#include "maemo6/tag_interface_p.h"
#include "ndefhandler_adaptor.h"
#include "accessrequestor_adaptor.h"

#include <qnearfieldtagtype1.h>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusServiceWatcher>

using namespace com::nokia::nfc;

QTM_BEGIN_NAMESPACE

static QAtomicInt handlerId = 0;
static const char * const registeredHandlerPath = "/com/nokia/nfc/ndefhandler";
static const char * const accessRequesterPath = "/com/nokia/nfc/accessRequester/";
static const char * const connectionUuid = "46a15ee4-b5ce-4395-9d76-c440cc3838c6";

static inline bool matchesTarget(QNearFieldTarget::Type type,
                                 const QList<QNearFieldTarget::Type> &types)
{
    return types.contains(type) || types.contains(QNearFieldTarget::AnyTarget);
}

static QStringList accessModesToKind(QNearFieldManager::TargetAccessModes accessModes)
{
    QStringList kind;

    if (accessModes & QNearFieldManager::NdefReadTargetAccess)
        kind.append(QLatin1String("tag.ndef.read"));

    if (accessModes & QNearFieldManager::NdefWriteTargetAccess)
        kind.append(QLatin1String("tag.ndef.write"));

    if (accessModes & QNearFieldManager::TagTypeSpecificTargetAccess)
        kind.append(QLatin1String("tag.raw"));

    return kind;
}

static QNearFieldManager::TargetAccessMode kindToAccessMode(const QString &kind)
{
    if (kind == QLatin1String("tag.ndef.read"))
        return QNearFieldManager::NdefReadTargetAccess;
    if (kind == QLatin1String("tag.ndef.write"))
        return QNearFieldManager::NdefWriteTargetAccess;
    if (kind == QLatin1String("tag.raw"))
        return QNearFieldManager::TagTypeSpecificTargetAccess;

    return QNearFieldManager::NoTargetAccess;
}

NdefHandler::NdefHandler(QNearFieldManagerPrivateImpl *manager, const QString &serviceName,
                         const QString &path, QObject *object, const QMetaMethod &method)
:   m_manager(manager), m_adaptor(0), m_object(object), m_method(method),
    m_serviceName(serviceName), m_path(path)
{
    QDBusConnection handlerConnection =
        QDBusConnection::connectToBus(QDBusConnection::SystemBus, connectionUuid);
    if (serviceName != handlerConnection.baseService()) {
        handlerConnection = QDBusConnection::connectToBus(QDBusConnection::SessionBus,
                                                          serviceName);

        if (!handlerConnection.registerService(serviceName))
            return;
    }

    m_adaptor = new NDEFHandlerAdaptor(this);

    if (!handlerConnection.registerObject(path, this)) {
        delete m_adaptor;
        m_adaptor = 0;
    }
}

NdefHandler::~NdefHandler()
{
    delete m_adaptor;
}

bool NdefHandler::isValid() const
{
    return m_adaptor;
}

QString NdefHandler::serviceName() const
{
    return m_serviceName;
}

QString NdefHandler::path() const
{
    return m_path;
}

void NdefHandler::NDEFData(const QDBusObjectPath &target, const QByteArray &message)
{
    m_method.invoke(m_object, Q_ARG(QNdefMessage, QNdefMessage::fromByteArray(message)),
                    Q_ARG(QNearFieldTarget *, m_manager->targetForPath(target.path())));
}

QNearFieldManagerPrivateImpl::QNearFieldManagerPrivateImpl()
:   m_connection(QDBusConnection::connectToBus(QDBusConnection::SystemBus,
                                               QLatin1String(connectionUuid))),
    m_manager(0), m_adapter(0), m_accessAgent(0)
{
    qDBusRegisterMetaType<QList<QByteArray> >();

    m_serviceWatcher = new QDBusServiceWatcher(QLatin1String("com.nokia.nfc"), m_connection);
    connect(m_serviceWatcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)),
            this, SLOT(_q_serviceOwnerChanged(QString,QString,QString)));

    ensureConnection();
}

QNearFieldManagerPrivateImpl::~QNearFieldManagerPrivateImpl()
{
    foreach (int id, m_registeredHandlers.keys())
        unregisterNdefMessageHandler(id);

    delete m_serviceWatcher;
    delete m_manager;
    delete m_adapter;
}

bool QNearFieldManagerPrivateImpl::ensureConnection() const
{
    if (m_manager && !m_manager->isValid()) {
        delete m_manager;
        m_manager = 0;
    }

    if (!m_manager) {
        m_manager = new Manager(QLatin1String("com.nokia.nfc"), QLatin1String("/"), m_connection);

        if (!m_manager->isValid()) {
            delete m_manager;
            m_manager = 0;
            return false;
        }
    }

    if (m_adapter && !m_adapter->isValid()) {
        delete m_adapter;
        m_adapter = 0;
    }

    if (!m_adapter) {
        QDBusObjectPath defaultAdapterPath = m_manager->DefaultAdapter();

        m_adapter = new Adapter(QLatin1String("com.nokia.nfc"), defaultAdapterPath.path(),
                                m_connection);

        if (!m_adapter->isValid()) {
            delete m_adapter;
            m_adapter = 0;
            return false;
        }

        const QString requesterPath =
            QLatin1String(accessRequesterPath) + QString::number(quintptr(this));

        foreach (const QString &kind, accessModesToKind(m_requestedModes))
            m_adapter->RequestAccess(QDBusObjectPath(requesterPath), kind);
    }

    if (!m_detectTargetTypes.isEmpty()) {
        connect(m_adapter, SIGNAL(TargetDetected(QDBusObjectPath)),
                this, SLOT(_q_targetDetected(QDBusObjectPath)), Qt::UniqueConnection);
        connect(m_adapter, SIGNAL(TargetLost(QDBusObjectPath)),
                this, SLOT(_q_targetLost(QDBusObjectPath)), Qt::UniqueConnection);
    } else {
        disconnect(m_adapter, SIGNAL(TargetDetected(QDBusObjectPath)),
                   this, SLOT(_q_targetDetected(QDBusObjectPath)));
        disconnect(m_adapter, SIGNAL(TargetLost(QDBusObjectPath)),
                   this, SLOT(_q_targetLost(QDBusObjectPath)));
    }

    return true;
}

bool QNearFieldManagerPrivateImpl::isAvailable() const
{
    if (!ensureConnection())
        return false;

    return m_adapter->state() == QLatin1String("on");
}

bool QNearFieldManagerPrivateImpl::startTargetDetection(const QList<QNearFieldTarget::Type> &targetTypes)
{
    m_detectTargetTypes = targetTypes;

    // signals connected in ensureConnection()
    if (!ensureConnection())
        return false;

    const QString requesterPath =
        QLatin1String(accessRequesterPath) + QString::number(quintptr(this));

    if (!m_accessAgent) {
        m_accessAgent = new AccessRequestorAdaptor(this);
        if (!m_connection.registerObject(requesterPath, this)) {
            delete m_accessAgent;
            m_accessAgent = 0;
            return false;
        }
    }

    m_adapter->RequestAccess(QDBusObjectPath(requesterPath), "target.detect");

    return true;
}

void QNearFieldManagerPrivateImpl::stopTargetDetection()
{
    m_detectTargetTypes.clear();

    // signals disconnected in ensureConnection()
    if (!ensureConnection())
        return;

    const QString requesterPath =
        QLatin1String(accessRequesterPath) + QString::number(quintptr(this));

    m_adapter->CancelAccessRequest(QDBusObjectPath(requesterPath), "target.detect");
}

QNearFieldTarget *QNearFieldManagerPrivateImpl::targetForPath(const QString &path)
{
    QNearFieldTarget *nearFieldTarget = m_targets.value(path).data();

    if (!nearFieldTarget) {
        Target *target = new Target(QLatin1String("com.nokia.nfc"), path, m_connection);

        const QString type = target->type();

        if (type == QLatin1String("tag")) {
            Tag *tag = new Tag(QLatin1String("com.nokia.nfc"), path, m_connection);

            const QString tagTechnology = tag->technology();
            if (tagTechnology == QLatin1String("jewel"))
                nearFieldTarget = new TagType1(this, target, tag);
            else if (tagTechnology == QLatin1String("mifare-ul"))
                nearFieldTarget = new TagType2(this, target, tag);
            else if (tagTechnology == QLatin1String("felica"))
                nearFieldTarget = new TagType3(this, target, tag);
            else if (tagTechnology == QLatin1String("iso-4a"))
                nearFieldTarget = new TagType4(this, target, tag);
            else
                nearFieldTarget = new NearFieldTarget<QNearFieldTarget>(this, target, tag);
        } else if (type == QLatin1String("device")) {
            Device *device = new Device(QLatin1String("com.nokia.nfc"), path, m_connection);
            nearFieldTarget = new NearFieldTarget<QNearFieldTarget>(this, target, device);
        }

        if (nearFieldTarget)
            m_targets.insert(path, nearFieldTarget);
    }

    return nearFieldTarget;
}

int QNearFieldManagerPrivateImpl::registerNdefMessageHandler(const QString &filter,
                                                             QObject *object,
                                                             const QMetaMethod &method)
{
    if (!ensureConnection())
        return -1;

    int id = handlerId.fetchAndAddOrdered(1);
    const QString handlerPath =
        QLatin1String(registeredHandlerPath) + QLatin1Char('/') + QString::number(id);

    NdefHandler *handler = new NdefHandler(this, m_connection.baseService(),
                                           handlerPath, object, method);
    if (!handler->isValid()) {
        delete handler;
        return -1;
    }

    QDBusPendingReply<> reply =
        m_manager->RegisterNDEFHandler(QLatin1String("system"),
                                       m_connection.baseService(),
                                       QDBusObjectPath(handlerPath),
                                       QLatin1String("any"),
                                       filter,
                                       QCoreApplication::applicationName());

    if (reply.isError()) {
        delete handler;
        return -1;
    }

    m_registeredHandlers[id] = handler;

    return id;
}

int QNearFieldManagerPrivateImpl::registerNdefMessageHandler(QObject *object,
                                                             const QMetaMethod &method)
{
    if (!ensureConnection())
        return -1;

    QFileInfo fi(qApp->applicationFilePath());
    const QString serviceName = QLatin1String("com.nokia.qtmobility.nfc.") + fi.baseName();

    int id = handlerId.fetchAndAddOrdered(1);

    const QString handlerPath = QLatin1String(registeredHandlerPath);

    NdefHandler *handler = new NdefHandler(this, serviceName, handlerPath, object, method);
    if (!handler->isValid()) {
        delete handler;
        return -1;
    }

    m_registeredHandlers[id] = handler;

    return id;
}

int QNearFieldManagerPrivateImpl::registerNdefMessageHandler(const QNdefFilter &filter,
                                                             QObject *object,
                                                             const QMetaMethod &method)
{
    if (!ensureConnection())
        return -1;

    QString matchString;

    if (filter.orderMatch())
        matchString += QLatin1String("sequence:");
    else
        matchString += QLatin1String("unordered:");

    for (int i = 0; i < filter.recordCount(); ++i) {
        QNdefFilter::Record record = filter.recordAt(i);

        QString type;

        switch (record.typeNameFormat) {
        case QNdefRecord::NfcRtd:
            type = QLatin1String("urn:nfc:wkt:") + record.type;
            break;
        case QNdefRecord::ExternalRtd:
            type = QLatin1String("urn:nfc:ext:") + record.type;
            break;
        case QNdefRecord::Mime:
            type = record.type;
            break;
        default:
            qWarning("Unsupported filter type");
            return -1;
        }

        matchString += QLatin1Char('\'') + type + QLatin1Char('\'');
        matchString += QLatin1Char('[');

        if (record.minimum == UINT_MAX)
            matchString += QLatin1Char('*');
        else
            matchString += QString::number(record.minimum);

        matchString += QLatin1Char(':');

        if (record.maximum == UINT_MAX)
            matchString += QLatin1Char('*');
        else
            matchString += QString::number(record.maximum);

        matchString += QLatin1Char(']');

        if (i == filter.recordCount() - 1)
            matchString += QLatin1Char(';');
        else
            matchString += QLatin1Char(',');
    }

    return registerNdefMessageHandler(matchString, object, method);
}

bool QNearFieldManagerPrivateImpl::unregisterNdefMessageHandler(int id)
{
    if (id < 0)
        return false;

    if (!ensureConnection())
        return false;

    NdefHandler *handler = m_registeredHandlers.take(id);

    QDBusPendingReply<> reply = m_manager->UnregisterNDEFHandler(QLatin1String("system"),
                                                                 handler->serviceName(),
                                                                 QDBusObjectPath(handler->path()));

    delete handler;

    return true;
}

void QNearFieldManagerPrivateImpl::requestAccess(QNearFieldManager::TargetAccessModes accessModes)
{
    if (!ensureConnection())
        return;

    const QString requesterPath =
        QLatin1String(accessRequesterPath) + QString::number(quintptr(this));

    if (!m_accessAgent) {
        m_accessAgent = new AccessRequestorAdaptor(this);
        if (!m_connection.registerObject(requesterPath, this)) {
            delete m_accessAgent;
            m_accessAgent = 0;
            return;
        }
    }

    foreach (const QString &kind, accessModesToKind(accessModes))
        m_adapter->RequestAccess(QDBusObjectPath(requesterPath), kind);

    QNearFieldManagerPrivate::requestAccess(accessModes);
}

void QNearFieldManagerPrivateImpl::releaseAccess(QNearFieldManager::TargetAccessModes accessModes)
{
    if (!ensureConnection())
        return;

    const QString requesterPath =
        QLatin1String(accessRequesterPath) + QString::number(quintptr(this));

    foreach (const QString &kind, accessModesToKind(accessModes))
        m_adapter->CancelAccessRequest(QDBusObjectPath(requesterPath), kind);

    QNearFieldManagerPrivate::releaseAccess(accessModes);
}

void QNearFieldManagerPrivateImpl::AccessFailed(const QDBusObjectPath &target, const QString &kind,
                                                const QString &error)
{
    Q_UNUSED(target);
    Q_UNUSED(kind);
    Q_UNUSED(error);
}

void QNearFieldManagerPrivateImpl::AccessGranted(const QDBusObjectPath &target,
                                                 const QString &kind)
{
    m_grantedModesForTarget[target.path()] |= kindToAccessMode(kind);

    if (m_grantedModesForTarget.value(target.path()) == m_requestedModes) {
        m_grantedModesForTarget.remove(target.path());

        if (m_pendingDetectedTargets.contains(target.path())) {
            m_pendingDetectedTargets[target.path()].stop();
            m_pendingDetectedTargets.remove(target.path());
        }

        emitTargetDetected(target.path());
    }
}

void QNearFieldManagerPrivateImpl::timerEvent(QTimerEvent *event)
{
    QMutableMapIterator<QString, QBasicTimer> i(m_pendingDetectedTargets);
    while (i.hasNext()) {
        i.next();

        if (i.value().timerId() == event->timerId()) {
            i.value().stop();

            const QString target = i.key();

            i.remove();
            m_grantedModesForTarget.remove(target);

            emitTargetDetected(target);

            break;
        }
    }
}

void QNearFieldManagerPrivateImpl::emitTargetDetected(const QString &targetPath)
{
    QNearFieldTarget *target = targetForPath(targetPath);
    if (target && matchesTarget(target->type(), m_detectTargetTypes))
        emit targetDetected(target);
}

void QNearFieldManagerPrivateImpl::_q_targetDetected(const QDBusObjectPath &targetPath)
{
    if (!m_requestedModes) {
        emitTargetDetected(targetPath.path());
    } else {
        m_pendingDetectedTargets[targetPath.path()].start(500, this);
        m_grantedModesForTarget[targetPath.path()] = QNearFieldManager::NoTargetAccess;
    }
}

void QNearFieldManagerPrivateImpl::_q_targetLost(const QDBusObjectPath &targetPath)
{
    QNearFieldTarget *nearFieldTarget = m_targets.value(targetPath.path()).data();

    // haven't seen target so just drop this event
    if (!nearFieldTarget) {
        // We either haven't seen target (started after target was detected by system) or the
        // application deleted the target. Remove from map and don't emit anything.
        m_targets.remove(targetPath.path());
        return;
    }

    if (matchesTarget(nearFieldTarget->type(), m_detectTargetTypes))
        emit targetLost(nearFieldTarget);
}

void QNearFieldManagerPrivateImpl::_q_serviceOwnerChanged(const QString &serviceName,
                                                          const QString &oldOwner,
                                                          const QString &newOwner)
{
    if (m_adapter) {
        delete m_adapter;
        m_adapter = 0;
    }

    if (m_manager) {
        delete m_manager;
        m_manager = 0;
    }

    if (newOwner.isEmpty())
        return;

    if (!ensureConnection())
        return;
}

#include "moc_qnearfieldmanager_maemo6_p.cpp"

QTM_END_NAMESPACE
