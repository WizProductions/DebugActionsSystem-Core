// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInputBase.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/Actor.h"
#include <type_traits>
#include "DebugActionsSystemCoreDefines.h"
#include "DebugInput_ActorComponentInstanceSelector.generated.h"

template <typename C>
concept ActorComponentType = std::is_base_of_v<UActorComponent, C>;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* An input that retrieves all ActorComponent currently present in the world and allows you to select one \n
* NEEDS A SETUP.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugInput_ActorComponentInstanceSelector : public UDebugInputBase {
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
	TArray<TObjectPtr<UActorComponent>> ActorComponentsCache;
	UPROPERTY()
	TObjectPtr<UClass> CacheActorComponentClass = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	virtual void ConfigureDebugInput_Implementation() override;

	template <ActorComponentType C>
	void Setup(const FString& InDebugInputTitle);
	template <ActorComponentType C>
	C* GetValue() const;
	
	UFUNCTION(BlueprintCallable, Category = "DebugActionsSystem|Setup")
	void Setup(TSubclassOf<UActorComponent> ActorComponentClass, const FString& InDebugInputTitle);
	UFUNCTION(BlueprintPure, Category = "DebugActionsSystem", meta = (DeterminesOutputType = "ActorComponentClass", DynamicOutputParam = "OutObject"))
	void GetValue(TSubclassOf<UActorComponent> ActorComponentClass, UObject*& OutObject);
};

//#############################################################################
//##-------------------------- INLINE TEMPLATES -----------------------------##
//#############################################################################

template <ActorComponentType C>
void UDebugInput_ActorComponentInstanceSelector::Setup(const FString& InDebugInputTitle) {
	this->Setup(C::StaticClass(), InDebugInputTitle);
}

template <ActorComponentType C>
C* UDebugInput_ActorComponentInstanceSelector::GetValue() const {
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG(NULL, "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	if (CacheActorComponentClass == NULL) {
		WIZ_RET_LOG(NULL, "Component selector not set up, do you call SetupSelector() after request debug input?", Error, LogDebugActionsSystem);
	}
	
	auto Index = MyComboBox.Get()->GetSelectedIndex();
	if (Index == INDEX_NONE) return NULL;
	
	if (ActorComponentsCache.IsValidIndex(Index)) {
		return static_cast<C*>(ActorComponentsCache[Index]);	
	}
	
	WIZ_RET_LOG(NULL, "Index of component invalid, probably delete between Setup() and ExecuteDebugAction()", Warning, LogDebugActionsSystem);
}
