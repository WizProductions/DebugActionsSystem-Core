// Copyright Wiz Corporation. All Rights Reserved.

#include "SubSystems/DebugSubsystem.h"
#include "DataAssets/DebugActionsSystemDataAsset.h"
#include "DebugActionsSystemCoreDefines.h"
#include "Inputs/DI_FloatSlider.h"
#include "WidgetBases/DebugToolsWidgetBase.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Settings/DASSettings.h"

void UDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection); //Register to the collection
	
	// - Initialization
	if (bDebugSystemInitialized) return;
}

UDebugSubsystem* UDebugSubsystem::Get(const UObject* WorldContextObject) {
	if (WorldContextObject == NULL)
		WIZ_RET_LOG(NULL, "World context invalid.", Error, LogDebugActionsSystem);
	
	UWorld* World = WorldContextObject->GetWorld();
	if (World == NULL)
		WIZ_RET_LOG(NULL, "World invalid", Error, LogDebugActionsSystem);
	
	ULocalPlayer* LP = World->GetFirstLocalPlayerFromController();
	if (LP == NULL)
		WIZ_RET_LOG(NULL, "Local Player invalid", Error, LogDebugActionsSystem);
	
	return LP->GetSubsystem<UDebugSubsystem>();
}

void UDebugSubsystem::PlayerControllerChanged(APlayerController* NewPlayerController) {
	
	Super::PlayerControllerChanged(NewPlayerController);
	
	//@ASKING: Check if the freecam DA call it every executed
	WIZ_LOG("Called", Warning, LogDebugActionsSystem);
	
	// - Clean-Up
	for (auto LinePair : FreeDebugsInputs) {
		LinePair.Value.Empty();
	}
	
	FreeDebugsInputs.Empty();
	UsedDebugInputs.Empty();
	SharedDebugInputs.Empty();
	
	const UDASSettings* DASSettings = GetDefault<UDASSettings>();
	if (DASSettings == NULL)
		WIZ_RET_LOG( , "Debug Actions System Settings not found, a critical problem occured, please contact developper.", Error, LogDebugActionsSystem);
	
	if (NewPlayerController == NULL)
		WIZ_RET_LOG( , "Player's controller is invalid! Request aborted.", Error, LogDebugActionsSystem);
	
	//Assets Loading
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	
	MyDebugDataAsset = Cast<UDebugActionsSystemDataAsset>(Streamable.LoadSynchronous(DASSettings->DebugActionsSystemDataAsset.ToSoftObjectPath()));
	UObject* DASToolsWidgetClassLoaded = Streamable.LoadSynchronous(DASSettings->DebugActionsSystemToolsWidgetClass);

	//Assets Validation
	if (MyDebugDataAsset == NULL) {
		WIZ_RET_LOG( , "DebugActionsSystemDataAsset failed to load (Invalid/Wrong inherited class).", Error, LogDebugActionsSystem);
	}

	if (DASToolsWidgetClassLoaded == NULL) {
		WIZ_RET_LOG( , "DebugToolsWidget failed to load.", Error, LogDebugActionsSystem);
	}

	//Debug Tools Widget
	if (UClass* DASToolsWidgetClass = Cast<UClass>(DASToolsWidgetClassLoaded)) {
		if (UDebugToolsWidgetBase* WidgetInstance = CreateWidget<UDebugToolsWidgetBase>(NewPlayerController, DASToolsWidgetClass)) {
			MyDebugToolsWidget = WidgetInstance;
			WidgetInstance->AddToViewport();
			WidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (MyDebugToolsWidget == NULL) {
		WIZ_RET_LOG( , "DebugToolsWidget is invalid! Request aborted.", Error, LogDebugActionsSystem);
	}
	
	MyDebugToolsWidget->GenerateDebugMenu(MyDebugDataAsset->DebugActionsArray);

	//@Upgrade: replace by EnhancedInputCompoent? with register mapping context? | Keybinding in project settings category?
	//Keybindings
	if (auto IC = NewPlayerController->InputComponent) {
		const TArray<FKey>& DASOpenMenuKeys = MyDebugDataAsset->DASOpenMenuKeys;

		for (auto Key : DASOpenMenuKeys) {
			IC->BindKey(Key, IE_Pressed, this, &UDebugSubsystem::OnDebugMenuKeyPressed);
		}
	}
}

void UDebugSubsystem::OnDebugMenuKeyPressed() {

	MyDebugToolsWidget->SetVisibility(static_cast<ESlateVisibility>(bDebugSystemOpened));
	bDebugSystemOpened = !bDebugSystemOpened;
	
	ULocalPlayer* LP = GetLocalPlayer();
	LP->PlayerController->SetShowMouseCursor(bDebugSystemOpened);
	
	WIZ_LOG(FString::Printf(TEXT("New debug tools widget state: %s"), bDebugSystemOpened ? TEXT("true") : TEXT("false")), Log, LogDebugActionsSystem);
}

void UDebugSubsystem::OnFolderStateChange(bool bIsDeveloped, UDebugActionBase* InDebugActionFolder) {

	if (InDebugActionFolder) {

		bool bIsNewFolderClicked = LastFolderClicked != InDebugActionFolder;
		//Other folder developed
		if (bIsNewFolderClicked) {
			
			//Clear all DI used array
			this->Internal_FreeAllDebugInputs();

			//Clear slots in the UI
			MyDebugToolsWidget->ClearSlotsAssigment();
			
			LastFolderClicked = InDebugActionFolder;
		}
		MyDebugToolsWidget->OnFolderStateChange(bIsDeveloped, bIsNewFolderClicked, InDebugActionFolder);
	}
}

bool UDebugSubsystem::Internal_SetFreeDI(UDebugInput* DI, const FGameplayTag& SharedKeyTag) {

	int Count;

	//UnShared
	if (SharedKeyTag.MatchesTagExact(DAS_SharedDIKey_UnShared)) {
		//Remove used
		Count = UsedDebugInputs.RemoveSwap(DI, EAllowShrinking::Yes);

		//Set free
		auto& Line = FreeDebugsInputs.FindOrAdd(DI->GetClass());
		Line.Add(DI);
	}
	else {
		//Remove used
		Count = SharedDebugInputs.Remove(SharedKeyTag);

		//Set free
		auto& Line = FreeDebugsInputs.FindOrAdd(DI->GetClass());
		Line.Add(DI);
	}

	return Count >= 1;
}

bool UDebugSubsystem::Internal_SetUsedDI(UDebugInput* DI, const FGameplayTag& SharedKeyTag) {

	//UnShared
	if (SharedKeyTag.MatchesTagExact(DAS_SharedDIKey_UnShared)) {
		int32 PreSize = UsedDebugInputs.Num();
		int32 Index = UsedDebugInputs.Add(DI);

		return PreSize == Index;
	}
	else {
		int32 PreSize = SharedDebugInputs.Num();
		SharedDebugInputs.Add(SharedKeyTag, DI);

		return PreSize == SharedDebugInputs.Num();
	}
}

void UDebugSubsystem::Internal_FreeAllDebugInputs() {

	for (auto DI : UsedDebugInputs) {
		auto& Line = FreeDebugsInputs.FindOrAdd(DI->GetClass());
		Line.Add(DI);
	}
	for (auto DI : SharedDebugInputs) {
		auto& Line = FreeDebugsInputs.FindOrAdd(DI.Value->GetClass());
		Line.Add(DI.Value);
	}

	UsedDebugInputs.Empty();
	SharedDebugInputs.Empty();
}

void UDebugSubsystem::Internal_CreateWidget() {
	
}