// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugSubsystem.h"
#include "WidgetBases/DebugToolsWidgetBase.h"

template <typename T> requires std::is_base_of_v<UDebugInput, T>
T* UDebugSubsystem::RequestDebugInput(const FGameplayTag& SharedKeyTag) {

	T* OutDI = NULL;
	
	//Check if DI is already registered on the Shared DI map
	if (SharedKeyTag.MatchesTagExact(DAS_SharedDIKey_UnShared) == false) {
		
		//Construct the key
		TSubclassOf<UDebugInput> DIClass = T::StaticClass();
		FSharedDIMapKey SharedDIKey(SharedKeyTag, DIClass);
		
		//Is found a map associated of tag?
		if (TObjectPtr<UDebugInput>* DIFound = SharedDebugInputs.Find(SharedDIKey)) {
			//Is the pointer wrapper valid?
			if (DIFound && *DIFound) {
				OutDI = Cast<T, UDebugInput>(*DIFound);
				return OutDI;
			}
		}
	}
	
	//Check if DI is free
	if (auto Line = FreeDebugsInputs.Find(T::StaticClass())) {	//Find return a pointer to value (not a ref)
		for (auto It = Line->FreeDebugLine.CreateIterator(); It; ++It) {
			if (UDebugInput* ExistingDI = *It) {

				//Remove from the free DI array
				Line->FreeDebugLine.Remove(It.GetId());

				//Set used to debug input recovered
				Internal_SetUsedDI(ExistingDI, SharedKeyTag);
				
				OutDI = Cast<T>(ExistingDI);
				break;
			}
		}
	} 

	//DI is still nullptr, no DI free -> create new one
	if (OutDI == nullptr) {
		OutDI = Internal_RegisterNewDI<T>();
		Internal_SetUsedDI(OutDI, SharedKeyTag);
	}
	

	//Assign a slot to DI
	if (MyDebugToolsWidget->AssignSlotToDebugInput(OutDI)) {
		return OutDI;
	}

	WIZ_RET_LOG(NULL, "Request aborted, no slot available. Try to add new one in DebugToolsWidget", Error, LogDebugActionsSystem);
}

template <typename T> requires std::is_base_of_v<UDebugInput, T>
T* UDebugSubsystem::Internal_RegisterNewDI() {
	
	WIZ_LOG(FString::Printf(TEXT("%s created"), *T::StaticClass()->GetName()), Log, LogDebugActionsSystem);
	return NewObject<T>(this, T::StaticClass());
}

template <typename T> requires std::is_base_of_v<UWidget, T>
T* UDebugSubsystem::GetNewWidgetInDebugTools() {
	return MyDebugToolsWidget->Internal_NewWidget<T>();
}
