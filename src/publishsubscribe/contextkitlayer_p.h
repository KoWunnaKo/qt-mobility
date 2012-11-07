/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
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

#ifndef CONTEXTKITLAYER_P_H
#define CONTEXTKITLAYER_P_H

#include "qvaluespace.h"
#include "qvaluespacepublisher.h"
#include "qvaluespace_p.h"

#include <QSet>
#include <QStringList>

#include <contextproperty.h>
#include <context/property.h>
#include <context/service.h>
#include <contextregistryinfo.h>

QTM_BEGIN_NAMESPACE

using namespace QValueSpace;

class ContextKitPath
{
public:
    enum PathType {
        DotPath,
        SlashPath
    };

    ContextKitPath();
    ContextKitPath(QString path);
    ContextKitPath(QString path, PathType type);
    ContextKitPath(const ContextKitPath &other);

    QString toQtPath() const;
    QString toNative() const;

    inline PathType type() const { return m_type; }

    bool includes(ContextKitPath &other) const;
    bool isRegistered() const;
    bool isProvided() const;

    inline int size() const { return parts.size(); }
    inline QString at(int i) const { return parts.at(i); }

    ContextKitPath &operator=(const ContextKitPath &other);
    bool operator==(const ContextKitPath &other) const;

    ContextKitPath operator+(const QString &str) const;
    ContextKitPath operator-(const ContextKitPath &other) const;
    ContextKitPath operator+(const ContextKitPath &other) const;

private:
    QStringList parts;
    PathType m_type;

    void initFromPath(QString path);
};

class ContextKitHandle : public QObject
{
    Q_OBJECT

public:
    ContextKitHandle(ContextKitHandle *parent, const QString &root,
                     QValueSpace::LayerOptions opts);
    ~ContextKitHandle();

    bool value(const QString &path, QVariant *data);
    bool setValue(const QString &path, const QVariant &data);
    bool unsetValue(const QString &path);
    void subscribe();
    void unsubscribe();
    QSet<QString> children();

    ContextKitPath path;

signals:
    void valueChanged();
    void interestChanged(QString path, bool status);

private:
    QHash<QString, ContextProperty*> readProps;
    QHash<QString, ContextProvider::Property*> writeProps;
    ContextProvider::Service *service;

    bool subscribed;

    void insertRead(const ContextKitPath &path);
    ContextProvider::Property *insertWrite(const ContextKitPath &path);

private slots:
    void updateSubtrees();
    void on_firstAppeared(QString key);
    void on_lastDisappeared(QString key);
};


class ContextKitLayer : public QAbstractValueSpaceLayer
{
    Q_OBJECT

public:
    ContextKitLayer();
    virtual ~ContextKitLayer();

    /* ValueSpaceLayer interface - Common functions */
    bool startup(Type);

    Handle item(Handle parent, const QString &);
    void removeHandle(Handle);
    void setProperty(Handle handle, Properties);

    bool value(Handle, QVariant *);
    bool value(Handle, const QString &, QVariant *);
    QSet<QString> children(Handle);

    /* ValueSpaceLayer interface - QValueSpaceSubscriber functions */
    bool supportsInterestNotification() const { return true; }
    bool notifyInterest(Handle handle, bool interested);

    /* ValueSpaceLayer interface - QValueSpacePublisher functions */
    bool setValue(QValueSpacePublisher *vsp, Handle handle,
                  const QString &path, const QVariant &value);
    bool removeValue(QValueSpacePublisher *vsp, Handle handle,
                     const QString &path);
    bool removeSubTree(QValueSpacePublisher *vsp, Handle handle);

    void addWatch(QValueSpacePublisher *, Handle);
    void removeWatches(QValueSpacePublisher *, Handle);
    void sync();

    virtual QValueSpace::LayerOptions layerOptions() const = 0;
    virtual QUuid id() = 0;

private slots:
    void contextHandleChanged();

private:
    static ContextKitHandle *handleToCKHandle(Handle handle);
};

class ContextKitCoreLayer : public ContextKitLayer
{
    Q_OBJECT

public:
    QValueSpace::LayerOptions layerOptions() const;
    QUuid id();
    QString name();
    unsigned int order();

    static ContextKitCoreLayer *instance();
};

class ContextKitNonCoreLayer : public ContextKitLayer
{
    Q_OBJECT

public:
    QValueSpace::LayerOptions layerOptions() const;
    QUuid id();
    QString name();
    unsigned int order();

    static ContextKitNonCoreLayer *instance();
};

QTM_END_NAMESPACE

#endif // CONTEXTKITLAYER_P_H
