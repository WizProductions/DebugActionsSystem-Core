// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DebugInput_LocalPlayerSelector.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"

void UDebugInput_LocalPlayerSelector::ConfigureDebugInput_Implementation() {
	Super::ConfigureDebugInput_Implementation();
	
	DebugInputTitle = FText::FromString("Player");
	DebugInputSize = FVector2D(140, 28);

	MyComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugPanel<UComboBoxString>();
	MyInputDataWidget = MyComboBox;
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG( , "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	RefreshDebugInputConfiguration();
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UDebugInput_LocalPlayerSelector::RefreshDebugInputConfiguration() {
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG( , "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	MyComboBox->ClearOptions();
	
	UWorld* World = GetWorld();
	if (World == NULL)
		WIZ_RET_LOG( , "World invalid", Error, LogDebugActionsSystem);
	
	UGameInstance* GameInstance = World->GetGameInstance();
	if (GameInstance == NULL)
		WIZ_RET_LOG( , "GameInstance invalid", Error, LogDebugActionsSystem);
	
	const TArray<ULocalPlayer*>& LocalPlayers = GameInstance->GetLocalPlayers();
	for (int i = 0; i < LocalPlayers.Num(); ++i) {
		MyComboBox->AddOption(FString::Printf(TEXT("Player_%d"), i)); 
	}
	
	MyComboBox->SetSelectedIndex(0);
}


ULocalPlayer* UDebugInput_LocalPlayerSelector::GetValue() const {
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG(NULL, "My ComboBox is invalid", Error, LogDebugActionsSystem);
	
	UWorld* World = GetWorld();
	if (World == NULL)
		WIZ_RET_LOG(NULL, "World invalid", Error, LogDebugActionsSystem);
	
	UGameInstance* GameInstance = World->GetGameInstance();
	if (GameInstance == NULL)
		WIZ_RET_LOG(NULL, "GameInstance invalid", Error, LogDebugActionsSystem);
	
	const TArray<ULocalPlayer*>& LocalPlayers = GameInstance->GetLocalPlayers();
	int32 SelectedIndex = MyComboBox->GetSelectedIndex();
	
	if (LocalPlayers.IsValidIndex(SelectedIndex) == false)
		WIZ_RET_LOG(NULL, FString::Printf(TEXT("Index %d out of bounds"), SelectedIndex), Error, LogDebugActionsSystem);
	
	return LocalPlayers[SelectedIndex];
}
