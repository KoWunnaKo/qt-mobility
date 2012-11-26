/****************************************************************************
**
** Copyright (C) 2012 Research In Motion
**
** Contact: Research In Motion <blackberry-qt@qnx.com>
** Contact: Klarälvdalens Datakonsult AB <info@kdab.com>
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
#include "bbmetadata.h"

#include <QtCore/qdebug.h>
#include <QtCore/qfile.h>
#include <QtCore/qstringlist.h>

QT_BEGIN_NAMESPACE

BbMetaData::BbMetaData()
{
    clear();
}

static const char * durationKey = "md_title_duration";
static const char * widthKey = "md_video_width";
static const char * heightKey = "md_video_height";
static const char * mediaTypeKey = "md_title_mediatype";
static const char * pixelWidthKey = "md_video_pixel_height";
static const char * pixelHeightKey = "md_video_pixel_width";

static const int mediaTypeAudioFlag = 4;
static const int mediaTypeVideoFlag = 2;

bool BbMetaData::parse(const QString &contextName)
{
    clear();
    QString fileName =
            QString("/pps/services/multimedia/renderer/context/%1/metadata").arg(contextName);

    // In newer OS versions, the filename is "metadata0", not metadata, so try both.
    if (!QFile::exists(fileName))
        fileName += '0';

    QFile metaDataFile(fileName);
    if (!metaDataFile.open(QFile::ReadOnly)) {
        qWarning() << "Unable to open media metadata file" << fileName << ":"
                   << metaDataFile.errorString();
        return false;
    }

    const QString seperator("::");
    QTextStream stream(&metaDataFile);
    Q_FOREVER {
        const QString line = stream.readLine();
        if (line.isNull())
            break;

        const int seperatorPos = line.indexOf(seperator);
        if (seperatorPos != -1) {
            const QString key = line.left(seperatorPos);
            const QString value = line.mid(seperatorPos + seperator.length());

            if (key == durationKey)
                m_duration = value.toLongLong();
            else if (key == widthKey)
                m_width = value.toInt();
            else if (key == heightKey)
                m_height = value.toInt();
            else if (key == mediaTypeKey)
                m_mediaType = value.toInt();
            else if (key == pixelWidthKey)
                m_pixelWidth = value.toFloat();
            else if (key == pixelHeightKey)
                m_pixelHeight = value.toFloat();
        }
    }

    return true;
}

void BbMetaData::clear()
{
    m_duration = 0;
    m_height = 0;
    m_width = 0;
    m_mediaType = -1;
    m_pixelWidth = 1;
    m_pixelHeight = 1;
}

qlonglong BbMetaData::duration() const
{
    return m_duration;
}

// Handling of pixel aspect ratio
//
// If the pixel aspect ratio is different from 1:1, it means the video needs to be stretched in
// order to look natural.
// For example, if the pixel width is 2, and the pixel height is 1, it means a video of 300x200
// pixels needs to be displayed as 600x200 to look correct.
// In order to support this the easiest way, we simply pretend that the actual size of the video
// is 600x200, which will cause the video to be displayed in an aspect ratio of 3:1 instead of 3:2,
// and therefore look correct.

int BbMetaData::height() const
{
    return m_height * m_pixelHeight;
}

int BbMetaData::width() const
{
    return m_width * m_pixelWidth;
}

bool BbMetaData::hasVideo() const
{
    // By default, assume no video if we can't extract the information
    if (m_mediaType == -1)
        return false;

    return (m_mediaType & mediaTypeVideoFlag);
}

bool BbMetaData::hasAudio() const
{
    // By default, assume audio only if we can't extract the information
    if (m_mediaType == -1)
        return true;

    return (m_mediaType & mediaTypeAudioFlag);
}

QT_END_NAMESPACE
