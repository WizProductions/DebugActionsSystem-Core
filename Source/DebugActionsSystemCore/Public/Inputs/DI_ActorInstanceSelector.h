// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/Actor.h"
#include <type_traits>
#include "EngineUtils.h"
#include "DebugActionsSystemCoreDefines.h"
#include "WidgetBases/DebugInputSlotBase.h"
#include "DI_ActorInstanceSelector.generated.h"

template <typename A>
concept ActorType = std::is_base_of_v<AActor, A>;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* Default.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_ActorInstanceSelector : public UDebugInput {
	GENERATED_BODY()

//##############################################################################
//##--------------------------------- FIELDS ---------------------------------##
//##############################################################################

	//==== Delegates ====\\.
private:
	/* Delegates */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputValueChange);
	
public:
	FOnInputValueChange OnInputValueChange;
	
	//==== Properties ====\\.
protected:
	UPROPERTY()
	TSubclassOf<UObject> ClassFilter = UObject::StaticClass();
	UPROPERTY()
	TObjectPtr<class UComboBoxString> MyComboBox = nullptr;
	
private:
	TArray<TObjectPtr<AActor>> ActorsCache;
	UPROPERTY()
	TObjectPtr<UClass> CacheActorClass = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

protected:
	virtual void PostInitProperties() override;
	
public:
	template <ActorType A>
	void Setup(FString InDebugInputTitle);
	template <ActorType A>
	A* GetValue() const;
	
private:
	UFUNCTION()
	void HandleValueChanged(FString fs, ESelectInfo::Type t);
};

//#############################################################################
//##-------------------------- INLINE TEMPLATES -----------------------------##
//#############################################################################

template <ActorType A>
void UDI_ActorInstanceSelector::Setup(FString InDebugInputTitle) {
	
	UWorld* World = GetWorld();
	if (World == NULL) {
		WIZ_RET_LOG( , "World is invalid.", Error, LogDebugActionsSystem);
	}
	
	if (MyComboBox.Get()->GetOptionCount() != 0) {
		MyComboBox->ClearOptions();
	}
	
	CacheActorClass = A::StaticClass();
	
	TActorIterator<A> ActorIt(World, CacheActorClass);
	for (; ActorIt; ++ActorIt) {
		
		if (!IsValid(*ActorIt)) continue;
		
		ActorsCache.Add(*ActorIt);
		
		FString OptionText = FString::Printf(TEXT("%s - %s"), *ActorIt->GetName(), *ActorIt->GetActorLocation().ToString());
		MyComboBox->AddOption(OptionText);
	}
	
	DebugInputTitle = FText::FromString(InDebugInputTitle);
	MyDebugInputSlot->SetTitle(DebugInputTitle);
}

template <ActorType A>
A* UDI_ActorInstanceSelector::GetValue() const {
	
	if (CacheActorClass == NULL) {
		WIZ_RET_LOG(NULL, "Component selector not set up, do you call SetupSelector() after request debug input?", Error, LogDebugActionsSystem);
	}
	
	auto Index = MyComboBox.Get()->GetSelectedIndex();
	if (ActorsCache.IsValidIndex(Index)) {
		return static_cast<A*>(ActorsCache[Index]);	
	}
	
	WIZ_RET_LOG(NULL, "Index of component invalid, probably delete between Setup() and ExecuteDebugAction()", Warning, LogDebugActionsSystem);
}
