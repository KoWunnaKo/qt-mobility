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

#include "qvcard30writer_p.h"
#include "versitutils_p.h"
#include "qmobilityglobal.h"
#include <QTextCodec>
#include <QBuffer>

QTM_USE_NAMESPACE

/*! Constructs a writer. */
QVCard30Writer::QVCard30Writer(QVersitDocument::VersitType type) : QVersitDocumentWriter(type)
{
    mPropertyNameMappings.insert(
        QLatin1String("X-NICKNAME"),QLatin1String("NICKNAME"));
    mPropertyNameMappings.insert(
        QLatin1String("X-IMPP"),QLatin1String("IMPP"));
}

/*! Destroys a writer. */
QVCard30Writer::~QVCard30Writer()
{
}

/*!
 * Encodes the \a property and writes it to the device.
 */
void QVCard30Writer::encodeVersitProperty(const QVersitProperty& property)
{
    QVersitProperty modifiedProperty(property);
    QString name = mPropertyNameMappings.value(property.name(),property.name());
    modifiedProperty.setName(name);
    encodeGroupsAndName(modifiedProperty);

    QVariant variant(modifiedProperty.variantValue());
    if (variant.type() == QVariant::ByteArray) {
        modifiedProperty.insertParameter(QLatin1String("ENCODING"), QLatin1String("b"));
    }
    encodeParameters(modifiedProperty.parameters());
    writeString(QLatin1String(":"));

    QString renderedValue;
    QByteArray renderedBytes;
    if (variant.canConvert<QVersitDocument>()) {
        QVersitDocument embeddedDocument = variant.value<QVersitDocument>();
        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        QVCard30Writer subWriter(mType);
        subWriter.setCodec(mCodec);
        subWriter.setDevice(&buffer);
        subWriter.encodeVersitDocument(embeddedDocument);
        QString documentString(mCodec->toUnicode(data));
        backSlashEscape(&documentString);
        renderedValue = documentString;
    } else if (variant.type() == QVariant::String) {
        renderedValue = variant.toString();
        if (property.valueType() != QVersitProperty::PreformattedType) {
            backSlashEscape(&renderedValue);
        }
    } else if (variant.type() == QVariant::StringList) {
        // We need to backslash escape and concatenate the values in the list
        QStringList values = property.variantValue().toStringList();
        QString separator;
        if (property.valueType() == QVersitProperty::CompoundType) {
            separator = QLatin1String(";");
        } else {
            if (property.valueType() != QVersitProperty::ListType) {
                qWarning("Variant value is a QStringList but the property's value type is neither "
                         "CompoundType or ListType");
            }
            // Assume it's a ListType
            separator = QLatin1String(",");
        }
        bool first = true;
        foreach (QString value, values) {
            if (!(value.isEmpty() && property.valueType() == QVersitProperty::ListType)) {
                if (!first) {
                    renderedValue += separator;
                }
                backSlashEscape(&value);
                renderedValue += value;
                first = false;
            }
        }
    } else if (variant.type() == QVariant::ByteArray) {
        if (mCodecIsAsciiCompatible) // optimize by not converting to unicode
            renderedBytes = variant.toByteArray().toBase64();
        else
            renderedValue = QLatin1String(variant.toByteArray().toBase64().data());
    }

    if (renderedBytes.isEmpty())
        writeString(renderedValue);
    else
        writeBytes(renderedBytes);
    writeCrlf();
}

/*!
 * Encodes the \a parameters and writes it to the device.
 */
void QVCard30Writer::encodeParameters(const QMultiHash<QString,QString>& parameters)
{
    QList<QString> names = parameters.uniqueKeys();
    foreach (QString nameString, names) {
        writeString(QLatin1String(";"));
        QStringList values = parameters.values(nameString);
        backSlashEscape(&nameString);
        writeString(nameString);
        writeString(QLatin1String("="));
        for (int i=0; i<values.size(); i++) {
            if (i > 0)
                writeString(QLatin1String(","));
            QString value = values.at(i);

            backSlashEscape(&value);
            writeString(value);
        }
    }
}


/*!
 * Performs backslash escaping for line breaks (CRLFs), semicolons, backslashes and commas according
 * to RFC 2426.  This is called on parameter names and values and property values.
 * Colons ARE NOT escaped because the examples in RFC2426 suggest that they shouldn't be.
 */
void QVCard30Writer::backSlashEscape(QString* text)
{
    static const QString m1(QLatin1String("([;,\\\\])"));
    static const QString r1(QLatin1String("\\\\1"));
    static const QString m2(QLatin1String("\r\n|\r|\n"));
    static const QString r2(QLatin1String("\\n"));
    /* replaces ; with \;
                , with \,
                \ with \\
     */
    text->replace(QRegExp(m1), r1);
    // replaces any CRLFs with \n
    text->replace(QRegExp(m2), r2);
}
