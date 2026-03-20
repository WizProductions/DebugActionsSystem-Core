// Copyright Wiz Corporation. All Rights Reserved.

#include "WidgetBases/DebugActionWidgetBase.h"
#include "Actions/DebugActionBase.h"
#include "Enumerations/DebugEnums.h"
#include "SubSystems/DebugSubsystem.h"

#define LOCTEXT_NAMESPACE "UMG"

bool UDebugActionWidgetBase::InitDebugActionWidget_Implementation(UDebugActionBase* LinkedDebugAction) {
	
	MyDebugAction = LinkedDebugAction;
	MyDebugAction->SetMyDebugActionWidget(this);

	if (MyDebugAction) {
		if (MyDebugAction->GetDepthLevel() != 0) {
			MyDebugAction->SetDebugActionWidgetVisibility(true);
		}

		return true;
	}

	return false;
}

#if WITH_EDITOR
const FText UDebugActionWidgetBase::GetPaletteCategory() {
	return LOCTEXT("Debug Actions System", "Debug Actions System");
}
#endif

EDebugActionResult UDebugActionWidgetBase::ExecuteMyDebugAction() {

	return MyDebugAction->ExecuteDebugAction();
}
