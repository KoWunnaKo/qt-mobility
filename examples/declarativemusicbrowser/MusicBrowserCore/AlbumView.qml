/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** $QT_END_LICENSE$
**
****************************************************************************/

import Qt 4.7
import QtMobility.gallery 1.1

Item {
    property string subTitle: ""
    property bool backEnabled: false

    function back() { albumView.state = 'grid' }

    id: albumView

    clip: true

    VisualDataModel {
        id: visualModel

        model: GalleryQueryModel {
            id: galleryModel
            gallery: documentGallery
            rootType: "Album"
            properties: [ "artist", "title" ]
            sortProperties: [ "title" ]
        }
        delegate: AlbumDelegate {
            state: albumView.state
            viewWidth: listView.width
            viewHeight: listView.height
            onClicked: {
                listView.positionViewAtIndex(index, ListView.Contain)
                albumView.state = 'fullScreen'
            }

        }
    }

    GridView {
        id: gridView
        anchors.fill: parent
        cellWidth: 256
        cellHeight: 192

        model: visualModel.parts.grid
    }

    ListView {
        id: listView
        anchors.fill: parent
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange

        model: visualModel.parts.fullScreen

        onCurrentIndexChanged: {
            gridView.positionViewAtIndex(currentIndex, GridView.Contain)
            subTitle = currentItem.albumTitle
        }

    }

    state: 'grid'

    states: [
        State {
            name: 'grid'
            PropertyChanges { target: albumView; backEnabled: false }
            PropertyChanges { target: listView; interactive: false; }
        },
        State {
            name: 'fullScreen'
            PropertyChanges { target: albumView; backEnabled: true }
            PropertyChanges { target: gridView; interactive: false }
            PropertyChanges { target: listView; interactive: true }
        }
    ]
}