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

#include "qlandmarkid.h"
#include "qlandmarkid_p.h"
#include <QString>

QTM_USE_NAMESPACE

/*!
    \class QLandmarkId

    \inmodule QtLocation

    \ingroup landmarks-main
    \since 1.1

    \brief The QLandmarkId class provides a unique identifier for
    a landmark.

    It consists of a manager URI which identifies the manager which contains
    the landmark, and a manager specific local id for that landmark.
*/

/*!

    Creates an invalid identifier.

     The local id and manager uri are empty strings.
*/
QLandmarkId::QLandmarkId()
        : d(new QLandmarkIdPrivate)
{
}

/*!
    Constructs a copy of \a other.
    \since 1.1
*/
QLandmarkId::QLandmarkId(const QLandmarkId &other)
        : d(other.d)
{
}

/*!
    Destroys the identifier.
*/
QLandmarkId::~QLandmarkId()
{
}

/*!
    Returns true if the identifier is valid, meaning that both
    a manager URI and local id have been set.
    \since 1.1
*/
bool QLandmarkId::isValid() const
{
    return (!d->m_uri.isEmpty() && !d->m_localId.isEmpty());
}

/*!
    Returns the local id of the landmark identifier.

    This id is specific to a particular landmark manager.
    \since 1.1
*/
QString QLandmarkId::localId() const
{
    return d->m_localId;
}

/*!
    Sets the local \a id of the landmark identifier.
    \since 1.1
*/
void QLandmarkId::setLocalId(const QString &id)
{
    d->m_localId = id;
}

/*!
    Returns the URI of the manager which this landmark identifer
    belongs to.
    \since 1.1
*/
QString QLandmarkId::managerUri() const
{
    return d->m_uri;
}

/*!
    Sets the manager \a uri which this landmark identifier
    belongs to.
    \since 1.1
*/
void QLandmarkId::setManagerUri(const QString &uri)
{
    d->m_uri = uri;
}

/*!
    Assigns \a other to this landmark identifier and returns a reference
    to this landmark identifier.
    \since 1.1
*/
QLandmarkId &QLandmarkId::operator=(const QLandmarkId & other)
{
    if (this != &other)
        d = other.d;
    return *this;
}

/*!
    Returns true if this landmark identifer is equal to \a other, otherwise
    returns false.

    Two QLandmarkIds are considered equal if both have the same
    manager URI and local id.

    \since 1.1
    \sa operator!=()
*/
bool QLandmarkId::operator==(const QLandmarkId &other) const
{
    return ((d->m_localId == other.d->m_localId)
            && (d->m_uri == other.d->m_uri));
}

/*!
    Returns true if this landmark identifier is not equal to \a other,
    otherwise returns false.

    If either the manager URIs or local ids differ, then the
    QLandmarkIds are not considered equal.

    \since 1.1
    \sa operator==()
*/
bool QLandmarkId::operator!=(const QLandmarkId &other) const
{
    return !this->operator ==(other);
}


