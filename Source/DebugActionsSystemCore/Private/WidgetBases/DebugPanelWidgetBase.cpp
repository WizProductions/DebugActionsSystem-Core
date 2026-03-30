// Copyright Wiz Corporation. All Rights Reserved.

#include "WidgetBases/DebugPanelWidgetBase.h"
#include "DataAssets/DebugActionsSystemDataAsset.h"
#include "DebugActionsSystemCoreDefines.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Enumerations/EDebugActionResult.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugActionWidgetBase.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

#define LOCTEXT_NAMESPACE "UMG"

#if WITH_EDITOR
const FText UDebugPanelWidgetBase::GetPaletteCategory() {
	return LOCTEXT("Debug Actions System", "Debug Actions System");
}

UDebugSubsystem* UDebugPanelWidgetBase::GetDebugSubSystemChecked() {
	if (MyDebugSubsystem == NULL)
		MyDebugSubsystem = UDebugSubsystem::Get(GetWorld());
	
	return MyDebugSubsystem;
}

#endif

bool UDebugPanelWidgetBase::AddDebugActionChildWidget_Implementation(
	int ChildDebugActionIndex, int DepthLevel, UDebugActionBase* ChildDebugAction, UDebugActionBase* ParentDebugAction) {

	ChildDebugAction->SetDepthLevel(DepthLevel);
	return true;
}

bool UDebugPanelWidgetBase::AddDebugActionParentWidget_Implementation(int DebugActionIndex, int DepthLevel, UDebugActionBase* DebugAction) {

	DebugAction->SetDepthLevel(DepthLevel);
	return true;
}

bool UDebugPanelWidgetBase::CreateDebugActionWidget_Implementation(UDebugActionBase* LinkedDebugAction, class UDebugActionWidgetBase*& NewWidget, class UCanvasPanelSlot*& NewWidgetCanvasSlot) {
	
	if (LinkedDebugAction == NULL)
		WIZ_RET_LOG(false , "Debug action invalid", Error, LogDebugActionsSystem);
	
	UDebugActionsSystemDataAsset* DAS_DA = GetDebugSubSystemChecked()->GetDebugDataAsset();
	
	UClass* DAWClass = NULL;
	if (DAS_DA->DebugActionWidgetClass.IsNull() == false)
		DAWClass = DAS_DA->DebugActionWidgetClass.LoadSynchronous();
	if (DAWClass == NULL)
		WIZ_RET_LOG(false , "Debug Action widget class failed to load.", Error, LogDebugActionsSystem);
	
	NewWidget = WidgetTree->ConstructWidget<UDebugActionWidgetBase>(DAWClass);
	NewWidgetCanvasSlot = Cast<UCanvasPanelSlot>(CP_Root.Get()->AddChild(NewWidget));
	
	if (NewWidget == NULL or NewWidgetCanvasSlot == NULL)
		WIZ_RET_LOG(false, "One of output object reference is invalid", Error, LogDebugActionsSystem);
	
	
	return NewWidget->InitDebugActionWidget(LinkedDebugAction);
}

void UDebugPanelWidgetBase::OnFolderStateChange_Implementation(bool bIsDeveloped, bool bIsNewFolderClicked, UDebugActionBase* InDebugActionFolder) {
	Internal_UpdateDebugActionsDepthLevelsArray(InDebugActionFolder);
	
	if (bIsNewFolderClicked == false) {
		SetActiveDebugInputSlotsVisibility(bIsDeveloped ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UDebugPanelWidgetBase::GenerateDebugMenu(const TArray<TObjectPtr<UDebugActionBase>>& DebugActions) {

	int DebugActionIndex = 0;
	TArray<TObjectPtr<UDebugActionBase>> OutDebugActionsStored;
	
	UDebugSubsystem* DebugSubSystem = GetDebugSubSystemChecked();
	for (auto ChildDebugAction : DebugActions) {

		if (ChildDebugAction == NULL) {
			WIZ_LOG("A ChildDebugAction is NULL", Error, LogDebugActionsSystem);
			continue;
		}

		if (AddDebugActionParentWidget(DebugActionIndex, 0, ChildDebugAction)) {
			if (ChildDebugAction->InitializeDebugAction(OutDebugActionsStored, DebugSubSystem) == EDebugActionResult::HierarchyInitialization) {
				Internal_FindAndInitChildDebugActions(DebugSubSystem, DebugActionIndex, 1, OutDebugActionsStored, ChildDebugAction);
			}
		}

		DebugActionIndex++;
	}

	if (DeepestLevel < 0) DeepestLevel = 0;
	DebugActionsDepthsArray.SetNum(DeepestLevel+1);
}

UDebugActionBase* UDebugPanelWidgetBase::GetDebugActionByDepth(int Depth) const {
	if (DebugActionsDepthsArray.Num() < Depth)
		WIZ_RET_LOG(NULL, "Depth target is out of bounds.", Warning, LogDebugActionsSystem);
	
	return DebugActionsDepthsArray[Depth];
}

void UDebugPanelWidgetBase::RegisterDebugInputSlot(UDebugInputSlotWidgetBase* InDebugInputSlot) {

	if (InDebugInputSlot == NULL)
		return;
	
	DebugInputsSlotRegistered.Add(InDebugInputSlot);
}

bool UDebugPanelWidgetBase::AssignSlotToDebugInput(class UDebugInputBase* InDebugInput) {
	
	if (InDebugInput == NULL)
		return false;
	
	for (auto DISlot : DebugInputsSlotRegistered) {
		if (DISlot->IsUsed() == false) {

			DISlot->SetInputWidget(InDebugInput);
			DISlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			return true;
		}
	}
	
	WIZ_RET_LOG(false, "No debug input slot available, please add new one in debug panel.", Error, LogDebugActionsSystem);
}

bool UDebugPanelWidgetBase::UnassignSlotToDebugInput(class UDebugInputBase* InDebugInput) {

	if (InDebugInput == NULL)
		return false;
	
	//Not assigned
	if (InDebugInput->GetMyDebugInputSlotWidget() == NULL)
		return true;
	
	InDebugInput->GetMyDebugInputSlotWidget()->RemoveInputWidget();
	return true;
}

void UDebugPanelWidgetBase::ClearSlotsAssigment() {
	for (auto DISlot : DebugInputsSlotRegistered) {
		if (DISlot->IsUsed()) {			
			DISlot->RemoveInputWidget();
		}
	}
}

void UDebugPanelWidgetBase::SetActiveDebugInputSlotsVisibility(ESlateVisibility InVisibility) {
	for (auto DISlot : DebugInputsSlotRegistered) {
		if (DISlot->IsUsed())
			DISlot->SetVisibility(InVisibility);
	}
}

void UDebugPanelWidgetBase::Internal_UpdateDebugActionsDepthLevelsArray(UDebugActionBase* InDebugActionFolder) {
	
	int DADepthLevel = InDebugActionFolder->GetDepthLevel();

	//User clicked on another debug action folder -> hide old debug action folder children's (same depth)
	if (DebugActionsDepthsArray[DADepthLevel] != InDebugActionFolder) {
		if (UDebugActionBase* OldOnDepthDebugAction = DebugActionsDepthsArray[DADepthLevel]) {
			OldOnDepthDebugAction->SetDebugActionWidgetVisibility(true, DADepthLevel);
			OldOnDepthDebugAction->SetDebugActionState(false);
		}
		DebugActionsDepthsArray[DADepthLevel] = InDebugActionFolder;
	}
}

void UDebugPanelWidgetBase::Internal_FindAndInitChildDebugActions(UDebugSubsystem* Subsystem, int ParentDebugActionIndex, int32 DepthLevel, TArray<TObjectPtr<UDebugActionBase>>& ChildDebugActions, TObjectPtr<UDebugActionBase> ParentDebugAction) {
	
	//At this point, the debug action is a folder
	int DebugActionIndex = 0;
	int NextDepthLevel = DepthLevel + 1;
	if (DeepestLevel < DepthLevel) DeepestLevel = DepthLevel;
	
	TArray<TObjectPtr<UDebugActionBase>> OutDebugActionsStored;
	
	//For every debugAction check children recursively
	for (auto ChildDebugAction : ChildDebugActions) {

		if (ChildDebugAction == nullptr) {
			WIZ_LOG("A ChildDebugAction invalid", Error, LogDebugActionsSystem);
			continue;
		}

		AddDebugActionChildWidget(DebugActionIndex, DepthLevel, ChildDebugAction, ParentDebugAction);
		
		if (ChildDebugAction->InitializeDebugAction(OutDebugActionsStored, Subsystem) == EDebugActionResult::HierarchyInitialization) {
			Internal_FindAndInitChildDebugActions(Subsystem, DebugActionIndex, NextDepthLevel, OutDebugActionsStored, ChildDebugAction);
		}

		DebugActionIndex++;
	}
}