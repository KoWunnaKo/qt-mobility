/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the Qt Mobility Components.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** No Commercial Usage
 ** This file contains pre-release code and may not be distributed.
 ** You may use this file in accordance with the terms and conditions
 ** contained in the Technology Preview License Agreement accompanying
 ** this package.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Nokia gives you certain additional
 ** rights.  These rights are described in the Nokia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Nokia at qt-info@nokia.com.
 **
 **
 **
 **
 **
 **
 **
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/
#include "qlandmarkutility.h"
#include <QDebug>
_LIT8(Klmx,"application/vnd.nokia.landmarkcollection+xml");
_LIT8(Kgpx,"application/gps+xml");
_LIT8(Kkml,"application/vnd.google-earth.kml+xml");
_LIT8(Kkmz,"application/vnd.google-earth.kmz");

/*
 * Landmark Utility To convert qt landmark data class to symbian landmark data classes
 */

/*
 * convert symbian landmar to qt landmark
 * 
 */
QLandmark* LandmarkUtility::convertToQtLandmark(QString managerUri, CPosLandmark* symbianLandmark)
{
    QLandmark* qtLandmark = new QLandmark();

    if (!symbianLandmark || managerUri.size() <= 0)
        return qtLandmark;

    TBuf<KPosLmMaxTextFieldLength> lmBuf;

    // set name
    TPtrC lmName;
    symbianLandmark->GetLandmarkName(lmName);
    if (lmName.Length() > 0) {
        lmBuf.Copy(lmName);
        QString landmarkName((QChar*) (lmBuf.Ptr()), lmBuf.Length());
        if (landmarkName.size() > 0)
            qtLandmark->setName(landmarkName);
    }

    // set QLandmarkId
    TPosLmItemId lmId = symbianLandmark->LandmarkId();
    if (lmId > 0) {
        qtLandmark->setLandmarkId(convertToQtLandmarkId(managerUri, lmId));
    }

    // set category ids
    QList<QLandmarkCategoryId> catIdList = getCategoryIds(managerUri, symbianLandmark);
    if (catIdList.size() > 0) {
        qtLandmark->setCategoryIds(catIdList);
    }

    // set landmark description
    TPtrC lmDesc;
    symbianLandmark->GetLandmarkDescription(lmDesc);
    if (lmDesc.Length() > 0) {
        TBuf<KPosLmMaxDescriptionLength> lmDescBuf;
        lmDescBuf.Copy(lmDesc);
        QString landmarkDesc((QChar*) (lmDescBuf.Ptr()), lmDescBuf.Length());
        qtLandmark->setDescription(landmarkDesc);
    }

    // set icon info
    TPtrC iconDesc;
    TInt iconIndex = 0;
    TInt iconMaskIndex = 0;
    symbianLandmark->GetIcon(iconDesc, iconIndex, iconMaskIndex);
    if (iconDesc.Length() > 0) {
        lmBuf.Copy(iconDesc);
        QString landmarkIcon((QChar*) (lmBuf.Ptr()), lmBuf.Length());
        qtLandmark->setIconUrl(landmarkIcon);
    }

    // set coordinate    
    TLocality local;
    TInt result = symbianLandmark->GetPosition(local);
    if (result == KErrNone) {
        if (!Math::IsNaN(local.Latitude()) || !Math::IsNaN(local.Longitude())) {
            QGeoCoordinate coord(local.Latitude(), local.Longitude(), local.Altitude());
            qtLandmark->setCoordinate(coord);
        }
    }

    // set radius
    TReal32 covRadius;
    symbianLandmark->GetCoverageRadius(covRadius);
    if (covRadius > 0) {
        qtLandmark->setRadius(covRadius);
    }

    // SETTING OF OTHER ATTRIBUTES / OTHER POSITION FIELDS 

    if (symbianLandmark->NumOfAvailablePositionFields() > 0) {

        // set phone
        if (symbianLandmark->IsPositionFieldAvailable(EPositionFieldBuildingTelephone)) {
            TPtrC phoneNum;
            symbianLandmark->GetPositionField(EPositionFieldBuildingTelephone, phoneNum);
            if (phoneNum.Length() > 0) {
                lmBuf.Copy(phoneNum);
                QString LandmarkPhoneNo((QChar*) (lmBuf.Ptr()), lmBuf.Length());
                qtLandmark->setPhoneNumber(LandmarkPhoneNo);
            }
        }

        // set landmark url
        if (symbianLandmark->IsPositionFieldAvailable(EPositionFieldMediaLinks)) {
            TPtrC lmUrl;
            symbianLandmark->GetPositionField(EPositionFieldMediaLinks, lmUrl);
            if (lmUrl.Length() > 0) {
                lmBuf.Copy(lmUrl);
                QString LandmarkUrl((QChar*) (lmBuf.Ptr()), lmBuf.Length());
                qtLandmark->setUrl(LandmarkUrl);
            }
        }

        // set address 
        // setting street, city, district, state, country, postcode/zipcode etc.
        QGeoAddress address;

        // set city 
        if (symbianLandmark->IsPositionFieldAvailable(EPositionFieldCity)) {
            TPtrC posField;
            symbianLandmark->GetPositionField(EPositionFieldCity, posField);
            if (posField.Length() > 0) {
                lmBuf.Copy(posField);
                QString lmPosField((QChar*) (lmBuf.Ptr()), lmBuf.Length());
                address.setCity(lmPosField);
            }
        }

        // set district
        if (symbianLandmark->IsPositionFieldAvailable(EPositionFieldDistrict)) {
            TPtrC posField;
            symbianLandmark->GetPositionField(EPositionFieldDistrict, posField);
            if (posField.Length() > 0) {
                lmBuf.Copy(posField);
                QString lmPosField((QChar*) (lmBuf.Ptr()), lmBuf.Length());
                address.setDistrict(lmPosField);
            }
        }

        // set county
        if (symbianLandmark->IsPositionFieldAvailable(EPositionFieldCounty)) {
            TPtrC posField;
            symbianLandmark->GetPositionField(EPositionFieldCounty, posField);
            if (posField.Length() > 0) {
                lmBuf.Copy(posField);
                QString lmPosField((QChar*) (lmBuf.Ptr()), lmBuf.Length());
                address.setCounty(lmPosField);
            }
        }
        
        // set state / province
        if (symbianLandmark->IsPositionFieldAvailable(EPositionFieldState)) {
            TPtrC posField;
            symbianLandmark->GetPositionField(EPositionFieldState, posField);
            if (posField.Length() > 0) {
                lmBuf.Copy(posField);
                QString lmPosField((QChar*) (lmBuf.Ptr()), lmBuf.Length());
                address.setState(lmPosField);
            }
        }

        // set country
        if (symbianLandmark->IsPositionFieldAvailable(EPositionFieldCountry)) {
            TPtrC posField;
            symbianLandmark->GetPositionField(EPositionFieldCountry, posField);
            if (posField.Length() > 0) {
                lmBuf.Copy(posField);
                QString lmPosField((QChar*) (lmBuf.Ptr()), lmBuf.Length());
                address.setCountry(lmPosField);
            }
        }

        // set country code
        if (symbianLandmark->IsPositionFieldAvailable(EPositionFieldCountryCode)) {
            TPtrC posField;
            symbianLandmark->GetPositionField(EPositionFieldCountryCode, posField);
            if (posField.Length() > 0) {
                lmBuf.Copy(posField);
                QString lmPosField((QChar*) (lmBuf.Ptr()), lmBuf.Length());
                address.setCountryCode(lmPosField);
            }
        }

        // set post code / zip code
        if (symbianLandmark->IsPositionFieldAvailable(EPositionFieldPostalCode)) {
            TPtrC posField;
            symbianLandmark->GetPositionField(EPositionFieldPostalCode, posField);
            if (posField.Length() > 0) {
                lmBuf.Copy(posField);
                QString lmPosField((QChar*) (lmBuf.Ptr()), lmBuf.Length());
                address.setPostCode(lmPosField);
            }
        }

        // set street 
        if (symbianLandmark->IsPositionFieldAvailable(EPositionFieldStreet)) {
            TPtrC posField;
            symbianLandmark->GetPositionField(EPositionFieldStreet, posField);
            if (posField.Length() > 0) {
                lmBuf.Copy(posField);
                QString lmPosField((QChar*) (lmBuf.Ptr()), lmBuf.Length());
                address.setStreet(lmPosField);
            }
        }

        // set QGeoAddress with above info
        qtLandmark->setAddress(address);

    }
    return qtLandmark;
}

/*
 * convert qt landmark to  symbian landmark
 * 
 */
void LandmarkUtility::setSymbianLandmarkL(CPosLandmark& symbianLandmark, QLandmark* qtLandmark)
{
    if (!qtLandmark)
        return;

    // set landmark name
    QString lmName = qtLandmark->name();
    if (lmName.length() > 0) {
        TPtrC symbianLmName(reinterpret_cast<const TText*> (lmName.constData()), lmName.length());
        symbianLandmark.SetLandmarkNameL(symbianLmName);
    }

    // set coordinate
    QGeoCoordinate coord = qtLandmark->coordinate();
    TLocality local;
    if (isValidLat(coord.latitude()) && isValidLong(coord.longitude())) {
        local.SetCoordinate(coord.latitude(), coord.longitude(), coord.altitude());
        symbianLandmark.SetPositionL(local);
    }

    // set coverage radius
    qreal rad = qtLandmark->radius();
    if (rad > 0) {
        symbianLandmark.SetCoverageRadius(rad);
    }

    // set landmark description
    QString lmDesc = qtLandmark->description();
    if (lmDesc.length() > 0) {
        TPtrC symbianLmDesc(reinterpret_cast<const TText*> (lmDesc.constData()), lmDesc.length());
        symbianLandmark.SetLandmarkDescriptionL(symbianLmDesc);
    }

    // set icon info
    QString lmIconInfo = qtLandmark->iconUrl().toString();
    if (lmIconInfo.length() > 0) {
        TPtrC symbianLmIcon(reinterpret_cast<const TText*> (lmIconInfo.constData()),
            lmIconInfo.length());
        if (symbianLmIcon.Length() > 0) {
            int iconIdx = 0;
            int iconMaskIdx = 0;
            symbianLandmark.SetIconL(symbianLmIcon, iconIdx, iconMaskIdx);
        }
    }

    // set category ids
    QList<QLandmarkCategoryId> catList = qtLandmark->categoryIds();
    if (catList.count() > 0) {
        for (int i = 0; i < catList.count(); ++i) {
            TPosLmItemId catId = convertToSymbianLandmarkCategoryId(catList.at(i));
            symbianLandmark.AddCategoryL(catId);
        }
    }

    // set phone
    QString lmPhoneNo = qtLandmark->phoneNumber();
    if (lmPhoneNo.length() > 0) {
        TPtrC symbianLmPhone(reinterpret_cast<const TText*> (lmPhoneNo.constData()),
            lmPhoneNo.length());
        symbianLandmark.SetPositionFieldL(EPositionFieldBuildingTelephone, symbianLmPhone);
    }

    // set url
    QString lmUrl = qtLandmark->url().toString();
    if (lmUrl.length() > 0) {
        TPtrC symbianLmUrl(reinterpret_cast<const TText*> (lmUrl.constData()), lmUrl.length());
        symbianLandmark.SetPositionFieldL(EPositionFieldMediaLinks, symbianLmUrl);
    }

    // set address

    // set city
    QString lmCity = qtLandmark->address().city();
    if (lmCity.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmCity.constData()),
            lmCity.length());
        symbianLandmark.SetPositionFieldL(EPositionFieldCity, symbianLmPosField);
    }

    // set district
    QString lmDistrict = qtLandmark->address().district();
    if (lmDistrict.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmDistrict.constData()),
            lmDistrict.length());
        symbianLandmark.SetPositionFieldL(EPositionFieldDistrict, symbianLmPosField);
    }
    
    // set county
    QString lmCounty = qtLandmark->address().county();
    if (lmCounty.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmCounty.constData()),
            lmCounty.length());
        symbianLandmark.SetPositionFieldL(EPositionFieldCounty, symbianLmPosField);
    }
    
    // set state
    QString lmState = qtLandmark->address().state();
    if (lmState.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmState.constData()),
            lmState.length());
        symbianLandmark.SetPositionFieldL(EPositionFieldState, symbianLmPosField);
    }

    // set country
    QString lmCountry = qtLandmark->address().country();
    if (lmCountry.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmCountry.constData()),
            lmCountry.length());
        symbianLandmark.SetPositionFieldL(EPositionFieldCountry, symbianLmPosField);
    }

    // set country code
    QString lmCountryCode = qtLandmark->address().countryCode();
    if (lmCountryCode.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmCountryCode.constData()),
            lmCountryCode.length());
        symbianLandmark.SetPositionFieldL(EPositionFieldCountryCode, symbianLmPosField);
    }

    // set postcode
    QString lmPostalCode = qtLandmark->address().postCode();
    if (lmPostalCode.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmPostalCode.constData()),
            lmPostalCode.length());
        symbianLandmark.SetPositionFieldL(EPositionFieldPostalCode, symbianLmPosField);
    }

    // set street
    QString lmStreet = qtLandmark->address().street();
    if (lmStreet.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmStreet.constData()),
            lmStreet.length());
        symbianLandmark.SetPositionFieldL(EPositionFieldStreet, symbianLmPosField);
    }
}

CPosLandmark* LandmarkUtility::convertToSymbianLandmarkL(QLandmark* qtLandmark)
{
    if (!qtLandmark)
        return 0;

    CPosLandmark* symbianLandmark = CPosLandmark::NewL();
    CleanupStack::PushL(symbianLandmark);

    // set landmark name
    QString lmName = qtLandmark->name();
    if (lmName.length() > 0) {
        TPtrC symbianLmName(reinterpret_cast<const TText*> (lmName.constData()), lmName.length());
        symbianLandmark->SetLandmarkNameL(symbianLmName);
    }

    // set coordinate
    QGeoCoordinate coord = qtLandmark->coordinate();
    TLocality local;
    if (isValidLat(coord.latitude()) && isValidLong(coord.longitude())) {
        local.SetCoordinate(coord.latitude(), coord.longitude(), coord.altitude());
        symbianLandmark->SetPositionL(local);
    }

    // set coverage radius
    qreal rad = qtLandmark->radius();
    if (rad > 0) {
        symbianLandmark->SetCoverageRadius(rad);
    }

    // set landmark description
    QString lmDesc = qtLandmark->description();
    if (lmDesc.length() > 0) {
        TPtrC symbianLmDesc(reinterpret_cast<const TText*> (lmDesc.constData()), lmDesc.length());
        symbianLandmark->SetLandmarkDescriptionL(symbianLmDesc);
    }

    // set icon info
    QString lmIconInfo = qtLandmark->iconUrl().toString();
    if (lmIconInfo.length() > 0) {
        TPtrC symbianLmIcon(reinterpret_cast<const TText*> (lmIconInfo.constData()),
            lmIconInfo.length());
        if (symbianLmIcon.Length() > 0) {
            int iconIdx = 0;
            int iconMaskIdx = 0;
            symbianLandmark->SetIconL(symbianLmIcon, iconIdx, iconMaskIdx);
        }
    }

    // set category ids
    QList<QLandmarkCategoryId> catList = qtLandmark->categoryIds();
    if (catList.count() > 0) {
        for (int i = 0; i < catList.count(); ++i) {
            TPosLmItemId catId = convertToSymbianLandmarkCategoryId(catList.at(i));
            symbianLandmark->AddCategoryL(catId);
        }
    }

    // set phone
    QString lmPhoneNo = qtLandmark->phoneNumber();
    if (lmPhoneNo.length() > 0) {
        TPtrC symbianLmPhone(reinterpret_cast<const TText*> (lmPhoneNo.constData()),
            lmPhoneNo.length());
        symbianLandmark->SetPositionFieldL(EPositionFieldBuildingTelephone, symbianLmPhone);
    }

    // set url
    QString lmUrl = qtLandmark->url().toString();
    if (lmUrl.length() > 0) {
        TPtrC symbianLmUrl(reinterpret_cast<const TText*> (lmUrl.constData()), lmUrl.length());
        symbianLandmark->SetPositionFieldL(EPositionFieldMediaLinks, symbianLmUrl);
    }

    // set address

    // set city
    QString lmCity = qtLandmark->address().city();
    if (lmCity.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmCity.constData()),
            lmCity.length());
        symbianLandmark->SetPositionFieldL(EPositionFieldCity, symbianLmPosField);
    }

    // set district
    QString lmDistrict = qtLandmark->address().district();
    if (lmDistrict.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmDistrict.constData()),
            lmDistrict.length());
        symbianLandmark->SetPositionFieldL(EPositionFieldDistrict, symbianLmPosField);
    }

    // set county
    QString lmCounty = qtLandmark->address().county();
    if (lmCounty.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmCounty.constData()),
            lmCounty.length());
        symbianLandmark->SetPositionFieldL(EPositionFieldCounty, symbianLmPosField);
    }
    
    // set state
    QString lmState = qtLandmark->address().state();
    if (lmState.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmState.constData()),
            lmState.length());
        symbianLandmark->SetPositionFieldL(EPositionFieldState, symbianLmPosField);
    }

    // set country
    QString lmCountry = qtLandmark->address().country();
    if (lmCountry.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmCountry.constData()),
            lmCountry.length());
        symbianLandmark->SetPositionFieldL(EPositionFieldCountry, symbianLmPosField);
    }

    // set country code
    QString lmCountryCode = qtLandmark->address().countryCode();
    if (lmCountryCode.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmCountryCode.constData()),
            lmCountryCode.length());
        symbianLandmark->SetPositionFieldL(EPositionFieldCountryCode, symbianLmPosField);
    }

    // set postcode
    QString lmPostalCode = qtLandmark->address().postCode();
    if (lmPostalCode.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmPostalCode.constData()),
            lmPostalCode.length());
        symbianLandmark->SetPositionFieldL(EPositionFieldPostalCode, symbianLmPosField);
    }

    // set street
    QString lmStreet = qtLandmark->address().street();
    if (lmStreet.length() > 0) {
        TPtrC symbianLmPosField(reinterpret_cast<const TText*> (lmStreet.constData()),
            lmStreet.length());
        symbianLandmark->SetPositionFieldL(EPositionFieldStreet, symbianLmPosField);
    }

    CleanupStack::Pop(symbianLandmark);
    return symbianLandmark;
}

/*
 * convert symbian landmark id to qt landmark id
 * 
 */
QLandmarkId LandmarkUtility::convertToQtLandmarkId(QString managerUri,
    TPosLmItemId symbianLandmarkId)
{
    QLandmarkId qtLandmarkId;
    QString id;
    id.setNum(symbianLandmarkId);

    qtLandmarkId.setLocalId(id);
    qtLandmarkId.setManagerUri(managerUri);
    return qtLandmarkId;
}

/*
 * convert symbian landmark ids to qt landmark ids
 * 
 */
QList<QLandmarkId> LandmarkUtility::convertToQtLandmarkIds(QString managerUri,
    RArray<TPosLmItemId>& symbianLandmarkIds)
{
    QList<QLandmarkId> qtLandmarkIds;
    for (int i = 0; i < symbianLandmarkIds.Count(); ++i) {
        QLandmarkId qtLmId = convertToQtLandmarkId(managerUri, symbianLandmarkIds[i]);
        qtLandmarkIds.append(qtLmId);
    }
    return qtLandmarkIds;
}

/*
 * convert qt landmark id to symbian landmark id
 * 
 */
TPosLmItemId LandmarkUtility::convertToSymbianLandmarkId(QLandmarkId qtLandmarkId)
{
    TPosLmItemId symbianLandmarkId = KPosLmNullItemId;
    QString localId = qtLandmarkId.localId();
    if (validLocalId(localId)) {
        symbianLandmarkId = localId.toULong();
    }
    return symbianLandmarkId;
}

/*
 * returns symbian landmark ids from qt landmark id list. 
 */
RArray<TPosLmItemId> LandmarkUtility::getSymbianLandmarkIds(QList<QLandmarkId>& qtLandmarkIds)
{
    RArray<TPosLmItemId> symbianLandmarkIds;
    for (int i = 0; i < qtLandmarkIds.size(); ++i) {
        TPosLmItemId lmId = convertToSymbianLandmarkId(qtLandmarkIds.at(i));
        if (lmId == KPosLmNullItemId) {
            symbianLandmarkIds.Reset();
            return symbianLandmarkIds;
        }
        else
            symbianLandmarkIds.Append(lmId);
    }
    return symbianLandmarkIds;
}

/*
 * convert symbian category to qt category
 * 
 */
QLandmarkCategory* LandmarkUtility::convertToQtLandmarkCategory(QString managerUri,
    CPosLandmarkCategory* symbianLandmarkCategory)
{
    QLandmarkCategory* qtLmCategory = new QLandmarkCategory();

    if (!symbianLandmarkCategory || managerUri.size() <= 0)
        return qtLmCategory;

    TBuf<KPosLmMaxCategoryNameLength> lmBuf;

    // set category name
    TPtrC catName;
    symbianLandmarkCategory->GetCategoryName(catName);
    if (catName.Length() > 0) {
        lmBuf.Copy(catName);
        QString categoryName((QChar*) (lmBuf.Ptr()), lmBuf.Length());
        qtLmCategory->setName(categoryName);
    }

    // set category id    
    TPosLmItemId catId = symbianLandmarkCategory->CategoryId();
    if (catId != KPosLmNullItemId) {
        qtLmCategory->setCategoryId(convertToQtLandmarkCategoryId(managerUri, catId));
    }

    // set icon info
    TPtrC iconDesc;
    TInt iconIndex, iconMaskIndex;
    symbianLandmarkCategory->GetIcon(iconDesc, iconIndex, iconMaskIndex);
    if (iconDesc.Length() > 0) {
        lmBuf.Copy(iconDesc);
        QString landmarkIcon((QChar*) (lmBuf.Ptr()), lmBuf.Length());
        qtLmCategory->setIconUrl(landmarkIcon);
    }

    return qtLmCategory;
}

/*
 * convert qt category to symbian category 
 * 
 */
CPosLandmarkCategory* LandmarkUtility::convertToSymbianLandmarkCategoryL(
    QLandmarkCategory* qtLandmarkCategory)
{
    if (!qtLandmarkCategory)
        return 0;

    CPosLandmarkCategory* symbianLmCategory = CPosLandmarkCategory::NewL();
    CleanupStack::PushL(symbianLmCategory);

    // set category name
    QString catName = qtLandmarkCategory->name();
    if (catName.length() > 0) {
        TPtrC
            symbianCatName(reinterpret_cast<const TText*> (catName.constData()), catName.length());
        symbianLmCategory->SetCategoryNameL(symbianCatName);
    }

    // set icon info
    QString catIconInfo = qtLandmarkCategory->iconUrl().toString();
    if (catIconInfo.length() > 0) {
        TPtrC symbianLmIcon(reinterpret_cast<const TText*> (catIconInfo.constData()),
            catIconInfo.length());
        if (symbianLmIcon.Length() > 0) {
            int iconIdx = 0;
            int iconMaskIdx = 0;
            symbianLmCategory->SetIconL(symbianLmIcon, iconIdx, iconMaskIdx);
        }
    }

    CleanupStack::Pop(symbianLmCategory);
    return symbianLmCategory;
}

/*
 * sets symbian category from qt category 
 * 
 */
void LandmarkUtility::setSymbianLandmarkCategoryL(CPosLandmarkCategory& symbianLmCategory,
    QLandmarkCategory* qtLandmarkCategory)
{
    if (!qtLandmarkCategory)
        return;

    // set category name
    QString catName = qtLandmarkCategory->name();
    if (catName.length() > 0) {
        TPtrC
            symbianCatName(reinterpret_cast<const TText*> (catName.constData()), catName.length());
        symbianLmCategory.SetCategoryNameL(symbianCatName);
    }

    // set icon info
    QString catIconInfo = qtLandmarkCategory->iconUrl().toString();
    if (catIconInfo.length() > 0) {
        TPtrC symbianLmIcon(reinterpret_cast<const TText*> (catIconInfo.constData()),
            catIconInfo.length());
        if (symbianLmIcon.Length() > 0) {
            int iconIdx = 0;
            int iconMaskIdx = 0;
            symbianLmCategory.SetIconL(symbianLmIcon, iconIdx, iconMaskIdx);
        }
    }
}

/*
 * convert symbian category id to qt category id 
 * 
 */
QLandmarkCategoryId LandmarkUtility::convertToQtLandmarkCategoryId(QString managerUri,
    TPosLmItemId symbianLandmarkCategoryId)
{
    QLandmarkCategoryId qtLmCategoryId;
    QString localId;
    localId.setNum(symbianLandmarkCategoryId);
    qtLmCategoryId.setLocalId(localId);
    qtLmCategoryId.setManagerUri(managerUri);
    return qtLmCategoryId;
}

/*
 * convert symbian landmark category ids to qt landmark category ids
 * 
 */
QList<QLandmarkCategoryId> LandmarkUtility::convertToQtLandmarkCategoryIds(QString managerUri,
    RArray<TPosLmItemId>& symbianCategoryIds)
{
    QList<QLandmarkCategoryId> qtCategoryIds;
    for (int i = 0; i < symbianCategoryIds.Count(); ++i) {
        QLandmarkCategoryId qtLmId = convertToQtLandmarkCategoryId(managerUri,
            symbianCategoryIds[i]);
        qtCategoryIds.append(qtLmId);
    }
    return qtCategoryIds;
}

/*
 * convert qt category id to symbian category id
 * 
 */
TPosLmItemId LandmarkUtility::convertToSymbianLandmarkCategoryId(
    QLandmarkCategoryId qtLandmarkCategoryId)
{
    TPosLmItemId symbianLmCategoryId = KPosLmNullItemId;
    QString localId = qtLandmarkCategoryId.localId();
    if (validLocalId(localId)) {
        symbianLmCategoryId = localId.toULong();
    }
    return symbianLmCategoryId;
}

/*
 * get symbian landmark id from qt landmark
 * 
 */
TPosLmItemId LandmarkUtility::getSymbianLandmarkId(QLandmark* qtLandmark)
{
    TPosLmItemId lmId = KPosLmNullItemId;
    if (qtLandmark)
        lmId = qtLandmark->landmarkId().localId().toUInt();

    return lmId;
}

/*
 * get category info from symbian landmark
 * 
 */
QList<QLandmarkCategoryId> LandmarkUtility::getCategoryIds(QString managerUri,
    CPosLandmark* symbianLandmark)
{
    QList<QLandmarkCategoryId> qtCategoryIds;

    RArray<TPosLmItemId> categoryIdArray;
    TRAPD(err, symbianLandmark->GetCategoriesL(categoryIdArray));
    if (err == KErrNone && categoryIdArray.Count() > 0) {
        for (int i = 0; i < categoryIdArray.Count(); ++i) {
            QLandmarkCategoryId qtCat;
            QString localId;
            localId.setNum(categoryIdArray[i]);
            qtCat.setLocalId(localId);
            qtCat.setManagerUri(managerUri);
            qtCategoryIds.append(qtCat);
        }
    }
    categoryIdArray.Close();
    return qtCategoryIds;
}

/*
 * validate landmark categories
 * 
 */
bool LandmarkUtility::validCategoriesExist(CPosLmCategoryManager* catMgr, QLandmark* qtLandmark)
{
    bool result = false;

    QList<QLandmarkCategoryId> catList = qtLandmark->categoryIds();
    if (catList.size() > 0) {
        qDebug() << "category list size = " << catList.size();
        for (int i = 0; i < catList.size(); ++i) {
            TPosLmItemId symbianCatId = convertToSymbianLandmarkCategoryId(catList.at(i));
            CPosLandmarkCategory* symbiancat = NULL;
            TRAPD(err, symbiancat = catMgr->ReadCategoryLC(symbianCatId);
                if (symbiancat!=NULL) CleanupStack::Pop( symbiancat ) )

            if (err != KErrNone) {
                result = false;
                break;
            }
            result = true;
        }
    }
    else {
        result = true;
    }
    return result;
}

/*
 * validate localid
 * 
 */
bool LandmarkUtility::validLocalId(QString localId)
{
    bool result = false;

    if (!localId.isEmpty()) {
        //qDebug() << "local id = " << localId;
        for (int i = 0; i < localId.size(); ++i) {
            QChar ch = localId.operator [](i);
            if (!ch.isNumber()) {
                result = false;
                break;
            }
            result = true;
        }
    }
    return result;
}

/*
 * get symbian landmark category from qt landmark category ids
 * Note: Assumption is that all are valid category ids and exist in landmarks database
 * 
 */
QList<QLandmarkCategory> LandmarkUtility::getCategoriesL(CPosLmCategoryManager* catMgr, QList<
    QLandmarkCategoryId> qtCategoryIds)
{
    QList<QLandmarkCategory> qtCategoryList;
    QString managerUri = qtCategoryIds.operator [](0).managerUri();

    for (int i = 0; i < qtCategoryIds.size(); ++i) {
        CPosLandmarkCategory* symbianCat = NULL;
        TPosLmItemId symbianCatId =
            convertToSymbianLandmarkCategoryId(qtCategoryIds.operator [](i));
        TRAPD(err, symbianCat = catMgr->ReadCategoryLC(symbianCatId);
            if (symbianCat!=NULL) CleanupStack::Pop( symbianCat );)
        if (err == KErrNone) {
            QLandmarkCategory* qtCat = convertToQtLandmarkCategory(managerUri, symbianCat);
            qtCategoryList.append(*qtCat);
            delete symbianCat;
            symbianCat = NULL;
        }
    }

    return qtCategoryList;
}

/*
 * verify global category id
 * 
 */
bool LandmarkUtility::isGlobalCategoryId(CPosLmCategoryManager* catMgr,
    QLandmarkCategoryId qtCategoryId)
{
    bool result = false;
    QStringList globalCategories;
    globalCategories << "3000" << "6000" << "9000" << "12000" << "15000" << "18000" << "21000"
        << "24000" << "27000" << "30000" << "33000" << "36000" << "39000" << "42000" << "45000";

    TPosLmItemId glCatId = convertToSymbianLandmarkCategoryId(qtCategoryId);

    for (int i = 0; i < globalCategories.size(); ++i) {
        TPosLmGlobalCategory gblCat = globalCategories.operator [](i).toUShort();
        TPosLmItemId gId = KPosLmNullItemId;
        TRAPD(err, gId= catMgr->GetGlobalCategoryL(gblCat);)
        if (err == KErrNone) {
            if (gId != KPosLmNullItemId && glCatId == gId) {
                result = true;
                break;
            }
        }
    }

    return result;
}

/*
 * list of landmark attribute keys 
 * 
 */
QStringList LandmarkUtility::landmarkAttributeKeys()
{
    QStringList commonKeys = QStringList()
                             << "name"
                             << "description"
                             << "iconUrl"
                             << "radius"
                             << "phoneNumber"
                             << "url"
                             << "latitude"
                             << "longitude"
                             << "altitude"
                             << "country"
                             << "countryCode"
                             << "state"
                             << "county"
                             << "city"
                             << "district"
                             << "street"
                             << "postCode";
    return commonKeys;
}

/*
 * list of category attribute keys
 *
 */
QStringList LandmarkUtility::categoryAttributeKeys()
{
    QStringList commonKeys = QStringList()
                             << "name"
                             << "iconUrl";
    return commonKeys;
}

/**
 * converts attribute key string to symbian position field id value
 */
TPositionFieldId LandmarkUtility::positionFieldId(QString keyValue)
{
    TPositionFieldId fieldId = 0;

    if (keyValue == "phone")
        fieldId = EPositionFieldBuildingTelephone;
    else if (keyValue == "street")
        fieldId = EPositionFieldStreet;
    else if (keyValue == "county")
        fieldId = EPositionFieldCounty;
    else if (keyValue == "city")
        fieldId = EPositionFieldCity;
    else if (keyValue == "district")
        fieldId = EPositionFieldDistrict;
    else if (keyValue == "state")
        fieldId = EPositionFieldState;
    else if (keyValue == "country")
        fieldId = EPositionFieldCountry;
    else if (keyValue == "countryCode")
        fieldId = EPositionFieldCountryCode;
    else if (keyValue == "postCode")
        fieldId = EPositionFieldPostalCode;

    return fieldId;
}

/**
 * Converts to symbian format string
 */
HBufC8* LandmarkUtility::landmarkPackageFormatsStrL(const QString& format)
{
    HBufC8* buffer = NULL;
    if (format == "lmx")
        buffer = Klmx().AllocLC();
    else if (format == "gpx")
        buffer = Kgpx().AllocLC();
    else if (format == "kml")
        buffer = Kkml().AllocLC();
    else if (format == "kmz")
        buffer = Kkmz().AllocLC();
    else
        User::Leave(KErrNotSupported);

    return buffer;
}

/**
 * Prepares the file path
 */
QString LandmarkUtility::preparePath(QString filename)
{
    int j = 0;

    // Change from / to // (Just to make path symbian compatible)
    while ((j = filename.indexOf("\\", j)) != -1) {
        qDebug() << "Found \\ tag at index position" << j;

        //Check if its already "\\"
        if ((filename.indexOf("\\\\", j) == -1) && (filename.indexOf("\\\\", j) > j)) {
            filename.remove(j, 1);
            filename.insert(j, "\\");
            ++j;
        }
    }
    j = 0;

    //Change from \ to // (Just to make path symbian compatible)    
    while ((j = filename.indexOf("/", j)) != -1) {
        qDebug() << "Found /  at index position" << j;
        filename.remove(j, 1);
        filename.insert(j, "\\");
        ++j;
    }

    return filename;
}

// end of file