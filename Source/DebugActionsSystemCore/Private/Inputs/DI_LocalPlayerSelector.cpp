// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DI_LocalPlayerSelector.h"
#include "Components/ComboBoxString.h"
#include "SubSystems/DebugSubsystem.h"

void UDI_LocalPlayerSelector::PostInitProperties() {
	Super::PostInitProperties();

	//In game only
	if (!GetWorld())
		return;

	DebugInputTitle = FText::FromString("Player");
	DebugInputSize = FVector2D(140, 28);

	MyComboBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugTools<UComboBoxString>();
	MyWidget = MyComboBox;
	
	RefreshValues();
}

ULocalPlayer* UDI_LocalPlayerSelector::GetValue() const {
	
	if (MyComboBox == NULL)
		WIZ_RET_LOG(NULL, "ComboBox is null, cannot get value, please check if enough slots are available.", Error, LogDebugActionsSystem);
	
	int32 SelectedIndex = MyComboBox->GetSelectedIndex();
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			auto LocalPlayers = GameInstance->GetLocalPlayers();
			
			if (LocalPlayers.IsValidIndex(SelectedIndex) == false)
				WIZ_RET_LOG(NULL, FString::Printf(TEXT("Index %d out of bounds"), SelectedIndex), Error, LogDebugActionsSystem);
			
			return LocalPlayers[SelectedIndex];
		}
	}
	
	WIZ_RET_LOG(NULL, "World invalid", Error, LogDebugActionsSystem);
}

void UDI_LocalPlayerSelector::RefreshValues() {
	Super::RefreshValues();
	
	MyComboBox->ClearOptions();
	
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			int PlayersID = 0;
			for (ULocalPlayer* LocalPlayer : GameInstance->GetLocalPlayers()) {
				MyComboBox->AddOption(FString::Printf(TEXT("Player_%d"), PlayersID)); 
				++PlayersID;
			}
		}
	}
	
	MyComboBox->SetSelectedIndex(0);
}
