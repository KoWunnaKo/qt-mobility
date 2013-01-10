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

#import <Foundation/Foundation.h>
#import <QTKit/QTKit.h>

#include <QtCore/qstring.h>
#include <QtCore/qdebug.h>

#include "qt7backend.h"
#include "qt7serviceplugin.h"
#include "qt7playerservice.h"

#include <qmediaserviceprovider.h>

QT_BEGIN_NAMESPACE


QT7ServicePlugin::QT7ServicePlugin()
{
    buildSupportedTypes();
}

QStringList QT7ServicePlugin::keys() const
{
    return QStringList()
#ifdef QMEDIA_QT7_PLAYER
        << QLatin1String(Q_MEDIASERVICE_MEDIAPLAYER)
#endif
        ;
}

QMediaService* QT7ServicePlugin::create(QString const& key)
{
#ifdef QT_DEBUG_QT7
    qDebug() << "QT7ServicePlugin::create" << key;
#endif
#ifdef QMEDIA_QT7_PLAYER
    if (key == QLatin1String(Q_MEDIASERVICE_MEDIAPLAYER))
        return new QT7PlayerService;
#endif
    qWarning() << "unsupported key:" << key;

    return 0;
}

void QT7ServicePlugin::release(QMediaService *service)
{
    delete service;
}

QMediaServiceProviderHint::Features QT7ServicePlugin::supportedFeatures(
        const QByteArray &service) const
{
    if (service == Q_MEDIASERVICE_MEDIAPLAYER)
        return QMediaServiceProviderHint::VideoSurface;
    else
        return QMediaServiceProviderHint::Features();
}

QtMultimediaKit::SupportEstimate QT7ServicePlugin::hasSupport(const QString &mimeType, const QStringList& codecs) const
{
    Q_UNUSED(codecs);

    if (m_supportedMimeTypes.contains(mimeType))
        return QtMultimediaKit::ProbablySupported;

    return QtMultimediaKit::MaybeSupported;
}

QStringList QT7ServicePlugin::supportedMimeTypes() const
{
    return m_supportedMimeTypes;
}

void QT7ServicePlugin::buildSupportedTypes()
{
    AutoReleasePool pool;
    NSArray *utis = [QTMovie movieTypesWithOptions:QTIncludeCommonTypes];
    for (NSString *uti in utis) {
        NSString* mimeType = (NSString*)UTTypeCopyPreferredTagWithClass((CFStringRef)uti, kUTTagClassMIMEType);
        if (mimeType != 0) {
            m_supportedMimeTypes.append(QString::fromUtf8([mimeType UTF8String]));
            [mimeType release];
        }
    }
}

Q_EXPORT_PLUGIN2(qtmedia_qt7engine, QT7ServicePlugin);

QT_END_NAMESPACE
