// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DI_ActorInstanceSelector.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"

void UDI_ActorInstanceSelector::PostInitProperties() {
	Super::PostInitProperties();

	//In game only
	if (GetWorld() == NULL)
		return;

	DebugInputTitle = FText::FromString("Default AIS Title");
	DebugInputSize = FVector2D(120, 28);

	//==== Combo Box (Class Filter) ====\\.
	MyComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugPanel<UComboBoxString>();
	if (MyComboBox == NULL)
		WIZ_RET_LOG( , "Widget is invalid", Error, LogDebugActionsSystem);
	
	MyInputDataWidget = MyComboBox;
}