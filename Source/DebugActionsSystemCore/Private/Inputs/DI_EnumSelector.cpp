// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DI_EnumSelector.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"

void UDI_EnumSelectorCB::PostInitProperties() {
	Super::PostInitProperties();

	//In game only
	if (!GetWorld())
		return;

	DebugInputTitle = FText::FromString("Default Enum Selector");
	DebugInputSize = FVector2D(120, 28);

	//==== Combo Box (Class Filter) ====\\.
	MyComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugTools<UComboBoxString>();
	MyWidget = MyComboBox;
	
	MyComboBox->OnSelectionChanged.AddDynamic(this, &UDI_EnumSelectorCB::HandleValueChanged);
}

void UDI_EnumSelectorCB::HandleValueChanged(FString fs, ESelectInfo::Type t) {
	OnInputValueChange.Broadcast();
}
