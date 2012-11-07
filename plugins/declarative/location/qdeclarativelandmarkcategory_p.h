/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
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
***************************************************************************/

#ifndef QDECLARATIVELANDMARKCATEGORY_P_H
#define QDECLARATIVELANDMARKCATEGORY_P_H

#include <qlandmark.h>
#include <qlandmarkcategory.h>
#include <QtDeclarative/qdeclarative.h>
#include <QAbstractListModel>
#include <QtCore>

QTM_BEGIN_NAMESPACE

class QDeclarativeLandmarkCategory : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QUrl iconSource READ iconSource WRITE setIconSource NOTIFY iconSourceChanged)

public:
    explicit QDeclarativeLandmarkCategory(QObject* parent = 0);

    QString name();
    void setName(const QString& name);

    QUrl iconSource();
    void setIconSource(const QUrl& iconSource);

    QLandmarkCategory category() const;

signals:
    void nameChanged();
    void iconSourceChanged();

private:

    QLandmarkCategory m_category;
    friend class QDeclarativeLandmarkCategoryModel;
    void setCategory(const QLandmarkCategory& category);
};

QTM_END_NAMESPACE
QML_DECLARE_TYPE(QTM_PREPEND_NAMESPACE(QDeclarativeLandmarkCategory));

#endif // QDECLARATIVELANDMARKCATEGORY_P_H
