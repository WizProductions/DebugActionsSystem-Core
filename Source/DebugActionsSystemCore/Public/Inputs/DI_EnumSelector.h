// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugInput.h"
#include "Components/ComboBoxString.h"
#include "WidgetBases/DebugInputSlotBase.h"
#include <type_traits>
#include "DebugActionsSystemCoreDefines.h"
#include "DI_EnumSelector.generated.h"

template <typename T>
concept Enum = std::is_enum_v<T>;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* Default.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_EnumSelectorCB : public UDebugInput {
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
	TSubclassOf<UObject> EnumValueSelector = UObject::StaticClass();
	UPROPERTY()
	TObjectPtr<class UComboBoxString> MyComboBox = nullptr;
	

private:
	UPROPERTY()
	UEnum* MyEnumPtr = NULL;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

protected:
	virtual void PostInitProperties() override;
	
public:
	template <Enum E>
	void Setup(FText InDebugInputTitle);
	template <Enum E>
	E GetValue() const;
	
private:
	UFUNCTION()
	void HandleValueChanged(FString fs, ESelectInfo::Type t);
};

//#############################################################################
//##-------------------------- INLINE TEMPLATES -----------------------------##
//#############################################################################

template <Enum E>
void UDI_EnumSelectorCB::Setup(FText InDebugInputTitle) {
	
	MyEnumPtr = StaticEnum<E>();
	if (!MyEnumPtr) return;
	
	if (MyComboBox->GetOptionCount() != 0) {
		MyComboBox->ClearOptions();
	}
	
	for (int32 i = 0; i < MyEnumPtr->NumEnums() - 1; ++i) {        

		FString Name = MyEnumPtr->GetNameStringByIndex(i);
		
		MyComboBox.Get()->AddOption(Name);
	}
	
	DebugInputTitle = InDebugInputTitle;
	MyDebugInputSlot->SetTitle(DebugInputTitle);
}

template <Enum E>
E UDI_EnumSelectorCB::GetValue() const {
	
	if (MyEnumPtr == NULL) {
		WIZ_RET_LOG(static_cast<E>(0), "Enum are not initialized, do you call SetupEnumSelector method before?", Error, LogDebugActionsSystem);
	}
	if (MyComboBox) {
		return static_cast<E>(MyComboBox->GetSelectedIndex());
	}
	
	return static_cast<E>(0);
}
