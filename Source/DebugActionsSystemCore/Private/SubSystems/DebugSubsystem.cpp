// Copyright Wiz Corporation. All Rights Reserved.

#include "SubSystems/DebugSubsystem.h"
#include "DataAssets/DebugActionsSystemDataAsset.h"
#include "DebugActionsSystemCoreDefines.h"
#include "Actions/DebugActionFolder.h"
#include "Inputs/DI_FloatSlider.h"
#include "WidgetBases/DebugPanelWidgetBase.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Settings/DASSettings.h"

UDebugSubsystem* UDebugSubsystem::Get(const UObject* WorldContextObject) {
	if (WorldContextObject == NULL)
		WIZ_RET_LOG(NULL, "World context object invalid.", Error, LogDebugActionsSystem);
	
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
	
	if (NewPlayerController == NULL)
		WIZ_RET_LOG( , "Player's controller is invalid! Request aborted.", Error, LogDebugActionsSystem);
	
	if (MyDebugPanelWidget != NULL)
		return; //Already created and still valid
	
	// - Clean-Up	
	FreeDebugsInputs.Empty();
	UsedDebugInputs.Empty();
	SharedDebugInputs.Empty();
	
	FreeDebugsInputs.Reserve(10);
	UsedDebugInputs.Reserve(5);
	SharedDebugInputs.Reserve(10);
	
	const UDASSettings* DASSettings = GetDefault<UDASSettings>();
	if (DASSettings == NULL)
		WIZ_RET_LOG( , "Debug Actions System Settings not found, a critical problem occured, please contact developper.", Error, LogDebugActionsSystem);
	
	//Assets Loading
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	
	MyDebugDataAsset = Cast<UDebugActionsSystemDataAsset>(Streamable.LoadSynchronous(DASSettings->DebugActionsSystemDataAsset.ToSoftObjectPath()));
	if (MyDebugDataAsset == NULL) {
		WIZ_RET_LOG( , "failed to load DebugActionsSystemDataAsset (invalid/wrong inherited class).", Error, LogDebugActionsSystem);
	}
	
	if (MyDebugDataAsset->bEnableDebugActionsSystem == false)
		WIZ_RET_LOG( , "Initialization séquence aborted, system is disabled", Log, LogDebugActionsSystem);
	
	UObject* DASPanelWidgetClassLoaded = Streamable.LoadSynchronous(MyDebugDataAsset->DebugActionsSystemPanelWidgetClass);
	if (DASPanelWidgetClassLoaded == NULL)
		WIZ_RET_LOG( , "failed to load DebugPanelWidget.", Error, LogDebugActionsSystem);

	//Debug Panel Widget
	if (UClass* DASPanelWidgetClass = Cast<UClass>(DASPanelWidgetClassLoaded)) {
		if (UDebugPanelWidgetBase* WidgetInstance = CreateWidget<UDebugPanelWidgetBase>(NewPlayerController, DASPanelWidgetClass)) {
			MyDebugPanelWidget = WidgetInstance;
			WidgetInstance->AddToViewport();
			WidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (MyDebugPanelWidget == NULL)
		WIZ_RET_LOG( , "DebugPanelWidget is invalid! Request aborted.", Error, LogDebugActionsSystem);
	
	MyDebugPanelWidget->GenerateDebugMenu(MyDebugDataAsset->DebugActionsArray);
	
	//Keybindings
	if (auto IC = NewPlayerController->InputComponent) {
		const TArray<FKey>& DASOpenMenuKeys = MyDebugDataAsset->DASOpenMenuKeys;

		for (auto Key : DASOpenMenuKeys) {
			IC->BindKey(Key, IE_Pressed, this, &UDebugSubsystem::OnDebugMenuKeyPressed);
		}
	}
}

void UDebugSubsystem::OnDebugMenuKeyPressed() {

	MyDebugPanelWidget->SetVisibility(static_cast<ESlateVisibility>(bDebugSystemOpened));
	bDebugSystemOpened = !bDebugSystemOpened;
	
	ULocalPlayer* LP = GetLocalPlayer();
	LP->PlayerController->SetShowMouseCursor(bDebugSystemOpened);
	
	OnDebugPanelWidgetVisibilityChange(bDebugSystemOpened);
	
	WIZ_LOG(FString::Printf(TEXT("New debug panel widget state: %s"), bDebugSystemOpened ? TEXT("true") : TEXT("false")), Log, LogDebugActionsSystem);
}

void UDebugSubsystem::OnDebugPanelWidgetVisibilityChange(bool bVisible) {
	
	//Clear all DI used array
	this->Internal_FreeAllDebugInputs();

	//Clear slots in the UI
	MyDebugPanelWidget->ClearSlotsAssigment();
	
	if (bVisible) {
		for (auto DebugAction : MyDebugDataAsset->DebugActionsArray) {
			DebugAction->OnParentFolderIsDeveloped(NULL);
		}
	}
}

void UDebugSubsystem::OnFolderStateChange(bool bIsDeveloped, UDebugActionBase* InDebugActionFolder) {

	if (InDebugActionFolder == NULL)
		WIZ_RET_LOG( , "DebugActionFolder invalid", Warning, LogDebugActionsSystem);
	
	bool bIsNewFolderClicked = LastFolderClicked != InDebugActionFolder;
	int FolderDepthLevel =  InDebugActionFolder->GetDepthLevel();
	
	//Clear all DI used array
	this->Internal_FreeAllDebugInputs();

	//Clear slots in the UI
	MyDebugPanelWidget->ClearSlotsAssigment();
		
	LastFolderClicked = InDebugActionFolder;
	
	//Change visibility of debug input
	MyDebugPanelWidget->OnFolderStateChange(bIsDeveloped, bIsNewFolderClicked, InDebugActionFolder);
	
	if (bIsDeveloped == true)
		return;
	
	if (FolderDepthLevel == 0) {
		OnDebugPanelWidgetVisibilityChange(true);
		return; //Do not change the visibility of debug inputs on collapse a root folder because DI for roots DA just spawned
	}
	
	//Refresh children of parent debug action folder when the child folder is collapse (request DI if needed)
	UDebugActionBase* DA = MyDebugPanelWidget.Get()->GetDebugActionByDepth(FolderDepthLevel - 1);
	UDebugActionFolder* DAF = Cast<UDebugActionFolder>(DA);
	
	if (DAF == NULL)
		WIZ_RET_LOG( , FString::Printf(TEXT("Cast failed, the DA %s on depth %d is not a folder"), *DA->GetClass()->GetName(), FolderDepthLevel), Warning, LogDebugActionsSystem);
	
	DAF->RefreshChildren();
}

bool UDebugSubsystem::Internal_SetFreeDI(UDebugInput* DI, const FGameplayTag& SharedKeyTag) {
	
	//UnShared
	if (SharedKeyTag.MatchesTagExact(DAS_SharedDIKey_UnShared)) {
		
		//Remove used
		int Count = UsedDebugInputs.RemoveSwap(DI, EAllowShrinking::Yes);

		//Set free
		auto& Line = FreeDebugsInputs.FindOrAdd(DI->GetClass());
		Line.FreeDebugLine.Add(DI);
		
		return Count >= 1;
	}
	else {
		//Construct key
		TSubclassOf<UDebugInput> DIClass = DI->GetClass();
		FSharedDIMapKey SharedDIKey(SharedKeyTag, DIClass);
		
		TObjectPtr<UDebugInput>* DIFound = SharedDebugInputs.Find(SharedDIKey);
		if (DIFound == NULL)
			WIZ_RET_LOG(false, "Shared DI not found", Warning, LogDebugActionsSystem);
		
		//Free used
		DIFound = NULL;

		//Set free
		auto& Line = FreeDebugsInputs.FindOrAdd(DI->GetClass());
		Line.FreeDebugLine.Add(DI);
		
		return true;
	}
}

bool UDebugSubsystem::Internal_SetUsedDI(UDebugInput* DI, const FGameplayTag& SharedKeyTag) {

	//UnShared
	if (SharedKeyTag.MatchesTagExact(DAS_SharedDIKey_UnShared)) {
		int32 PreSize = UsedDebugInputs.Num();
		int32 Index = UsedDebugInputs.Add(DI);

		return PreSize == Index;
	}
	else {
		//Construct key
		TSubclassOf<UDebugInput> DIClass = DI->GetClass();
		FSharedDIMapKey SharedDIKey(SharedKeyTag, DIClass);

		int32 PreSize = SharedDebugInputs.Num();
		SharedDebugInputs.Add(SharedDIKey, DI);
		return PreSize < SharedDebugInputs.Num();
	}
}

void UDebugSubsystem::Internal_FreeAllDebugInputs() {

	//Find in unshared using map
	for (auto& DI : UsedDebugInputs) {
		
		//Add in free map
		auto& FreeDILine = FreeDebugsInputs.FindOrAdd(DI->GetClass());
		FreeDILine.FreeDebugLine.Add(DI);
	}
	
	//Find in shared using map
	for (auto& DI : SharedDebugInputs) {
		
		if (DI.Value == NULL)
			continue;
			
		//Add in free map
		auto& FreeDILine = FreeDebugsInputs.FindOrAdd(DI.Value.GetClass());
		FreeDILine.FreeDebugLine.Add(DI.Value);
		
		//Free line at the same time to prevent rewrite
		DI.Value = NULL;
	}

	//Free unshared using map
	UsedDebugInputs.Empty();
}