// Copyright Wiz Corporation. All Rights Reserved.

#include "Inputs/DebugInput.h"
#include "Components/CanvasPanelSlot.h"
#include "DebugActionsSystemCoreDefines.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

void UDebugInput::OnAddedToSlot_Implementation(UDebugInputSlotWidgetBase* InSlot) {

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

void UDebugInput::OnRemovedFromSlot_Implementation(class UDebugInputSlotWidgetBase* InSlot) {
	MyDebugInputSlotWidget = NULL;
}

void UDebugInput::SetWidgetVisibility_Implementation(ESlateVisibility InVisibility) {
	
	if (MyInputDataWidget == NULL)
		WIZ_LOG("Widget is invalid, make sure UDebugInput has set MyWidget to created widget.", Error, LogDebugActionsSystem);
	
	MyInputDataWidget->SetVisibility(InVisibility);
}

void UDebugInput::RefreshValues_Implementation() {}
