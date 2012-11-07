/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QtCore/qglobal.h>
#include <QtGui/qwidget.h>
#include <qmediaplayer.h>

#ifdef Q_OS_SYMBIAN
#include <remconcoreapitarget.h>
#include <remconcoreapitargetobserver.h>
#include <remconinterfaceselector.h>
#endif // Q_OS_SYMBIAN

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QAbstractSlider;
class QComboBox;
QT_END_NAMESPACE

QT_USE_NAMESPACE

class PlayerControls : public QWidget
#ifdef Q_OS_SYMBIAN
                     , public MRemConCoreApiTargetObserver
#endif // Q_OS_SYMBIAN
{
    Q_OBJECT
public:

    PlayerControls(QWidget *parent, QMediaPlayer *player);
    ~PlayerControls();

    QMediaPlayer::State state() const;

    int volume() const;
    bool isMuted() const;
    qreal playbackRate() const;

#ifdef Q_OS_SYMBIAN
protected:
    void MrccatoCommand(TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct);
private:
    void initRemCon();
#endif // Q_OS_SYMBIAN

public slots:
    void setState(QMediaPlayer::State state);
    void setVolume(int volume);
    void setMuted(bool muted);
    void setPlaybackRate(float rate);

signals:
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void changeVolume(int volume);
    void changeMuting(bool muting);
    void changeRate(qreal rate);

private slots:
    void playClicked();
    void muteClicked();
    void updateRate();

private:
    QMediaPlayer::State playerState;
    bool playerMuted;
    QAbstractButton *playButton;
    QAbstractButton *stopButton;
    QAbstractButton *nextButton;
    QAbstractButton *previousButton;
    QAbstractButton *muteButton;
    QAbstractSlider *volumeSlider;
    QComboBox *rateBox;
#ifdef Q_OS_SYMBIAN
    CRemConInterfaceSelector *interfaceSelector;
    CRemConCoreApiTarget *coreTarget;
#endif // Q_OS_SYMBIAN
    QMediaPlayer *iplayer;
};

#endif // PLAYERCONTROLS_H
