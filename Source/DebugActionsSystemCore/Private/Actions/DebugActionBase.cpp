// Copyright Wiz Corporation. All Rights Reserved.

#include "Actions/DebugActionBase.h"
#include "Enumerations/EDebugActionResult.h"
#include "WidgetBases/DebugActionWidgetBase.h"

#if WITH_EDITOR
void UDebugActionBase::PostInitProperties() {
	Super::PostInitProperties();
	
	UpdateEditorDataAssetTitle();
	OnPostInitProperties();
}

void UDebugActionBase::UpdateEditorDataAssetTitle() {
	Private_DataAssetActionTitle = GetDebugActionTitle().ToString();
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UDebugActionBase::RefreshDebugDataAssetView() {
	
	//Force refresh the data asset view when the folder title is loaded from the disk
	if (UObject* Outer = GetOuter()) {
		Outer->PostEditChange();
	}
}
#endif

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

void UDebugActionBase::OnParentFolderIsCollapsed_Implementation(class UDebugActionFolder* ParentFolder) {}
void UDebugActionBase::OnParentFolderIsDeveloped_Implementation(class UDebugActionFolder* ParentFolder) {}

EDebugActionResult UDebugActionBase::ExecuteDebugAction_Implementation() {
	bDebugActionState = !bDebugActionState;
	return EDebugActionResult::Success;
}

FText UDebugActionBase::GetDebugActionTitle_Implementation() const { return FText::FromString("DefaultAction"); }