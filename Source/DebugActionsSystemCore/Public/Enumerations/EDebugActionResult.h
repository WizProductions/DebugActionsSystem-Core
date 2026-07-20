// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EDebugActionResult.generated.h"

//#############################################################################
//##-------------------------------- ENUMERATION ----------------------------##
//#############################################################################

/**
* The enum used as result of execute debug action method, used for init hierarchy at start and result of action execution.
*/
UENUM(Blueprintable)
enum class EDebugActionResult : uint8 {
	Fail                        = 0 UMETA(DisplayName = "Fail"), //-> Default value of result
	Success                     = 1 UMETA(DisplayName = "Success"),
	SuccessWithoutColorFeedback = 2 UMETA(DisplayName = "Success without color feedback"),
	NotImplemented              = 253 UMETA(Hidden /*DisplayName = "Not implemented"*/),
	HierarchyInitialization     = 254 UMETA(Hidden /*DisplayName = "Hierarchy Initialization"*/)
};
