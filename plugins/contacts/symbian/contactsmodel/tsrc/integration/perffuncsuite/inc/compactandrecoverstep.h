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




/**
 @file 
 @publishedAll
 @released
*/
#ifndef __COMPACTRECOVER_STEP_H__
#define __COMPACTRECOVER_STEP_H__
#include <test/testexecutestepbase.h>
#include "performancefunctionalitybase.h"

class CCompactAndRecoverStep : public CPerformanceFunctionalityBase
	{
public:
	CCompactAndRecoverStep();
	TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
	virtual void InitializeL();
	virtual void Cleanup();

private:
	void PreTestL();
	void TestWrapperL( void (CCompactAndRecoverStep::*aOperations)(const TInt ),  
									const TInt aErr, 
									const TInt aErr2, 
									const TInt aErr3, 
									const TInt aErr4);
	void CRUDOperationsL(const TInt aError);
	void TransactionOperations(const TInt aError);
	void SimpleOperationsL(const TInt aError);
	void OpenAsyncTestL();


private:
	CContactDatabase* 		iContactsDatabase2;
	TBool					iRecoverFlag;//perform active recover, otherwise perform active compress
	CContactOpenOperation* 	iContactOpenOperation;//asynchronous open 
	TRequestStatus 			iStatus;//status of asynchronous open
	};

_LIT(KCompactAndRecoverStep,"CompactAndRecoverStep");

#endif
