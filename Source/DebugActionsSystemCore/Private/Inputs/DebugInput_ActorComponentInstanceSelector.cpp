// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DebugInput_ActorComponentInstanceSelector.h"

#include "EngineUtils.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

void UDebugInput_ActorComponentInstanceSelector::ConfigureDebugInput_Implementation() {
	Super::ConfigureDebugInput_Implementation();
	
	DebugInputTitle = FText::FromString("Default CIS Title");
	DebugInputSize = FVector2D(120, 28);

	//==== Combo Box (Class Filter) ====\\.
	MyComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugPanel<UComboBoxString>();	
	MyInputDataWidget = MyComboBox;
}

void UDebugInput_ActorComponentInstanceSelector::Setup(TSubclassOf<UActorComponent> ActorComponentClass, const FString& InDebugInputTitle) {
	
	UWorld* World = GetWorld();
	if (World == NULL)
		WIZ_RET_LOG( ,"World is invalid.", Error, LogDebugActionsSystem);
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG( , "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	if (MyComboBox.Get()->GetOptionCount() != 0) {
		MyComboBox->ClearOptions();
	}
	
	CacheActorComponentClass = ActorComponentClass;
	
	TActorIterator<AActor> ActorIt(World);
	for (; ActorIt; ++ActorIt) {
		
		if (IsValid(*ActorIt) == false) 
			continue;
		
		TArray<UActorComponent*> Components;
		ActorIt->GetComponents(CacheActorComponentClass, Components);
		
		for (int i(0); i < Components.Num(); ++i) {
			
			if (IsValid(Components[i]) == false) 
				continue;
			
			ActorComponentsCache.Add(Components[i]);
			
			FString OptionText = FString::Printf(TEXT("%s - %s - %s(%d)"), *ActorIt->GetName(), *ActorIt->GetActorLocation().ToString(), *Components[i]->GetName(), i);
			MyComboBox->AddOption(OptionText);
		}
	}
	
	DebugInputTitle = FText::FromString(InDebugInputTitle);
	MyDebugInputSlotWidget->SetTitle(DebugInputTitle);
}

void UDebugInput_ActorComponentInstanceSelector::GetValue(TSubclassOf<UActorComponent> ActorComponentClass, UObject*& OutObject) {
	OutObject = GetValue<UActorComponent>();
}
