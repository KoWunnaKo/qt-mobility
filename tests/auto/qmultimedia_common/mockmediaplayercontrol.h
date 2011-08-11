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

#ifndef MOCKMEDIAPLAYERCONTROL_H
#define MOCKMEDIAPLAYERCONTROL_H

#include "qmediaplayercontrol.h"

class MockMediaPlayerControl : public QMediaPlayerControl
{
    friend class MockMediaPlayerService;

public:
    MockMediaPlayerControl():QMediaPlayerControl(0) {}

    QMediaPlayer::State state() const { return _state; }
    QMediaPlayer::MediaStatus mediaStatus() const { return _mediaStatus; }

    qint64 duration() const { return _duration; }

    qint64 position() const { return _position; }

    void setPosition(qint64 position) { if (position != _position) emit positionChanged(_position = position); }

    int volume() const { return _volume; }
    void setVolume(int volume) { emit volumeChanged(_volume = volume); }

    bool isMuted() const { return _muted; }
    void setMuted(bool muted) { if (muted != _muted) emit mutedChanged(_muted = muted); }

    int bufferStatus() const { return _bufferStatus; }

    bool isAudioAvailable() const { return _audioAvailable; }
    bool isVideoAvailable() const { return _videoAvailable; }

    bool isSeekable() const { return _isSeekable; }
    QMediaTimeRange availablePlaybackRanges() const { return QMediaTimeRange(_seekRange.first, _seekRange.second); }
    void setSeekRange(qint64 minimum, qint64 maximum) { _seekRange = qMakePair(minimum, maximum); }

    qreal playbackRate() const { return _playbackRate; }
    void setPlaybackRate(qreal rate) { if (rate != _playbackRate) emit playbackRateChanged(_playbackRate = rate); }

    QMediaContent media() const { return _media; }
    void setMedia(const QMediaContent &content, QIODevice *stream)
    {
        _stream = stream;
        _media = content;
        if (_state != QMediaPlayer::StoppedState) {
            _mediaStatus = _media.isNull() ? QMediaPlayer::NoMedia : QMediaPlayer::LoadingMedia;
            emit stateChanged(_state = QMediaPlayer::StoppedState);
            emit mediaStatusChanged(_mediaStatus);
        }
        emit mediaChanged(_media = content);
    }
    QIODevice *mediaStream() const { return _stream; }

    void play() { if (_isValid && !_media.isNull() && _state != QMediaPlayer::PlayingState) emit stateChanged(_state = QMediaPlayer::PlayingState); }
    void pause() { if (_isValid && !_media.isNull() && _state != QMediaPlayer::PausedState) emit stateChanged(_state = QMediaPlayer::PausedState); }
    void stop() { if (_state != QMediaPlayer::StoppedState) emit stateChanged(_state = QMediaPlayer::StoppedState); }

    QMediaPlayer::State _state;
    QMediaPlayer::MediaStatus _mediaStatus;
    QMediaPlayer::Error _error;
    qint64 _duration;
    qint64 _position;
    int _volume;
    bool _muted;
    int _bufferStatus;
    bool _audioAvailable;
    bool _videoAvailable;
    bool _isSeekable;
    QPair<qint64, qint64> _seekRange;
    qreal _playbackRate;
    QMediaContent _media;
    QIODevice *_stream;
    bool _isValid;
    QString _errorString;
};

#endif // MOCKMEDIAPLAYERCONTROL_H
