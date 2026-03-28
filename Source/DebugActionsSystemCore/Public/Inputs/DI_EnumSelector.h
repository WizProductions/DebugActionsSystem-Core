// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.h"
#include "Components/ComboBoxString.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"
#include <type_traits>
#include "DebugActionsSystemCoreDefines.h"
#include "DI_EnumSelector.generated.h"

template <typename T>
concept Enum = std::is_enum_v<T>;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* An input that allows you to select a value from a specific enum. \n
* NEEDS A SETUP.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_EnumSelectorCB : public UDebugInput {
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
	TSubclassOf<UObject> EnumValueSelector = UObject::StaticClass();
	UPROPERTY()
	UEnum* MyEnumPtr = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	virtual void ConfigureDebugInput_Implementation() override;
	
	template <Enum E>
	void Setup(const FText& InDebugInputTitle);
	template <Enum E>
	E GetValue() const;
	
	UFUNCTION(BlueprintCallable, Category = "DebugActionsSystem|Setup")
	void Setup(UEnum* Enum, const FText& InDebugInputTitle);
	
	UFUNCTION(BlueprintPure, Category = "DebugActionsSystem", meta = (DisplayName = "Get Value"))
	UPARAM(DisplayName = "Enum Value") uint8 K2_GetValue() const;
};

//#############################################################################
//##-------------------------- INLINE TEMPLATES -----------------------------##
//#############################################################################

template <Enum E>
void UDI_EnumSelectorCB::Setup(const FText& InDebugInputTitle) {
	Setup(StaticEnum<E>(), InDebugInputTitle);
}

template <Enum E>
E UDI_EnumSelectorCB::GetValue() const {
	return static_cast<E>(K2_GetValue());
}
