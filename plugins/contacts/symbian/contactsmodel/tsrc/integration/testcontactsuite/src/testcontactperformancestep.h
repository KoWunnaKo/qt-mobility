/*
* Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
* Contact: http://www.qt-project.org/legal
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/




#if (!defined __TESTCONTACTPERFORMANCESTEP_H__)
#define __TESTCONTACTPERFORMANCESTEP_H__
#include <test/testexecutestepbase.h>
class CTestContactPerformanceStep : public CTestStep
	{
public:
	CTestContactPerformanceStep();
	~CTestContactPerformanceStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	CActiveScheduler* iScheduler;
	void FieldPerformanceL();
	void UpdatePerformanceL(void);
	void DeletePerformanceL(void);
	static TInt iStorage[];
	static TUid iFieldUid[];
	static TUid iVcardUid[];
	TInt iYear, iMonth, iDay, iNoOfCI;
	};

_LIT(KTestContactPerformanceStep,"TestContactPerformanceStep");

#endif
