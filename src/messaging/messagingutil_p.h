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

#ifndef _SYMBIANHELPERS_H_
#define _SYMBIANHELPERS_H_

#include <QtCore>
#include "qmessage.h"

QTM_BEGIN_NAMESPACE

class QMessageId;
class QMessageAccountId;
class QMessageFolderId;

namespace SymbianHelpers {

    enum EngineType {
        EngineTypeMTM = 0,
        EngineTypeFreestyle,
        EngineDefault
    };

    EngineType idType(const QString& id);
    EngineType idType(const QMessageId& id);
    EngineType idType(const QMessageAccountId& id);
    EngineType idType(const QMessageFolderId& id);

    QString addIdPrefix(const QString& id, const EngineType& type = EngineDefault);
    QMessageId addIdPrefix(const QMessageId& id, const EngineType& type = EngineDefault);
    QMessageAccountId addIdPrefix(const QMessageAccountId& id, const EngineType& type = EngineDefault);
    QMessageFolderId addIdPrefix(const QMessageFolderId& id, const EngineType& type = EngineDefault);

    QString stripIdPrefix(const QString& id);
    QMessageId stripIdPrefix(const QMessageId& id);
    QMessageAccountId stripIdPrefix(const QMessageAccountId& id);
    QMessageFolderId stripIdPrefix(const QMessageFolderId& id);

    static const char* mtmPrefix = "MTM_";
    static const char* freestylePrefix = "FS_";
}

namespace MessagingUtil
{
    QString addIdPrefix(const QString& id);
    QString stripIdPrefix(const QString& id);
    QString idPrefix();
    bool globMatch(const QString &pattern, const QString &value);
}

QTM_END_NAMESPACE

#endif
