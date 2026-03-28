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
	Fail						= 0      UMETA(DisplayName = "Fail"),
	Success						= 1 << 0 UMETA(DisplayName = "Success"), // 0b0001
	HierarchyInitialization		= 1 << 1 UMETA(Hidden /*DisplayName = "Hierarchy Initialization"*/) // 0b0010
};