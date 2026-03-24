// Copyright Wiz Corporation. All Rights Reserved.

#include "WidgetBases/DebugPanelWidgetBase.h"
#include "Components/CanvasPanel.h"
#include "DataAssets/DebugActionsSystemDataAsset.h"
#include "DebugActionsSystemCoreDefines.h"
#include "Enumerations/EDebugActionResult.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotBase.h"

#define LOCTEXT_NAMESPACE "UMG"

void UDebugPanelWidgetBase::GenerateDebugMenu(TArray<TObjectPtr<UDebugActionBase>>& DebugActions) {

	int DebugActionIndex = 0;
    TArray<TObjectPtr<UDebugActionBase>> OutDebugActionsStored;
	
	UDebugSubsystem* Subsystem = UDebugSubsystem::Get(GetWorld());
	
    for (auto ChildDebugAction : DebugActions) {

    	if (ChildDebugAction == NULL) {
    		WIZ_LOG("A ChildDebugAction is NULL", Error, LogDebugActionsSystem);
    		continue;
    	}

    	if (AddDebugActionParentWidget(DebugActionIndex, 0, ChildDebugAction)) {
    		if (ChildDebugAction->InitializeDebugAction(OutDebugActionsStored, Subsystem) == EDebugActionResult::HierarchyInitialization) {
    			Internal_FindAndInitChildDebugActions(Subsystem, DebugActionIndex, 1, OutDebugActionsStored, ChildDebugAction);
    		}
    	}

    	DebugActionIndex++;
    }

	if (MaxDepthLevel < 0) MaxDepthLevel = 0;
	DebugActionsDepthsArray.SetNum(MaxDepthLevel+1);
}

#if WITH_EDITOR
const FText UDebugPanelWidgetBase::GetPaletteCategory() {
	return LOCTEXT("Debug Actions System", "Debug Actions System");
}
#endif

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

void UDebugPanelWidgetBase::OnFolderStateChange(bool bIsDeveloped, bool bIsNewFolderClicked, UDebugActionBase* InDebugActionFolder) {
	Internal_UpdateDebugActionsDepthLevelsArray(InDebugActionFolder);
	
	if (bIsNewFolderClicked == false) {
		SetDebugInputSlotsRegisteredVisibility(bIsDeveloped ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

UDebugActionBase* UDebugPanelWidgetBase::GetDebugActionByDepth(int Depth) const {
	if (DebugActionsDepthsArray.Num() < Depth)
		WIZ_RET_LOG(NULL, "Depth target is out of bounds.", Warning, LogDebugActionsSystem);
	
	return DebugActionsDepthsArray[Depth];
}

void UDebugPanelWidgetBase::Internal_FindAndInitChildDebugActions(UDebugSubsystem* Subsystem, int ParentDebugActionIndex, int DepthLevel, TArray<TObjectPtr<UDebugActionBase>>& ChildDebugActions, TObjectPtr<UDebugActionBase> ParentDebugAction) {
	
	//At this point, the debug action is a folder
	int DebugActionIndex = 0;
	int NextDepthLevel = DepthLevel + 1;
	if (MaxDepthLevel < DepthLevel) MaxDepthLevel = DepthLevel;
	
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

void UDebugPanelWidgetBase::RegisterDebugInputSlot(UDebugInputSlotBase* InDebugInputSlot) {

	if (InDebugInputSlot) {
		DebugInputsSlotRegistered.Add(InDebugInputSlot);
	}
}

bool UDebugPanelWidgetBase::AssignSlotToDebugInput(class UDebugInput* InDebugInput) {

	if (InDebugInput) {
		for (auto DISlot : DebugInputsSlotRegistered) {
			if (DISlot->IsUsed() == false) {

				DISlot->SetInputWidget(InDebugInput);
				DISlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				return true;
			}
		}
	}
	
	return false;
}

bool UDebugPanelWidgetBase::UnassignSlotToDebugInput(class UDebugInput* InDebugInput) {

	if (InDebugInput) {
		for (auto DISlot : DebugInputsSlotRegistered) {
			if (DISlot->IsUsed()) {

				//Why need this if? //@TODO: Try to delete this and use unassign in ClearSlotsAssigment method
				if (DISlot->GetInputWidget() == InDebugInput->GetMyWidget()) {

					//Callback to UDebugInput
					InDebugInput->OnRemovedFromSlot(DISlot);
					
					DISlot->RemoveInputWidget();
					DISlot->SetVisibility(ESlateVisibility::Collapsed);
					return true;
				}
			}
		}
	}

	return false;
}

void UDebugPanelWidgetBase::ClearSlotsAssigment() {
	for (auto DISlot : DebugInputsSlotRegistered) {
		if (DISlot->IsUsed()) {

			//Callback to UDebugInput
			//@TODO: To add this callback need to add DebugInputLinkedArray in UDebugInputSlot and register dynamically in RequestDebugInput<T>()
			//InDebugInput->OnRemovedFromSlot(DISlot);
			
			DISlot->RemoveInputWidget();
			DISlot->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UDebugPanelWidgetBase::SetDebugInputSlotsRegisteredVisibility(ESlateVisibility InVisibility) {
	for (auto DISlot : DebugInputsSlotRegistered) {
		if (DISlot->IsUsed())
			DISlot->SetVisibility(InVisibility);
	}
}

bool UDebugPanelWidgetBase::AddDebugActionChildWidget_Implementation(
	int ChildDebugActionIndex, int DepthLevel, UDebugActionBase* ChildDebugAction, UDebugActionBase* ParentDebugAction) {

	ChildDebugAction->SetDepthLevel(DepthLevel);
	return true;
}

bool UDebugPanelWidgetBase::AddDebugActionParentWidget_Implementation(int DebugActionIndex, int DepthLevel, UDebugActionBase* DebugAction) {

	DebugAction->SetDepthLevel(DepthLevel);
	return true;
}