// Copyright Wiz Corporation. All Rights Reserved.


#include "Structs/SharedDIMapKey.h"
#include "Inputs/DebugInput.h"

bool FSharedDIMapKey::operator==(const FSharedDIMapKey& Rhs) const {
	return KeyTag == Rhs.KeyTag && InputClass == Rhs.InputClass;
}
