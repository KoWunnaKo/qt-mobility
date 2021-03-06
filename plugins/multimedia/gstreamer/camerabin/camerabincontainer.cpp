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

#include "camerabincontainer.h"


#include <QtCore/qdebug.h>

CameraBinContainer::CameraBinContainer(QObject *parent)
    :QMediaContainerControl(parent)
{
    QList<QByteArray> formatCandidates;
    formatCandidates << "mp4" << "ogg" << "wav" << "amr" << "mkv"
                     << "avi" << "3gp" << "3gp2" << "webm" << "mjpeg" << "asf" << "mov";

    QMap<QString,QByteArray> elementNames;

    elementNames.insertMulti("mp4", "ffmux_mp4");
    elementNames.insertMulti("mp4", "hantromp4mux");
    elementNames.insertMulti("mp4", "mp4mux");
    elementNames.insert("ogg", "oggmux");
    elementNames["wav"] = "wavenc";
    elementNames["amr"] = "ffmux_amr";
    elementNames["mkv"] = "matroskamux";
    elementNames["avi"] = "avimux";
    elementNames["3gp"] = "ffmux_3gp";
    elementNames["3gp2"] = "ffmux_3g2";
    elementNames["webm"] = "webmmux";
    elementNames["mjpeg"] = "ffmux_mjpeg";
    elementNames["asf"] = "ffmux_asf";
    elementNames["mov"] = "qtmux";

    QSet<QString> allTypes;

    foreach(const QByteArray &formatName, formatCandidates) {
        foreach(const QByteArray &elementName, elementNames.values(formatName)) {
            GstElementFactory *factory = gst_element_factory_find(elementName.constData());
            if (factory) {
                m_supportedContainers.append(formatName);
                const gchar *descr = gst_element_factory_get_description(factory);
                m_containerDescriptions.insert(formatName, QString::fromUtf8(descr));


                if (formatName == QByteArray("raw")) {
                    m_streamTypes.insert(formatName, allTypes);
                } else {
                    QSet<QString> types = supportedStreamTypes(factory, GST_PAD_SINK);
                    m_streamTypes.insert(formatName, types);
                    allTypes.unite(types);
                }

                gst_object_unref(GST_OBJECT(factory));

                m_elementNames.insert(formatName, elementName);
                break;
            }
        }
    }
}

QSet<QString> CameraBinContainer::supportedStreamTypes(GstElementFactory *factory, GstPadDirection direction)
{
    QSet<QString> types;
    const GList *pads = gst_element_factory_get_static_pad_templates(factory);
    for (const GList *pad = pads; pad; pad = g_list_next(pad)) {
        GstStaticPadTemplate *templ = (GstStaticPadTemplate*)pad->data;
        if (templ->direction == direction) {
            GstCaps *caps = gst_static_caps_get(&templ->static_caps);
            for (uint i=0; i<gst_caps_get_size(caps); i++) {
                GstStructure *structure = gst_caps_get_structure(caps, i);
                types.insert( QString::fromUtf8(gst_structure_get_name(structure)) );
            }
            gst_caps_unref(caps);
        }
    }

    return types;
}


QSet<QString> CameraBinContainer::supportedStreamTypes(const QString &container) const
{
    return m_streamTypes.value(container);
}
