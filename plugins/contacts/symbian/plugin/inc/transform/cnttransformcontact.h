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

#ifndef TRANSFORMCONCTACT_H
#define TRANSFORMCONCTACT_H

#include <qtcontacts.h>

#include <cntfldst.h>
#include <cntdb.h>
#include <cntdef.h>
#include <cntitem.h>
#include <tz.h>
#include <tzconverter.h>

class CntTransformContactData;
class CTzConverter;

QTM_BEGIN_NAMESPACE
class QContactDetailDefinition;
QTM_END_NAMESPACE

QTM_USE_NAMESPACE

class CntTransformContact
{
public:
	CntTransformContact();
	virtual ~CntTransformContact();

public:
	QContact transformContactL(CContactItem &contact);
	void transformPostSaveDetailsL(
	        const CContactItem& contactItem,
	        QContact& contact,
	        const CContactDatabase &contactDatabase,
	        QString managerUri);
	void transformContactL(
	        QContact &contact,
	        CContactItem &contactItem) const;
	QList<TUid> supportedSortingFieldTypes( QString detailDefinitionName, QString detailFieldName );
	QList<TUid> itemFieldUidsL(const QString detailDefinitionName) const;
    TUint32 GetIdForDetailL(const QContactDetailFilter& detailFilter,bool& isSubtype) const;
    void detailDefinitions(QMap<QString, QContactDetailDefinition>& defaultSchema, const QString& contactType, QContactManager::Error* error) const;
    QContactDetail *transformGuidItemFieldL(const CContactItem &contactItem, const CContactDatabase &contactDatabase) const;
    QContactDetail *transformTimestampItemFieldL(const CContactItem &contactItem, const CContactDatabase &contactDatabase);
    void generateThumbnailDetail(QContact &contact) const;
private:
	enum ContactData
	{
		Name = 0,
		Nickname,
		PhoneNumber,
		EmailAddress,
		Address,
		URL,
		OnlineAccount,
		Presence,
		Birthday,
		Organisation,
		Avatar,
		SyncTarget,
		Gender,
		Anniversary,
		Geolocation,
		Note,
		Family,
        Ringtone,
        Thumbnail,
        Favorite,
		Empty
	};

	void initializeCntTransformContactData();
	QList<CContactItemField *> transformDetailL(const QContactDetail &detail) const;
	QContactDetail *transformItemField(const CContactItemField& field, const QContact &contact);
	void transformPreferredDetailL(const QContact& contact, const QContactDetail& detail, QList<CContactItemField*> &fieldList) const;
	void transformPreferredDetail(const CContactItemField& field, const QContactDetail& detail, QContact& contact) const;
	void resetTransformObjects() const;
	

private:
	QMap<ContactData, CntTransformContactData*> m_transformContactData;
	QHash<TUint32, CntTransformContactData*> m_fieldTypeToTransformContact;
	CTzConverter* m_tzConverter;
	RTz m_tzoneServer;
};

#endif /* TRANSFORMCONCTACT_H_ */
