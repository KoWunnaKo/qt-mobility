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

#include "qlandmarkidfetchrequest.h"
#include "qlandmarkfilter.h"
#include "qlandmarksortorder.h"
#include "qlandmarkrequests_p.h"


QTM_BEGIN_NAMESPACE

/*!
    \class QLandmarkIdFetchRequest
    \brief The QLandmarkIdFetchRequest class allows a client to asynchronously
    request a list of landmark identifiers from a landmark manager.

    For a QLandmarkIdFetchRequest, the resultsAvailable() signal will be emitted when the resultant
    identifiers (which may be retrieved by calling landmarkIds()), are updated, as well as if
    the overall operation error (which may be retrieved by calling error()) is updated.

    Please see the class documentation for QLandmarkAbstractRequest for more information about
    the usage of request classes and ownership semantics.

    \inmodule QtLocation
    \since 1.1

    \ingroup landmarks-request
*/

/*!
    Creates a new landmark id fetch request object with the given \a manager and \a parent.
*/
QLandmarkIdFetchRequest::QLandmarkIdFetchRequest(QLandmarkManager *manager, QObject *parent)
    : QLandmarkAbstractRequest(new QLandmarkIdFetchRequestPrivate(manager), parent)
{
}

/*!
    Destroys the request object.
*/
QLandmarkIdFetchRequest::~QLandmarkIdFetchRequest()
{
}

/*!
    Returns the filter which will be used to select the landmarks whose identifiers
    will be returned.

    By default, the filter's type will be a QLandmarkFilter::DefaultFilter
    and thus match all landmarks.
    \since 1.1
*/
QLandmarkFilter QLandmarkIdFetchRequest::filter() const
{
    Q_D(const QLandmarkIdFetchRequest);
    QMutexLocker ml(&d->mutex);
    return d->filter;
}

/*!
    Sets the \a filter which will be used to select landmark identifiers.

    \since 1.1
*/
void QLandmarkIdFetchRequest::setFilter(const QLandmarkFilter &filter)
{
    Q_D(QLandmarkIdFetchRequest);
    QMutexLocker ml(&d->mutex);
    d->filter = filter;
}

/*!
    Returns the sort ordering which is used to sort the result.  By default the
    sort order list is empty, thus no sorting will take place.
    \since 1.1
*/
QList<QLandmarkSortOrder> QLandmarkIdFetchRequest::sorting() const
{
    Q_D(const QLandmarkIdFetchRequest);
    QMutexLocker ml(&d->mutex);
    return d->sorting;
}

/*!
    Sets the sort ordering of the request to \a sorting.  This
    function will only have an effect on the results if invoked
    prior to calling QLandmarkAbstractRequest::start().
    \since 1.1
*/
void QLandmarkIdFetchRequest::setSorting(const QList<QLandmarkSortOrder> &sorting)
{
    Q_D(QLandmarkIdFetchRequest);
    QMutexLocker ml(&d->mutex);
    d->sorting = sorting;
}

/*!
    Convenience function to set the sort ordering of the request to a single \a sorting.  This
    function will only have an effect on the results if invoked
    prior to calling QLandmarkAbstractRequest::start().
    \since 1.1
*/
void QLandmarkIdFetchRequest::setSorting(const QLandmarkSortOrder &sorting)
{
    Q_D(QLandmarkIdFetchRequest);
    QMutexLocker ml(&d->mutex);
    d->sorting.clear();
    d->sorting.append(sorting);
}

/*!
    Returns the maximum number of landmark IDs to be returned.  By default
    the limit is set to -1 indicating that IDs of all landmarks are to be retrieved.
    \since 1.1
*/
int QLandmarkIdFetchRequest::limit() const
{
    Q_D(const QLandmarkIdFetchRequest);
    QMutexLocker ml(&d->mutex);
    return d->limit;
}

/*!
    Sets the maximum number of landmarks to be returned to \a limit.

    A limit of -1 indicates that IDs of all landmarks are to be retrieved.

    (A limit of 0 will retrieve no landmark IDs.)
    \since 1.1
 */
void QLandmarkIdFetchRequest::setLimit(int limit)
{
    Q_D(QLandmarkIdFetchRequest);
    QMutexLocker ml(&d->mutex);
    d->limit = limit;
}

/*!
    Returns the index offset for the request.  By default the offset is set to 0.
    The offset determines the first index which is retrieved, it is generally used
    in conjunction with limit() to facilitate paging.

    For example, if there are 10 landmarks in the landmark store, setting the offset
    to 2 and limit to 5 will retrieve the 3rd and 7th landmark ids inclusively.  (The order
    of the landmark ids is sepecified by the sorting field).
    \since 1.1
*/
int QLandmarkIdFetchRequest::offset() const
{
    Q_D(const QLandmarkIdFetchRequest);
    QMutexLocker ml(&d->mutex);
    return d->offset;
}

/*!
    Sets the index \a offset for the request.
    \since 1.1
*/
void QLandmarkIdFetchRequest::setOffset(int offset) {
    Q_D(QLandmarkIdFetchRequest);
    QMutexLocker ml(&d->mutex);
    d->offset = offset;
}

/*!
    Returns the list of identifiers of landmarks which matched the
    filter.
    \since 1.1
*/
QList<QLandmarkId> QLandmarkIdFetchRequest::landmarkIds() const
{
    Q_D(const QLandmarkIdFetchRequest);
    QMutexLocker ml(&d->mutex);
    return d->landmarkIds;
}

#include "moc_qlandmarkidfetchrequest.cpp"

QTM_END_NAMESPACE











