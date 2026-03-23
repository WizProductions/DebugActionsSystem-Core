// Copyright Wiz Corporation. All Rights Reserved.

#include "SubSystems/DebugSubsystem.h"
#include "DataAssets/DebugActionsSystemDataAsset.h"
#include "DebugActionsSystemCoreDefines.h"
#include "Actions/DebugActionFolder.h"
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
	
	if (MyDebugToolsWidget != NULL)
		return; //Already created and still valid
	
	// - Clean-Up
	for (auto LinePair : FreeDebugsInputs) {
		LinePair.Value.Empty();
	}
	
	FreeDebugsInputs.Empty();
	UsedDebugInputs.Empty();
	SharedDebugInputs.Empty();
	
	FreeDebugsInputs.Reserve(10);
	UsedDebugInputs.Reserve(5);
	SharedDebugInputs.Reserve(20);
	
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
	
	OnDebugToolsWidgetVisibilityChange(bDebugSystemOpened);
	
	WIZ_LOG(FString::Printf(TEXT("New debug tools widget state: %s"), bDebugSystemOpened ? TEXT("true") : TEXT("false")), Log, LogDebugActionsSystem);
}

void UDebugSubsystem::OnDebugToolsWidgetVisibilityChange(bool bVisible) {
	
	//Clear all DI used array
	this->Internal_FreeAllDebugInputs();

	//Clear slots in the UI
	MyDebugToolsWidget->ClearSlotsAssigment();
	
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
	MyDebugToolsWidget->ClearSlotsAssigment();
		
	LastFolderClicked = InDebugActionFolder;
	
	//Change visibility of debug input
	MyDebugToolsWidget->OnFolderStateChange(bIsDeveloped, bIsNewFolderClicked, InDebugActionFolder);
	
	if (bIsDeveloped == true)
		return;
	
	if (FolderDepthLevel == 0) {
		OnDebugToolsWidgetVisibilityChange(true);
		return; //Do not change the visibility of debug inputs on collapse a root folder because DI for roots DA just spawned
	}
	
	//Refresh children of parent debug action folder when the child folder is collapse (request DI if needed)
	UDebugActionBase* DA = MyDebugToolsWidget.Get()->GetDebugActionByDepth(FolderDepthLevel - 1);
	UDebugActionFolder* DAF = Cast<UDebugActionFolder>(DA);
	
	if (DAF == NULL)
		WIZ_RET_LOG( , FString::Printf(TEXT("Cast failed, the DA %s on depth %d is not a folder"), *DA->GetClass()->GetName(), FolderDepthLevel), Warning, LogDebugActionsSystem);
	
	DAF->RefreshChildren();
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
		auto SharedDIMapOfKeyTag = SharedDebugInputs.Find(SharedKeyTag);
		if (SharedDIMapOfKeyTag == NULL)
			WIZ_RET_LOG(false, "Key not found", Warning, LogDebugActionsSystem);
		
		TSubclassOf<UDebugInput> DIClass = DI->GetClass();
		Count = SharedDIMapOfKeyTag->Remove(DIClass);

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
		auto& SharedDIMapOfKeyTag = SharedDebugInputs.FindOrAdd(SharedKeyTag);
		TSubclassOf<UDebugInput> DIClass = DI->GetClass();
		int32 PreSize = SharedDIMapOfKeyTag.Num();
		
		SharedDIMapOfKeyTag.Add(DIClass, DI);

		return PreSize < SharedDIMapOfKeyTag.Num();
	}
}

void UDebugSubsystem::Internal_FreeAllDebugInputs() {

	//Find in unshared using map
	for (auto& DI : UsedDebugInputs) {
		
		//Add in free map
		auto& FreeDILine = FreeDebugsInputs.FindOrAdd(DI->GetClass());
		FreeDILine.Add(DI);
	}
	
	//Find in shared using map
	for (auto& SharedDI_Line : SharedDebugInputs) {
		for (auto& DILine : SharedDI_Line.Value) {
			
			//Add in free map
			auto& FreeDILine = FreeDebugsInputs.FindOrAdd(DILine.Value.GetClass());
			FreeDILine.Add(DILine.Value);
			
			//Free line at the same time to prevent rewrite
			DILine.Value = NULL;
		}
	}

	//Free unshared using map
	UsedDebugInputs.Empty();
}