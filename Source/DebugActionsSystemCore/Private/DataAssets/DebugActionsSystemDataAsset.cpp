// Copyright Wiz Corporation. All Rights Reserved.

#include "DataAssets/DebugActionsSystemDataAsset.h"

#include "Actions/DebugAction_FreeCam.h"

#if WITH_EDITOR
void UDebugActionsSystemDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//Force a default value to a debug action pointer, catch every nullptr
	static const FName OptionsName = GET_MEMBER_NAME_CHECKED(UDebugActionsSystemDataAsset, DebugActionsArray);

	if (PropertyChangedEvent.Property &&
		PropertyChangedEvent.Property->GetFName() == OptionsName)
	{
		for (int32 i = 0; i < DebugActionsArray.Num(); i++)
		{
			if (DebugActionsArray[i] == nullptr)
			{
				DebugActionsArray[i] = NewObject<UDebugAction_FreeCam>(this, UDebugAction_FreeCam::StaticClass(), NAME_None, RF_Transactional);
			}
		}
	}
}
#endif