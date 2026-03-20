// Copyright Wiz Corporation. All Rights Reserved.

#include "Inputs/DebugInput.h"
#include "Components/CanvasPanelSlot.h"
#include "DebugActionsSystemCoreDefines.h"
#include "WidgetBases/DebugInputSlotBase.h"

void UDebugInput::OnAddedToSlot(UDebugInputSlotBase* InSlot) {

	MyDebugInputSlot = InSlot;
	
	if (InSlot == NULL)
		WIZ_RET_LOG( , "New textblock is invalid.", Error, LogDebugActionsSystem);
	
	InSlot->SetTitle(DebugInputTitle);

	if (auto CPSlot = InSlot->GetInputWidgetSlot()) {
		CPSlot->SetSize(DebugInputSize);
	}
	else {
		WIZ_RET_LOG( , "Textblock slot is invalid.", Error, LogDebugActionsSystem);
	}
}

void UDebugInput::OnRemovedFromSlot(class UDebugInputSlotBase* InSlot) {
	MyDebugInputSlot = NULL;
}

void UDebugInput::SetWidgetVisibility(ESlateVisibility InVisibility) {
	if (MyWidget) {
		MyWidget->SetVisibility(InVisibility);
	}
	else {
		WIZ_LOG("Widget is invalid, make sure UDebugInput has set MyWidget to created widget.", Error, LogDebugActionsSystem);
	}
}

void UDebugInput::RefreshValues() {}
