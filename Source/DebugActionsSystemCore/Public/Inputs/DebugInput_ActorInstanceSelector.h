// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInputBase.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/Actor.h"
#include <type_traits>
#include "DebugActionsSystemCoreDefines.h"
#include "DebugInput_ActorInstanceSelector.generated.h"

template <typename A>
concept ActorType = std::is_base_of_v<AActor, A>;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* An input that retrieves all Actor currently present in the world and allows you to select one. \n
* NEEDS A SETUP.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugInput_ActorInstanceSelector : public UDebugInputBase {
	GENERATED_BODY()

//##############################################################################
//##--------------------------------- FIELDS ---------------------------------##
//##############################################################################

	//==== References ====\\.
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UComboBoxString> MyComboBox = NULL;
	
	//==== Properties ====\\.
protected:
	UPROPERTY()
	TSubclassOf<UObject> ClassFilter = UObject::StaticClass();
	UPROPERTY()
	TArray<TObjectPtr<AActor>> ActorsCache;
	UPROPERTY()
	TObjectPtr<UClass> CacheActorClass = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	virtual void ConfigureDebugInput_Implementation() override;

	template <ActorType A>
	void Setup(const FString& InDebugInputTitle);
	template <ActorType A>
	A* GetValue() const;
	
	UFUNCTION(BlueprintCallable, Category = "DebugActionsSystem|Setup")
	void Setup(TSubclassOf<AActor> ActorClass, const FString& InDebugInputTitle);
	UFUNCTION(BlueprintPure, Category = "DebugActionsSystem", meta = (DeterminesOutputType = "ActorClass", DynamicOutputParam = "OutObject"))
	void GetValue(TSubclassOf<AActor> ActorClass, UObject*& OutObject);
};

//#############################################################################
//##-------------------------- INLINE TEMPLATES -----------------------------##
//#############################################################################

template <ActorType A>
void UDebugInput_ActorInstanceSelector::Setup(const FString& InDebugInputTitle) {
	this->Setup(A::StaticClass(), InDebugInputTitle);
}

template <ActorType A>
A* UDebugInput_ActorInstanceSelector::GetValue() const {
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG(NULL, "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	if (CacheActorClass == NULL)
		WIZ_RET_LOG(NULL, "Component selector not set up, do you call SetupSelector() after request debug input?", Error, LogDebugActionsSystem);
	
	auto Index = MyComboBox.Get()->GetSelectedIndex();
	if (ActorsCache.IsValidIndex(Index)) {
		return static_cast<A*>(ActorsCache[Index]);	
	}
	
	WIZ_RET_LOG(NULL, "Index of component invalid, probably delete between Setup() and ExecuteDebugAction()", Warning, LogDebugActionsSystem);
}
