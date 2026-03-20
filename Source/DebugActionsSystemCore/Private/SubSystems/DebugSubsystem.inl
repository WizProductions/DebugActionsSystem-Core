// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugSubsystem.h"
#include "WidgetBases/DebugToolsWidgetBase.h"

template <typename T> requires std::is_base_of_v<UDebugInput, T>
T* UDebugSubsystem::RequestDebugInput(const FGameplayTag& SharedKeyTag) {

	T* DI = NULL;
	
	//Check if DI already registered to Shared TMap
	if (SharedKeyTag.MatchesTagExact(DAS_SharedDIKey_UnShared) == false && SharedDebugInputs.Contains(SharedKeyTag)) {
		DI = Cast<T, UDebugInput>(SharedDebugInputs[SharedKeyTag]);
		return DI;
	}
	
	//Check if DI is free
	if (auto Line = FreeDebugsInputs.Find(T::StaticClass())) {	//Find return a pointer to value (not a ref)
		for (auto It = Line->CreateIterator(); It; ++It) {
			if (UDebugInput* ExistingDI = *It) {

				//Remove from the free DI array
				Line->Remove(It.GetId());

				//Set used to debug input recovered
				Internal_SetUsedDI(ExistingDI, SharedKeyTag);
				
				DI = Cast<T>(ExistingDI);
				break;
			}
		}
	} 

	//DI is still nullptr, no DI free -> create another one
	if (DI == nullptr) {
		DI = Internal_RegisterNewDI<T>();
		Internal_SetUsedDI(DI, SharedKeyTag);
	}
	

	//Assign a slot to DI
	if (MyDebugToolsWidget->AssignSlotToDebugInput(DI)) {
		return DI;
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
