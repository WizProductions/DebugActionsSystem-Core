// Copyright Wiz Corporation. All Rights Reserved.

#include "Actions/DebugActionFolder.h"
#include "DebugActionsSystemCoreDefines.h"
#include "DataAssets/DebugActionsSystemDataAsset.h"
#include "Enumerations/EDebugActionResult.h"
#include "SubSystems/DebugSubsystem.h"

#if WITH_EDITOR
void UDebugActionFolder::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent )
{
	//Update the property before check it
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	//Force a default value to a debug action pointer, catch every nullptr in Data Asset
	static const FName ArrayPropertyName = GET_MEMBER_NAME_CHECKED(UDebugActionFolder, DebugActionsStored);

	//On this folder or sub-action name changed or an object inside (ValueSet because it is recreated)
	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet)
	{
		RecursiveRequestDataAssetTitleUpdate();
		return;
	}
	
	else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
	{
		if (PropertyChangedEvent.Property &&
		    PropertyChangedEvent.Property->GetFName() == ArrayPropertyName)
		{
			for (int32 i = 0; i < DebugActionsStored.Num(); i++)
			{
				if (DebugActionsStored[i] == nullptr)
				{
					DebugActionsStored[i] = NewObject<UDebugActionFolder>(this, UDebugActionFolder::StaticClass(), NAME_None, RF_Transactional);
				}
			}

			int32 AddedObjectIndex = PropertyChangedEvent.GetArrayIndex(ArrayPropertyName.ToString());
			if (DebugActionsStored.IsValidIndex(AddedObjectIndex))
			{
				//Request a name update to the action added
				DebugActionsStored[AddedObjectIndex]->UpdateEditorDataAssetTitle();
			}
		}
	}
	
	else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate)
	{
		//Update DebugAction title in data asset
		int32 NewObjectIndex = PropertyChangedEvent.GetArrayIndex(ArrayPropertyName.ToString()) + 1;
		if (DebugActionsStored.IsValidIndex(NewObjectIndex))
		{
			//Request a name update to the action added
			DebugActionsStored[NewObjectIndex]->UpdateEditorDataAssetTitle();
		}
	}
}

void UDebugActionFolder::RecursiveRequestDataAssetTitleUpdate()
{
	Super::RecursiveRequestDataAssetTitleUpdate();

	for (auto& DA : DebugActionsStored)
	{
		DA->RecursiveRequestDataAssetTitleUpdate();
	}
}
#endif

void UDebugActionFolder::OpenChildren()
{
	for (auto& DA : DebugActionsStored)
	{
		DA->OnParentFolderIsDeveloped(this);
	}
}

EDebugActionResult UDebugActionFolder::InitializeDebugAction( TArray<TObjectPtr<UDebugActionBase>>& OutActions, UDebugSubsystem* Subsystem )
{
	//Set properties
	Super::InitializeDebugAction(OutActions, Subsystem);

	OutActions = DebugActionsStored;
	return EDebugActionResult::HierarchyInitialization;
}

void UDebugActionFolder::SetDebugActionWidgetVisibility( ESlateVisibility NewVisibility, int32 DepthRecursivity )
{
	if (DepthRecursivity <= DepthLevel)
	{
		//Change children's visibility by recursivity
		if (NewVisibility == ESlateVisibility::Collapsed)
		{
			//Hide all children
			for (auto DebugAction : DebugActionsStored)
			{
				DebugAction->SetDebugActionWidgetVisibility(NewVisibility, DepthRecursivity);
			}
			if (DepthRecursivity != DepthLevel)
			{
				//If depth recursivity is my depth doesn't hide me
				Super::SetDebugActionWidgetVisibility(NewVisibility); //Change his visibility
				bDebugActionState = false;
			}
		}
		else
		{
			//Show first depth children
			for (auto DebugAction : DebugActionsStored)
			{
				if (DebugAction == NULL)
					WIZ_LOG("DebugActionFolder cannot be developed because a debug action is invalid, please check data asset", Warning, LogDebugActionsSystem);

				DebugAction->SetDebugActionWidgetVisibility(NewVisibility);
			}
		}
	}
}

EDebugActionResult UDebugActionFolder::OnExecuteDebugAction_Implementation()
{
	//Catch the old debug action folder (is different -> hide old children)
	MyDebugSubsystem->OnFolderStateChange(bDebugActionState, this);

	//Change visibility of the debug action folder clicked
	SetDebugActionWidgetVisibility(bDebugActionState
		                               ? ESlateVisibility::SelfHitTestInvisible
		                               : ESlateVisibility::Collapsed, this->GetDepthLevel());

	for (UDebugActionBase* DebugAction : DebugActionsStored)
	{
		if (DebugAction)
		{
			if (bDebugActionState)
			{
				DebugAction->OnParentFolderIsDeveloped(this);
			}
			else
			{
				DebugAction->OnParentFolderIsCollapsed(this);
			}
		}
	}

	return EDebugActionResult::Success;
}
