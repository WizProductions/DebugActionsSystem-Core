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
* <b>Needs a Setup</b>.
*/
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDI_EnumSelectorCB : public UDebugInput {
	GENERATED_BODY()

//##############################################################################
//##--------------------------------- FIELDS ---------------------------------##
//##############################################################################
	
	//==== Exposed Properties ====\\.
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

protected:
	virtual void PostInitProperties() override;
	
public:
	template <Enum E>
	void Setup(FText InDebugInputTitle);
	template <Enum E>
	E GetValue() const;
};

//#############################################################################
//##-------------------------- INLINE TEMPLATES -----------------------------##
//#############################################################################

template <Enum E>
void UDI_EnumSelectorCB::Setup(FText InDebugInputTitle) {
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG( , "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	if (InDebugInputTitle.IsEmpty())
		WIZ_LOG("Title is empty", Warning, LogDebugActionsSystem);
	
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
	MyDebugInputSlotWidget->SetTitle(DebugInputTitle);
}

template <Enum E>
E UDI_EnumSelectorCB::GetValue() const {
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG(static_cast<E>(0), "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	if (MyEnumPtr == NULL) {
		WIZ_RET_LOG(static_cast<E>(0), "Enum are not initialized, did you call SetupEnumSelector method before?", Error, LogDebugActionsSystem);
	}

	return static_cast<E>(MyComboBox->GetSelectedIndex());
}
