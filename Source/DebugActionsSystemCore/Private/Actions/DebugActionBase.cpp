// Copyright Wiz Corporation. All Rights Reserved.

#include "Actions/DebugActionBase.h"
#include "Enumerations/EDebugActionResult.h"
#include "WidgetBases/DebugActionWidgetBase.h"

EDebugActionResult UDebugActionBase::InitializeDebugAction(TArray<TObjectPtr<UDebugActionBase>>& OutDebugActionsHierarchy, UDebugSubsystem* Subsystem) {
	
	bDebugActionState = false;
	MyDebugSubsystem = Subsystem;
	
	return EDebugActionResult::Success;
}

void UDebugActionBase::SetDebugActionWidgetVisibility(bool bNewIsCollapsed, int DepthRecursivity) {
	SetDebugActionWidgetVisibility(bNewIsCollapsed);
}

void UDebugActionBase::SetDebugActionWidgetVisibility(bool bNewIsCollapsed) {
	MyDebugActionWidget->SetVisibility(static_cast<ESlateVisibility>(bNewIsCollapsed));
}

void UDebugActionBase::OnParentFolderIsDeveloped(class UDebugActionFolder* ParentFolder) {}
void UDebugActionBase::OnParentFolderIsCollapsed(class UDebugActionFolder* ParentFolder) {}

EDebugActionResult UDebugActionBase::ExecuteDebugAction() {
	
	bDebugActionState = !bDebugActionState;
	return EDebugActionResult::Success;
}
