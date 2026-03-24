// Copyright Wiz Corporation. All Rights Reserved.

#include "Actions/DebugActionFolder.h"
#include "DebugActionsSystemCoreDefines.h"
#include "DataAssets/DebugActionsSystemDataAsset.h"
#include "Enumerations/EDebugActionResult.h"
#include "SubSystems/DebugSubsystem.h"

#if WITH_EDITOR
void UDebugActionFolder::UpdateEditorDataAssetTitle() {
	Private_DataAssetActionTitle = FString("Folder: " + GetDebugActionTitle().ToString());
}

void UDebugActionFolder::PostLoad() {
	Super::PostLoad();
	
	//Force refresh the data asset view when the folder title is loaded from the disk
	if (UObject* Outer = GetOuter()) {
		Outer->PostEditChange();
	}
}

void UDebugActionFolder::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	//Force a default value to a debug action pointer, catch every nullptr in Data Asset
	static const FName OptionsName = GET_MEMBER_NAME_CHECKED(UDebugActionFolder, DebugActionsStored);

	if (PropertyChangedEvent.Property &&
		PropertyChangedEvent.Property->GetFName() == OptionsName)
	{
		for (int32 i = 0; i < DebugActionsStored.Num(); i++)
		{
			if (DebugActionsStored[i] == nullptr)
			{
				DebugActionsStored[i] = NewObject<UDebugActionFolder>(this, UDebugActionFolder::StaticClass(), NAME_None, RF_Transactional);
			}
		}
	}
	
	//Update folder title by new value
	UpdateEditorDataAssetTitle();
	
	//Force refresh the data asset view
	if (UObject* Outer = GetOuter())
	{
		Outer->PostEditChange();
	}
}
#endif

EDebugActionResult UDebugActionFolder::InitializeDebugAction(TArray<TObjectPtr<UDebugActionBase>>& OutActions, UDebugSubsystem* Subsystem) {

	//Set properties
	Super::InitializeDebugAction(OutActions, Subsystem);
	
	OutActions = DebugActionsStored;
	return EDebugActionResult::HierarchyInitialization;
}

void UDebugActionFolder::SetDebugActionWidgetVisibility(bool bNewIsCollapsed, int DepthRecursivity) {
	
	if (DepthRecursivity <= DepthLevel) { //Change children's visibility by recursivity
		if (bNewIsCollapsed) { //Hide all children
			for (auto DebugAction : DebugActionsStored) {
				DebugAction->SetDebugActionWidgetVisibility(bNewIsCollapsed, DepthRecursivity);
			}
			if (DepthRecursivity != DepthLevel) { //If depth recursivity is my depth doesn't hide me
				Super::SetDebugActionWidgetVisibility(bNewIsCollapsed); //Change his visibility
				bDebugActionState = false;
			}
		}
		else { //Show first depth children
			for (auto DebugAction : DebugActionsStored) {
				if (DebugAction == NULL)
					WIZ_RET_LOG( , "DebugActionFolder cannot be developped because a debug action is invalid, please check data asset", Error, LogDebugActionsSystem);
				
				DebugAction->SetDebugActionWidgetVisibility(bNewIsCollapsed);
			}
		}
	}
}

EDebugActionResult UDebugActionFolder::ExecuteDebugAction() {

	if (Super::ExecuteDebugAction() == EDebugActionResult::Success) {
		//Catch the old debug action folder (is different -> hide old children's)
		MyDebugSubsystem->OnFolderStateChange(bDebugActionState, this); //@UPGRADE: to event delegates to replace GetDebugPanelWidget()?

		//Change visibility of the debug action folder clicked
		SetDebugActionWidgetVisibility(!bDebugActionState, this->GetDepthLevel());

		for (UDebugActionBase* DebugAction : DebugActionsStored) {
			if (DebugAction) {
				if (bDebugActionState) {
					DebugAction->OnParentFolderIsDeveloped(this);
				}
				else {
					DebugAction->OnParentFolderIsCollapsed(this);
				}
			}
		}
		
		return EDebugActionResult::Success;
	}
	
	return EDebugActionResult::Fail;
}

void UDebugActionFolder::RefreshChildren() {
	
	for (auto DA : DebugActionsStored) {
		DA->OnParentFolderIsDeveloped(this);
	}
}