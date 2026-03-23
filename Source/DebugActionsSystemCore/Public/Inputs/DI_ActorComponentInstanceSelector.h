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
#include "DI_ActorComponentInstanceSelector.generated.h"

template <typename C>
concept ActorComponentType = std::is_base_of_v<UActorComponent, C>;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* Default.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_ActorComponentInstanceSelector : public UDebugInput {
	GENERATED_BODY()

//##############################################################################
//##--------------------------------- FIELDS ---------------------------------##
//##############################################################################

	//==== Delegates ====\\.
private:
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
	TArray<TObjectPtr<UActorComponent>> ActorComponentsCache;
	UPROPERTY()
	TObjectPtr<UClass> CacheActorComponentClass = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

protected:
	virtual void PostInitProperties() override;
	
public:
	template <ActorComponentType C>
	void Setup(FString InDebugInputTitle);
	template <ActorComponentType C>
	C* GetValue() const;
	
private:
	UFUNCTION()
	void HandleValueChanged(FString fs, ESelectInfo::Type t);
};

//#############################################################################
//##-------------------------- INLINE TEMPLATES -----------------------------##
//#############################################################################

template <ActorComponentType C>
void UDI_ActorComponentInstanceSelector::Setup(FString InDebugInputTitle) {
	
	UWorld* World = GetWorld();
	if (World == NULL)
		WIZ_RET_LOG( ,"World is invalid.", Error, LogDebugActionsSystem);
	
	if (MyComboBox.Get()->GetOptionCount() != 0) {
		MyComboBox->ClearOptions();
	}
	
	CacheActorComponentClass = C::StaticClass();
	
	TActorIterator<AActor> ActorIt(World);
	for (; ActorIt; ++ActorIt) {
		
		if (!IsValid(*ActorIt)) continue;
		
		TArray<C*> Components;
		ActorIt->GetComponents(CacheActorComponentClass, Components);
		
		for (int i(0); i < Components.Num(); ++i) {
			
			if (!IsValid(Components[i])) continue;
			
			ActorComponentsCache.Add(Components[i]);
			
			FString OptionText = FString::Printf(TEXT("%s - %s - %s(%d)"), *ActorIt->GetName(), *ActorIt->GetActorLocation().ToString(), *Components[i]->GetName(), i);
			MyComboBox->AddOption(OptionText);
		}
	}
	
	DebugInputTitle = FText::FromString(InDebugInputTitle);
	MyDebugInputSlot->SetTitle(DebugInputTitle);
}

template <ActorComponentType C>
C* UDI_ActorComponentInstanceSelector::GetValue() const {
	
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
