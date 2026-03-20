// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DI_ActorComponentInstanceSelector.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"

void UDI_ActorComponentInstanceSelector::PostInitProperties() {
	Super::PostInitProperties();

	//In game only
	if (!GetWorld())
		return;

	DebugInputTitle = FText::FromString("Default CIS Title");
	DebugInputSize = FVector2D(120, 28);

	//==== Combo Box (Class Filter) ====\\.
	MyComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugTools<UComboBoxString>();
	MyWidget = MyComboBox;
	
	MyComboBox->OnSelectionChanged.AddDynamic(this, &UDI_ActorComponentInstanceSelector::HandleValueChanged);
}

void UDI_ActorComponentInstanceSelector::HandleValueChanged(FString fs, ESelectInfo::Type t) {
	OnInputValueChange.Broadcast();
}
