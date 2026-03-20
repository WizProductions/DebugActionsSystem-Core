// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugEnums.generated.h"

//#############################################################################
//##-------------------------------- ENUMERATION ----------------------------##
//#############################################################################

/**
* Debug action result.
*/
UENUM(Blueprintable)
enum class EDebugActionResult : UINT8 {

	Fail = 0b000,
	Success = 0b0001,
	InitHierarchy = 0b0010
};

/**
* Debug action teleport mode.
*/
UENUM()
enum class EDebugTeleportMode : UINT8 {

	Mouse = 0b000,
	Actor = 0b0001,
	Level = 0b0010
};