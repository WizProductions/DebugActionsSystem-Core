// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DebugInput_ActorInstanceSelector.h"

#include "EngineUtils.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

void UDebugInput_ActorInstanceSelector::ConfigureDebugInput_Implementation() {
	Super::ConfigureDebugInput_Implementation();
	
	DebugInputTitle = FText::FromString("Default AIS Title");
	DebugInputSize = FVector2D(120, 28);

	//==== Combo Box (Class Filter) ====\\.
	MyComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugPanel<UComboBoxString>();
	if (MyComboBox == NULL)
		WIZ_RET_LOG( , "Widget is invalid", Error, LogDebugActionsSystem);
	
	MyInputDataWidget = MyComboBox;
}

void UDebugInput_ActorInstanceSelector::Setup(TSubclassOf<AActor> ActorClass, const FString& InDebugInputTitle) {
	
	UWorld* World = GetWorld();
	if (World == NULL) {
		WIZ_RET_LOG( , "World is invalid.", Error, LogDebugActionsSystem);
	}
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG( , "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	if (MyComboBox.Get()->GetOptionCount() != 0) {
		MyComboBox->ClearOptions();
	}
	
	CacheActorClass = ActorClass;
	
	TActorIterator<AActor> ActorIt(World, CacheActorClass);
	for (; ActorIt; ++ActorIt) {
		
		if (IsValid(*ActorIt) == false) 
			continue;
		
		ActorsCache.Add(*ActorIt);
		
		FString OptionText = FString::Printf(TEXT("%s - %s"), *ActorIt->GetName(), *ActorIt->GetActorLocation().ToString());
		MyComboBox->AddOption(OptionText);
	}
	
	DebugInputTitle = FText::FromString(InDebugInputTitle);
	MyDebugInputSlotWidget->SetTitle(DebugInputTitle);
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UDebugInput_ActorInstanceSelector::GetValue(TSubclassOf<AActor> ActorClass, UObject*& OutObject) {
	OutObject = GetValue<AActor>();
}
