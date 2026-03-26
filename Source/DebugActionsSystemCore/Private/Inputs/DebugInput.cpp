// Copyright Wiz Corporation. All Rights Reserved.

#include "Inputs/DebugInput.h"
#include "Components/CanvasPanelSlot.h"
#include "DebugActionsSystemCoreDefines.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

void UDebugInput::OnAddedToSlot(UDebugInputSlotWidgetBase* InSlot) {

	MyDebugInputSlotWidget = InSlot;
	
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

void UDebugInput::OnRemovedFromSlot(class UDebugInputSlotWidgetBase* InSlot) {
	MyDebugInputSlotWidget = NULL;
}

void UDebugInput::SetWidgetVisibility(ESlateVisibility InVisibility) {
	if (MyInputDataWidget) {
		MyInputDataWidget->SetVisibility(InVisibility);
	}
	else {
		WIZ_LOG("Widget is invalid, make sure UDebugInput has set MyWidget to created widget.", Error, LogDebugActionsSystem);
	}
}

void UDebugInput::RefreshValues() {}
