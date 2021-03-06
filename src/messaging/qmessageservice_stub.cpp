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
#include "qmessageservice.h"


QTM_BEGIN_NAMESPACE

QMessageService::QMessageService(QObject *parent)
    : QObject(parent)
{
}

QMessageService::~QMessageService()
{
}

bool QMessageService::queryMessages(const QMessageFilter &filter, const QMessageSortOrder &sortOrder, uint limit, uint offset)
{
    Q_UNUSED(filter);
    Q_UNUSED(sortOrder);
    Q_UNUSED(limit);
    Q_UNUSED(offset);
    return false; // stub
}

bool QMessageService::queryMessages(const QMessageFilter &filter, const QString &body, QMessageDataComparator::MatchFlags matchFlags, const QMessageSortOrder &sortOrder, uint limit, uint offset)
{
    Q_UNUSED(filter);
    Q_UNUSED(body);
    Q_UNUSED(matchFlags);
    Q_UNUSED(sortOrder);
    Q_UNUSED(limit);
    Q_UNUSED(offset);
    return false; // stub
}

bool QMessageService::countMessages(const QMessageFilter &filter)
{
    // TODO: Implement this
    Q_UNUSED(filter);
    return false;
}

bool QMessageService::send(QMessage &message)
{
    Q_UNUSED(message)
    return false; // stub
}

bool QMessageService::compose(const QMessage &message)
{
    Q_UNUSED(message)
    return false; // stub
}

bool QMessageService::retrieveHeader(const QMessageId& id)
{
    Q_UNUSED(id)
    return false; // stub
}

bool QMessageService::retrieveBody(const QMessageId& id)
{
    Q_UNUSED(id)
    return false; // stub
}

bool QMessageService::retrieve(const QMessageId &messageId, const QMessageContentContainerId& id)
{
    Q_UNUSED(messageId)
    Q_UNUSED(id)
    return false; // stub
}

bool QMessageService::show(const QMessageId& id)
{
    Q_UNUSED(id)
    return false; // stub
}

bool QMessageService::exportUpdates(const QMessageAccountId &id)
{
    Q_UNUSED(id)
    return false; // stub
}

QMessageService::State QMessageService::state() const
{
    return InactiveState; // stub
}

void QMessageService::cancel()
{
}

QMessageManager::Error QMessageService::error() const
{
    return QMessageManager::NoError;
}

QTM_END_NAMESPACE
