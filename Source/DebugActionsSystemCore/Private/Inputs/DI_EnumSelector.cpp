// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DI_EnumSelector.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"

void UDI_EnumSelectorCB::ConfigureDebugInput_Implementation() {
	Super::ConfigureDebugInput_Implementation();
	
	DebugInputTitle = FText::FromString("Default Enum Selector");
	DebugInputSize = FVector2D(120, 28);

	//==== Combo Box (Class Filter) ====\\.
	MyComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugPanel<UComboBoxString>();
	MyInputDataWidget = MyComboBox;
}

void UDI_EnumSelectorCB::Setup(UEnum* Enum, const FText& InDebugInputTitle) {
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG( , "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	if (Enum == NULL)
		WIZ_RET_LOG( , "Enum invalid", Error, LogDebugActionsSystem);
	
	if (InDebugInputTitle.IsEmpty())
		WIZ_LOG("Title is empty", Warning, LogDebugActionsSystem);
	
	MyEnumPtr = Enum;
	
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

uint8 UDI_EnumSelectorCB::K2_GetValue() const {
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG(0, "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	if (MyEnumPtr == NULL)
		WIZ_RET_LOG(0, "Did you call UDI_EnumSelectorCB::Setup after requesting input? Enum not valid", Error, LogDebugActionsSystem);
	
	return MyComboBox->GetSelectedIndex();
}
