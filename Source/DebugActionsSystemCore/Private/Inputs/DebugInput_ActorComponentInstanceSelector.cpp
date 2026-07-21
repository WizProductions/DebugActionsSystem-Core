// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DebugInput_ActorComponentInstanceSelector.h"

#include "EngineUtils.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

UWidget* UDebugInput_ActorComponentInstanceSelector::OnConfigureDebugInput_Implementation()
{
	MySlotProperties = FDISlotProperties("Default CIS Title", FVector2D(120, 28));

	//==== Combo Box (Class Filter) ====\\.
	MyComboBox = UDebugSubsystem::Get(this)->GetNewWidgetInDebugPanel<UComboBoxString>();

	return MyComboBox;
}

void UDebugInput_ActorComponentInstanceSelector::Setup( TSubclassOf<UActorComponent> ActorComponentClass, const FText& InDebugInputTitle )
{
	ActorComponentsCache.Empty();

	UWorld* World = GetWorld();
	if (World == NULL)
		WIZ_RET_LOG(, "World is invalid.", Error, LogDebugActionsSystem);

	if (MyComboBox == NULL)
		WIZ_RET_LOG(, "My ComboBox is invalid", Error, LogDebugActionsSystem);

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
	
	MyComboBox.Get()->SetSelectedIndex(0);

	MySlotProperties.DisplayName = InDebugInputTitle;
	MyDebugInputSlotWidget->SetSlotProperties(MySlotProperties);
}

void UDebugInput_ActorComponentInstanceSelector::GetValue( TSubclassOf<UActorComponent> ActorComponentClass, UObject*& OutObject )
{
	OutObject = GetValue<UActorComponent>();
}
