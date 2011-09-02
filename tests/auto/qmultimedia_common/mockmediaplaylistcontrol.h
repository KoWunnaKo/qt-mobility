/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MOCKMEDIAPLAYLISTCONTROL_H
#define MOCKMEDIAPLAYLISTCONTROL_H

#include "qmediaplaylistcontrol.h"
#include "qmediaplaylistnavigator.h"

#include "mockreadonlyplaylistprovider.h"

// Hmm, read only.
class MockMediaPlaylistControl : public QMediaPlaylistControl
{
    Q_OBJECT
public:
    MockMediaPlaylistControl(QObject *parent) : QMediaPlaylistControl(parent)
    {
        m_navigator = new QMediaPlaylistNavigator(new MockReadOnlyPlaylistProvider(this), this);
    }

    ~MockMediaPlaylistControl()
    {
    }

    QMediaPlaylistProvider* playlistProvider() const { return m_navigator->playlist(); }
    bool setPlaylistProvider(QMediaPlaylistProvider *newProvider)
    {
        bool bMediaContentChanged = false;
        int i = 0;
        for (; i < playlistProvider()->mediaCount(); i++) {
            if (playlistProvider()->media(i).canonicalUrl().toString() != newProvider->media(i).canonicalUrl().toString()) {
                bMediaContentChanged = true;
                break;
            }
        }

        if (playlistProvider()->mediaCount() != newProvider->mediaCount() || bMediaContentChanged ) {
            emit playlistProviderChanged();
            emit currentMediaChanged(newProvider->media(i));
        }

        m_navigator->setPlaylist(newProvider);
        return true;
    }

    int currentIndex() const { return m_navigator->currentIndex(); }
    void setCurrentIndex(int position)
    {
        if (position != currentIndex())
            emit currentIndexChanged(position);
        m_navigator->jump(position);
    }

    int nextIndex(int steps) const { return m_navigator->nextIndex(steps); }
    int previousIndex(int steps) const { return m_navigator->previousIndex(steps); }

    void next() { m_navigator->next(); }
    void previous() { m_navigator->previous(); }

    QMediaPlaylist::PlaybackMode playbackMode() const { return m_navigator->playbackMode(); }
    void setPlaybackMode(QMediaPlaylist::PlaybackMode mode)
    {
        if (playbackMode() != mode)
            emit playbackModeChanged(mode);

        m_navigator->setPlaybackMode(mode);
    }

private:
    QMediaPlaylistNavigator *m_navigator;
};

#endif // MOCKMEDIAPLAYLISTCONTROL_H
