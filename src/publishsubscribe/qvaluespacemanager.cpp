/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qvaluespacemanager_p.h"

QTM_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QValueSpaceManager, valueSpaceManager);

QValueSpaceManager *QValueSpaceManager::instance()
{
    return valueSpaceManager();
}

QValueSpaceManager::QValueSpaceManager()
    : type(Uninit)
{
}

void QValueSpaceManager::init(QAbstractValueSpaceLayer::Type type)
{
    // Both server and clients may call this function multiple times
    if (Uninit != this->type)
        return;

    // Install all the dormant layers
    for (int ii = 0; ii < funcs.count(); ++ii)
        install(funcs[ii]());
    funcs.clear();

    this->type = (type == QAbstractValueSpaceLayer::Server) ? Server : Client;

    for (int ii = 0; ii < layers.count(); ++ii) {
        if (!layers.at(ii)->startup(type)) {
            layers.removeAt(ii);
            --ii;
        }
    }
}

bool QValueSpaceManager::isServer() const
{
    return (Server == type);
}

void QValueSpaceManager::install(QAbstractValueSpaceLayer * layer)
{
    Q_ASSERT(Uninit == type);
    Q_ASSERT(layer);

    layers.append(layer);
}

void QValueSpaceManager::install(QValueSpace::LayerCreateFunc func)
{
    Q_ASSERT(Uninit == type);
    funcs.append(func);
}

QList<QAbstractValueSpaceLayer *> const & QValueSpaceManager::getLayers()
{
    init(QAbstractValueSpaceLayer::Client); // Fallback init

    return layers;
}

QTM_END_NAMESPACE
