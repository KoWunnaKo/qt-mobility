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

#include "qlandmarkmanagerenginefactory.h"
#include <QList>

QTM_BEGIN_NAMESPACE

/*!
    \class QLandmarkManagerEngineFactory
    \brief The QLandmarkManagerEngineFactory class provides the interface for
    plugins that implement QLandmarkManagerEngine functionality.

    \inmodule QtLocation

    \ingroup landmarks-backend

    This class provides a simple interface for the creation of
    landmark manager engine instances.  Each factory has a specific id
    associated with it, which forms the \c managerName parameter
    when creating \l QLandmarkManager objects.

    \since 1.1
    \sa QLandmarkManager, QLandmarkManagerEngine
 */

/*!
  Destroys the engine factory.
 */
QLandmarkManagerEngineFactory::~QLandmarkManagerEngineFactory()
{
}

/*!
    This function returns a list of versions of the engine which this factory can instantiate.
 */
QList<int> QLandmarkManagerEngineFactory::supportedImplementationVersions() const
{
    return QList<int>();
}

/*!
    \fn QLandmarkManagerEngineFactory::managerName() const

    This function returns a unique string that identifies
    the engine provided by this factory.

    Typically this would be of the form "com.nokia.qt.landmarks.engines.sqlite", with
    the appropriate domain and engine name substituted.
    \since 1.1
*/

/*!
    \fn QLandmarkManagerEngineFactory::engine(const QMap<QString, QString>& parameters, QLandmarkManager::Error *error,
                                                QString *errorString)

    This function is called by the QLandmarkManager implementation to
    create an instance of the engine provided by this factory.

    The \a parameters supplied can be ignored or interpreted as desired.

    If a supplied parameter results in an unfulfillable request, or some other error
    occurs, this function may return a null pointer, and the client developer will get an
    invalid QLandmarkManager in return.  Errors are stored in \a error and \a errorString.
    \since 1.1
 */

QTM_END_NAMESPACE


