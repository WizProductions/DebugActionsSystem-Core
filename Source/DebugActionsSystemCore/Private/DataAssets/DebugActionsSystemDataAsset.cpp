// Copyright Wiz Corporation. All Rights Reserved.

#include "DataAssets/DebugActionsSystemDataAsset.h"
#include "Actions/DebugActionFolder.h"

#if WITH_EDITOR
void UDebugActionsSystemDataAsset::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent )
{
	//Update the property before check it
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	//Force a default value to a debug action pointer, catch every nullptr in Data Asset
	static const FName ArrayPropertyName = GET_MEMBER_NAME_CHECKED(UDebugActionsSystemDataAsset, DebugActionsArray);

	//On root action's name changed (ValueSet because it is recreated)
	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet)
	{
		for (auto& RootAction : DebugActionsArray)
		{
			RootAction->UpdateEditorDataAssetTitle();
		}
		return;
	}

	else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
	{
		if (PropertyChangedEvent.Property &&
		    PropertyChangedEvent.Property->GetFName() == ArrayPropertyName)
		{
			for (int32 i = 0; i < DebugActionsArray.Num(); i++)
			{
				if (DebugActionsArray[i] == nullptr)
				{
					DebugActionsArray[i] = NewObject<UDebugActionFolder>(this, UDebugActionFolder::StaticClass(), NAME_None, RF_Transactional);
				}
			}

			//Update DebugAction title in data asset
			int32 AddedObjectIndex = PropertyChangedEvent.GetArrayIndex(ArrayPropertyName.ToString());
			if (DebugActionsArray.IsValidIndex(AddedObjectIndex))
			{

				//Request a name update to the action added
				DebugActionsArray[AddedObjectIndex]->UpdateEditorDataAssetTitle();
			}
		}
	}
	
	else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate)
	{
		//Update DebugAction title in data asset
		int32 NewObjectIndex = PropertyChangedEvent.GetArrayIndex(ArrayPropertyName.ToString()) + 1;
		if (DebugActionsArray.IsValidIndex(NewObjectIndex))
		{

			//Request a name update to the action added
			DebugActionsArray[NewObjectIndex]->UpdateEditorDataAssetTitle();
		}
	}
}

void UDebugActionsSystemDataAsset::PostLoad()
{
	Super::PostLoad();

	TWeakObjectPtr<ThisClass> WeakThis(this);

	//Need to wait 1 tick because we cannot call an UFUNCTION method during PostLoad and when the action title is updated, the system call GetDebugActionTitle.
	FTSTicker::GetCoreTicker().AddTicker(
	FTickerDelegate::CreateLambda([WeakThis] ( float ){
		if (WeakThis.IsValid())
		{
			//Update all titles
			for (auto& RootAction : WeakThis->DebugActionsArray)
			{
				RootAction->RecursiveRequestDataAssetTitleUpdate();
			}
		}
		return false;
	}));
}
#endif
