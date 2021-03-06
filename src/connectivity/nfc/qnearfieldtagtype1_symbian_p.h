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

#ifndef QNEARFIELDTAGTYPE1SYMBIAN_H
#define QNEARFIELDTAGTYPE1SYMBIAN_H

#include <qnearfieldtagtype1.h>
#include "symbian/nearfieldndeftarget_symbian.h"
#include "symbian/nearfieldtag_symbian.h"
#include "symbian/nearfieldtagimpl_symbian.h"

QT_BEGIN_HEADER

QTM_BEGIN_NAMESPACE

class QNearFieldTagType1Symbian : public QNearFieldTagType1, private QNearFieldTagImpl<QNearFieldTagType1Symbian>
{
    Q_OBJECT

public:

    explicit QNearFieldTagType1Symbian(CNearFieldNdefTarget *tag, QObject *parent = 0);

    ~QNearFieldTagType1Symbian();

    virtual QByteArray uid() const;

    // DIGPROTO
    RequestId readIdentification();

    // static memory functions
    RequestId readAll();
    RequestId readByte(quint8 address);
    RequestId writeByte(quint8 address, quint8 data, WriteMode mode = EraseAndWrite);

    // dynamic memory functions
    RequestId readSegment(quint8 segmentAddress);
    RequestId readBlock(quint8 blockAddress);
    RequestId writeBlock(quint8 blockAddress, const QByteArray &data,
                         WriteMode mode = EraseAndWrite);

    bool hasNdefMessage();
    RequestId readNdefMessages();
    RequestId writeNdefMessages(const QList<QNdefMessage> &messages);

    bool isProcessingCommand() const { return _isProcessingRequest(); }
    RequestId sendCommand(const QByteArray &command);
    RequestId sendCommands(const QList<QByteArray> &commands);
    bool waitForRequestCompleted(const RequestId &id, int msecs = 5000);

    void setAccessMethods(const QNearFieldTarget::AccessMethods& accessMethods)
    {
        _setAccessMethods(accessMethods);
    }

    QNearFieldTarget::AccessMethods accessMethods() const
    {
        return _accessMethods();
    }

    void handleTagOperationResponse(const RequestId &id, const QByteArray &command, const QByteArray &response, bool emitRequestCompleted);
    QVariant decodeResponse(const QByteArray &command, const QByteArray &response);
    friend class QNearFieldTagImpl<QNearFieldTagType1Symbian>;
};

QTM_END_NAMESPACE

QT_END_HEADER

#endif // QNEARFIELDTAGTYPE1SYMBIAN_H
