// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DI_ActorComponentInstanceSelector.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"

void UDI_ActorComponentInstanceSelector::PostInitProperties() {
	Super::PostInitProperties();

	//In game only
	if (GetWorld() == NULL)
		return;

	DebugInputTitle = FText::FromString("Default CIS Title");
	DebugInputSize = FVector2D(120, 28);

	//==== Combo Box (Class Filter) ====\\.
	MyComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugPanel<UComboBoxString>();	
	MyInputDataWidget = MyComboBox;
}