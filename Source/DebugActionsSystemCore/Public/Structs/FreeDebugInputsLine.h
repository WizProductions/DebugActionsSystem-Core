// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FreeDebugInputsLine.generated.h"

//#############################################################################
//##-------------------------------- STRUCTURE ------------------------------##
//#############################################################################

/**
* A simple structure for the FreeDebugInputs Map line.
*/
USTRUCT()
struct FFreeDebugInputsLine {
	GENERATED_BODY()

//#############################################################################
//##---------------------------------- FIELDS -------------------------------##
//#############################################################################

//==== Properties ====\\.
public:
	TSet<TObjectPtr<class UDebugInput>> FreeDebugLine;
};
