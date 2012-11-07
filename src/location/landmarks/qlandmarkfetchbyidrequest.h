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

#ifndef QLANDMARKFETCHBYIDREQUEST_H
#define QLANDMARKFETCHBYIDREQUEST_H

#include "qlandmarkabstractrequest.h"

QTM_BEGIN_NAMESPACE

class QLandmarkFetchByIdRequestPrivate;
class Q_LOCATION_EXPORT QLandmarkFetchByIdRequest : public QLandmarkAbstractRequest
{
    Q_OBJECT
public:
    QLandmarkFetchByIdRequest(QLandmarkManager *manager, QObject *parent = 0);
    ~QLandmarkFetchByIdRequest();

    QList<QLandmarkId> landmarkIds() const;
    void setLandmarkIds(const QList<QLandmarkId> &ids);
    void setLandmarkId(const QLandmarkId &id);

    QList<QLandmark> landmarks() const;
    QMap<int, QLandmarkManager::Error> errorMap() const;
private:
    Q_DISABLE_COPY(QLandmarkFetchByIdRequest)
    Q_DECLARE_PRIVATE(QLandmarkFetchByIdRequest)
    friend class QLandmarkManagerEngine;
};

QTM_END_NAMESPACE

#endif
