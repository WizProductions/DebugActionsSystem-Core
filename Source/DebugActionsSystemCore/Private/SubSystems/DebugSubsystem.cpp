// Copyright Wiz Corporation. All Rights Reserved.

#include "SubSystems/DebugSubsystem.h"
#include "DataAssets/DebugActionsSystemDataAsset.h"
#include "DebugActionsSystemCoreDefines.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsSettings.h"
#include "InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "Actions/DebugActionFolder.h"
#include "Inputs/DebugInput_FloatSlider.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "WidgetBases/DebugPanelWidgetBase.h"
#include "Settings/DASSettings.h"

void UDebugSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	Super::Initialize(Collection);

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UDebugSubsystem::Internal_OnWorldChanged);
}

UDebugSubsystem* UDebugSubsystem::Get( const UObject* WorldContextObject )
{
	if (IsValid(WorldContextObject) == false)
		WIZ_RET_LOG(NULL, "World context object invalid.", Error, LogDebugActionsSystem);

	UWorld* World = WorldContextObject->GetWorld();
	if (World == NULL)
		WIZ_RET_LOG(NULL, "World invalid", Error, LogDebugActionsSystem);

	WIZ_LOG("Subsystem fallback version used. Not recommended.", Warning, LogDebugActionsSystem);

	//In this version, you get only the subsystem of the first local player, make a lot of bugs in editor/split screen, is only a fallback version, it is recommended to use another version.
	return UDebugSubsystem::Get(World->GetFirstLocalPlayerFromController());
}

UDebugSubsystem* UDebugSubsystem::Get( const APawn* WorldContextObject )
{
	if (WorldContextObject == NULL)
		WIZ_RET_LOG(NULL, "World context object invalid.", Error, LogDebugActionsSystem);

	if (WorldContextObject->IsPlayerControlled() == false)
		WIZ_RET_LOG(NULL, "World context object not controller by a player.", Error, LogDebugActionsSystem);

	return UDebugSubsystem::Get(WorldContextObject->GetController<APlayerController>());
}

UDebugSubsystem* UDebugSubsystem::Get( const UUserWidget* WorldContextObject )
{
	if (WorldContextObject == NULL)
		WIZ_RET_LOG(NULL, "World context object invalid.", Error, LogDebugActionsSystem);

	return UDebugSubsystem::Get(WorldContextObject->GetOwningLocalPlayer());
}

UDebugSubsystem* UDebugSubsystem::Get( const APlayerController* WorldContextObject )
{
	if (WorldContextObject == NULL)
		WIZ_RET_LOG(NULL, "World context object invalid.", Error, LogDebugActionsSystem);

	return UDebugSubsystem::Get(WorldContextObject->GetLocalPlayer());
}

UDebugSubsystem* UDebugSubsystem::Get( const ULocalPlayer* WorldContextObject )
{
	if (WorldContextObject == NULL)
		WIZ_RET_LOG(NULL, "World context object invalid.", Error, LogDebugActionsSystem);

	return WorldContextObject->GetSubsystem<UDebugSubsystem>();
}

UDebugSubsystem* UDebugSubsystem::Get( const UDebugInputBase* WorldContextObject )
{
	if (WorldContextObject == NULL)
		WIZ_RET_LOG(NULL, "World context object invalid.", Error, LogDebugActionsSystem);

	//Because debug input object is created with subsystem as outer, look UDebugSubsystem::RequestDebugInput()
	return Cast<UDebugSubsystem>(WorldContextObject->GetOuter());
}

void UDebugSubsystem::PlayerControllerChanged( APlayerController* NewPlayerController )
{
	Internal_UnRegisterCallbacks();
	MyPlayerController = NewPlayerController;

	if (NewPlayerController == NULL)
		WIZ_RET_LOG(, "New player's controller is invalid!", Error, LogDebugActionsSystem);

	//The only call to setup method, PlayerControllerChanged is used as Initialize more secure.
	if (bDebugSubsSystemInitialized == false)
	{
		Internal_SetupDebugActionsSystem(false);
		return;
	}

	//Wait one tick to register again key because input component is not ready when this method is called.
	Internal_RegisterCallbacks(false);

	//New
	Internal_RegisterWidget(); //Recreate widget on open level context
}

void UDebugSubsystem::OnDebugMenuKeyPressed_Implementation()
{
	if (bDebugSubsSystemInitialized == false)
		WIZ_RET_LOG(, "A problem occured while loading debug subsystem, please check logs with DebugActionsSystem category.", Error, LogDebugActionsSystem, true, FColor::Red, 10.0f);

	WIZ_LOG(FString::Printf(TEXT("New debug panel widget state: %s"), !bDebugSystemPanelOpened ? TEXT("true") : TEXT("false")), Log, LogDebugActionsSystem);

	bDebugSystemPanelOpened = !bDebugSystemPanelOpened;

	if (bDebugSystemPanelOpened)
	{
		MyDebugPanelWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MyPlayerController->SetShowMouseCursor(true);
	}
	else
	{
		MyDebugPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
		MyPlayerController->SetShowMouseCursor(false);
	}

	OnDebugPanelWidgetVisibilityChange(bDebugSystemPanelOpened);
}

void UDebugSubsystem::OnDebugPanelWidgetVisibilityChange_Implementation( bool bVisible )
{
	Internal_RefreshUI();
}

void UDebugSubsystem::OnFolderStateChange_Implementation( bool bIsDeveloped, UDebugActionFolder* InDebugActionFolder )
{
	if (InDebugActionFolder == NULL)
		WIZ_RET_LOG(, "DebugActionFolder invalid", Warning, LogDebugActionsSystem);

	bool bIsNewFolderClicked = LastFolderClicked != InDebugActionFolder;
	int32 FolderDepthLevel   = InDebugActionFolder->GetDepthLevel();

	//Clear all DI used array
	this->Internal_FreeAllDebugInputs();

	//Clear slots in the UI
	MyDebugPanelWidget->ClearSlotsAssigment();

	LastFolderClicked = InDebugActionFolder;

	//Change visibility of debug input
	MyDebugPanelWidget->OnFolderStateChange(bIsDeveloped, bIsNewFolderClicked, InDebugActionFolder);

	if (bIsDeveloped)
		return;

	if (FolderDepthLevel == 0)
	{
		Internal_DevelopFirstActions();
		return; //Stop here and do not change the visibility of debug inputs on collapse a root folder because Debug inputs for root Debug actions just spawned
	}

	//Refresh children of parent debug action folder when the child folder is collapse
	UDebugActionBase* DA = MyDebugPanelWidget.Get()->GetDebugActionByDepth(FolderDepthLevel - 1);
	if (DA == NULL)
		WIZ_RET_LOG(, FString::Printf(TEXT("Debug action on depth %d not found"), FolderDepthLevel), Warning, LogDebugActionsSystem);

	UDebugActionFolder* DAF = Cast<UDebugActionFolder>(DA);
	if (DAF == NULL)
		WIZ_RET_LOG(, FString::Printf(TEXT("Cast failed, the DA %s on depth %d is not a folder"), *DA->GetClass()->GetName(), FolderDepthLevel), Warning, LogDebugActionsSystem);

	DAF->OpenChildren();
}

bool UDebugSubsystem::RequestDebugInput( TSubclassOf<UDebugInputBase> DebugInputClass, UDebugInputBase*& OutDebugInput, const FGameplayTag& SharedKeyTag )
{
	//Check if DI is already registered on the Shared DI map
	if (SharedKeyTag.IsValid() && SharedKeyTag.MatchesTagExact(DAS_SharedDIKey_UnShared) == false)
	{

		//Construct the key
		TSubclassOf<UDebugInputBase> DIClass = DebugInputClass;
		FSharedDIMapKey SharedDIKey(SharedKeyTag, DIClass);

		//Is found a map associated of tag?
		if (TObjectPtr<UDebugInputBase>* DIFound = SharedDebugInputs.Find(SharedDIKey))
		{

			//Is the pointer wrapper valid?
			if (DIFound && *DIFound)
			{
				OutDebugInput = *DIFound;
				return true;
			}
		}
	}

	//Check if DI is free
	if (auto Line = FreeDebugsInputs.Find(DebugInputClass))
	{
		//Find return a pointer to value (not a ref)
		for (auto It = Line->FreeDebugLine.CreateIterator(); It; ++It)
		{
			if (UDebugInputBase* ExistingDI = *It)
			{

				//Remove from the free DI array
				Line->FreeDebugLine.Remove(It.GetId());

				//Set used to debug input recovered
				Internal_SetUsedDI(ExistingDI, SharedKeyTag);

				OutDebugInput = ExistingDI;
				break;
			}
		}
	}

	//DI is still nullptr, no DI free -> create new one
	if (OutDebugInput == NULL)
	{
		OutDebugInput = NewObject<UDebugInputBase>(this, DebugInputClass);
		OutDebugInput->ConfigureDebugInput();

		Internal_SetUsedDI(OutDebugInput, SharedKeyTag);

		WIZ_LOG(FString::Printf(TEXT("%s created"), *DebugInputClass->GetName()), Log, LogDebugActionsSystem);
	}

	//Assign a slot to DI
	if (MyDebugPanelWidget->AssignSlotToDebugInput(OutDebugInput))
	{
		return true;
	}

	DAS_RET_LOG_RED(false, "Request finished but no slot available. Please organize your folders or add new one in DebugPanelWidget", Error);
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UDebugSubsystem::GetNewWidgetInDebugPanel( TSubclassOf<UWidget> WidgetClass, UWidget*& OutWidget )
{
	OutWidget = MyDebugPanelWidget.Get()->Internal_NewWidget(WidgetClass);
}

void UDebugSubsystem::Internal_SetupDebugActionsSystem( bool bForceSetup )
{
	if (bDebugSubsSystemInitialized && !bForceSetup)
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
		WIZ_RET_LOG(, "Debug Actions System Settings not found, a critical problem occured, please contact developper.", Error, LogDebugActionsSystem);

	UDebugActionsSystemDataAsset* LoadedDataAsset = NULL;
	if (DASSettings->DebugActionsSystemDataAsset.IsNull() == false)
		LoadedDataAsset = Cast<UDebugActionsSystemDataAsset>(DASSettings->DebugActionsSystemDataAsset.TryLoad());
	if (LoadedDataAsset == NULL)
	{
		WIZ_RET_LOG(, "failed to load DebugActionsSystemDataAsset (invalid inherited class).", Error, LogDebugActionsSystem);
	}

	MyDebugDataAsset = DuplicateObject<UDebugActionsSystemDataAsset>(LoadedDataAsset, this, FName("Temporary_DataAsset"));

	if (MyDebugDataAsset->bEnableDebugActionsSystem == false)
		WIZ_RET_LOG(, "Initialization séquence aborted, system is disabled", Log, LogDebugActionsSystem);

	if (UGameplayTagsSettings* GameplayTagsSettings = GetMutableDefault<UGameplayTagsSettings>())
	{
		if (MyDebugDataAsset->DebugInputCustomKeyTagDataTable.IsNull() == false)
		{

			FSoftObjectPath TablePath = MyDebugDataAsset->DebugInputCustomKeyTagDataTable.ToSoftObjectPath();

			if (GameplayTagsSettings->GameplayTagTableList.Contains(TablePath) == false)
			{

				GameplayTagsSettings->GameplayTagTableList.Add(TablePath);
				GameplayTagsSettings->SaveConfig();

				WIZ_LOG(
				FString::Printf(TEXT("DebugKeyTagTable %s added to the Project GameplayTagsTableList."), *MyDebugDataAsset->DebugInputCustomKeyTagDataTable.ToSoftObjectPath().ToString()),
				Log, LogDebugActionsSystem);
			}
		}
	}

	//Validate all data from DebugDataAsset
	if (Internal_ValidatePrerequisites() == false)
		return;

	Internal_RegisterCallbacks();

	//Set init before register widget
	bDebugSubsSystemInitialized = true;

	Internal_RegisterWidget();
	WIZ_LOG("DebugSubSystem ready.", Log, LogDebugActionsSystem);
}

void UDebugSubsystem::Internal_RegisterCallbacks( bool bClearOldCallbacks )
{
	if (IsValid(MyDebugDataAsset) == false)
		WIZ_RET_LOG(, "Data asset invalid, Setup has been called?", Fatal, LogDebugActionsSystem);

	if (IsValid(MyPlayerController) == false)
		return;

	if (bClearOldCallbacks)
		Internal_UnRegisterCallbacks();

	ULocalPlayer* LP = GetLocalPlayer<>();
	if (IsValid(LP) == false)
		WIZ_RET_LOG(, "LocalPlayer is invalid.", Error, LogDebugActionsSystem);

	UEnhancedInputLocalPlayerSubsystem* EIS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP);
	if (EIS == NULL)
		WIZ_RET_LOG(, "Enhanced Input Component is invalid.", Error, LogDebugActionsSystem);

	//Need to create and setup context
	if (IsValid(DebugMappingContext) == false)
	{

		DebugMappingContext = NewObject<UInputMappingContext>(this);

		DebugInputAction            = NewObject<UInputAction>();
		DebugInputAction->ValueType = EInputActionValueType::Boolean;

		const TArray<FKey>& DASOpenMenuKeys = MyDebugDataAsset->DASOpenMenuKeys;

		for (auto& Key : DASOpenMenuKeys)
		{
			DebugMappingContext->MapKey(DebugInputAction, Key);
		}
	}

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(MyPlayerController->InputComponent))
	{

		FModifyContextOptions Options;
		Options.bIgnoreAllPressedKeysUntilRelease = false;
		EIS->AddMappingContext(DebugMappingContext, MyDebugDataAsset->MappingContextPriority, Options);

		auto& ActionBinding    = EIC->BindAction(DebugInputAction, ETriggerEvent::Started, this, &UDebugSubsystem::OnDebugMenuKeyPressed);
		DebugInputActionHandle = ActionBinding.GetHandle();
	}
}

void UDebugSubsystem::Internal_UnRegisterCallbacks()
{
	if (IsValid(MyPlayerController) == false)
		return;

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(MyPlayerController->InputComponent))
	{
		EIC->RemoveBindingByHandle(DebugInputActionHandle);
	}

	ULocalPlayer* LP = GetLocalPlayer<>();
	if (IsValid(LP) == false)
		WIZ_RET_LOG(, "LocalPlayer is invalid.", Error, LogDebugActionsSystem);

	UEnhancedInputLocalPlayerSubsystem* EIS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP);
	if (EIS == NULL)
		WIZ_RET_LOG(, "Enhanced Input Component is invalid.", Error, LogDebugActionsSystem);

	EIS->RemoveMappingContext(DebugMappingContext);
}

bool UDebugSubsystem::Internal_ValidatePrerequisites()
{
	bool bValidated = true;

	if (MyDebugDataAsset == NULL)
		WIZ_RET_LOG(false, "Data Asset invalid", Error, LogDebugActionsSystem);

	if (MyDebugDataAsset->DebugActionsSystemPanelWidgetClass.IsNull())
	{
		bValidated = false;
		WIZ_LOG("Debug Panel widget class invalid", Error, LogDebugActionsSystem);
	}


	if (MyDebugDataAsset->DebugActionWidgetClass.IsNull())
	{
		bValidated = false;
		WIZ_LOG("Debug Action widget class invalid", Error, LogDebugActionsSystem);
	}

	if (MyDebugDataAsset->DebugInputSlotWidgetClass.IsNull())
	{
		bValidated = false;
		WIZ_LOG("Debug Input Slot widget class invalid", Error, LogDebugActionsSystem);
	}

	return bValidated;
}

bool UDebugSubsystem::Internal_SetFreeDI( UDebugInputBase* DI, const FGameplayTag& SharedKeyTag )
{
	//UnShared
	if (SharedKeyTag.MatchesTagExact(DAS_SharedDIKey_UnShared))
	{

		//Remove used
		int Count = UsedDebugInputs.RemoveSwap(DI, EAllowShrinking::Yes);

		//Set free
		auto& Line = FreeDebugsInputs.FindOrAdd(DI->GetClass());
		Line.FreeDebugLine.Add(DI);

		return Count >= 1;
	}
	else
	{
		//Construct key
		TSubclassOf<UDebugInputBase> DIClass = DI->GetClass();
		FSharedDIMapKey SharedDIKey(SharedKeyTag, DIClass);

		TObjectPtr<UDebugInputBase>* DIFound = SharedDebugInputs.Find(SharedDIKey);
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

bool UDebugSubsystem::Internal_SetUsedDI( UDebugInputBase* DI, const FGameplayTag& SharedKeyTag )
{
	//UnShared
	if (SharedKeyTag.MatchesTagExact(DAS_SharedDIKey_UnShared))
	{
		int32 PreSize = UsedDebugInputs.Num();
		int32 Index   = UsedDebugInputs.Add(DI);

		return PreSize == Index;
	}
	else
	{
		//Construct key
		TSubclassOf<UDebugInputBase> DIClass = DI->GetClass();
		FSharedDIMapKey SharedDIKey(SharedKeyTag, DIClass);

		int32 PreSize = SharedDebugInputs.Num();
		SharedDebugInputs.Add(SharedDIKey, DI);
		return PreSize < SharedDebugInputs.Num();
	}
}

void UDebugSubsystem::Internal_FreeAllDebugInputs()
{
	//Find in unshared using map
	for (auto& DI : UsedDebugInputs)
	{

		//Add in free map
		auto& FreeDILine = FreeDebugsInputs.FindOrAdd(DI->GetClass());
		FreeDILine.FreeDebugLine.Add(DI);
	}

	//Find in shared using map
	for (auto& DI : SharedDebugInputs)
	{

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

void UDebugSubsystem::Internal_OnWorldChanged( UWorld* World, const UWorld::InitializationValues InitializationValues )
{
	//OnWorldChanged is only used for seamless travel, on world change by an open level the player controller is not valid directly.
	if (IsValid(MyPlayerController) == false)
		return;
	
	Internal_RegisterWidget();
}

void UDebugSubsystem::Internal_RegisterWidget()
{
	//Wait subsystem initialization
	if (!bDebugSubsSystemInitialized)
		return;

	if (MyDebugDataAsset == NULL || MyPlayerController == NULL)
		WIZ_RET_LOG(, "Cannot register widget, context not ready, please call SetupDebugActionsSystem before.", Error, LogDebugActionsSystem);

	if (IsValid(MyDebugPanelWidget))
	{
		if (IsValid(MyPlayerController))
		{
			//This result statement is for seamless travel and OpenLevel because at here the context is desync.			
			if (MyDebugPanelWidget->GetOwningPlayer() != MyPlayerController)
			{
				//Change the owner
				MyDebugPanelWidget->SetOwningPlayer(MyPlayerController);
			}
			
			if (MyDebugPanelWidget->IsInViewport() == false)
			{
				MyDebugPanelWidget->AddToViewport();
			}
			
			//Sync again
			Internal_RefreshUI();
			Internal_SetInputMode();
			bDebugSystemPanelOpened = MyDebugPanelWidget->IsVisible();
			MyPlayerController->SetShowMouseCursor(bDebugSystemPanelOpened);
			
			return;
		}
	}

	UClass* PanelWidgetClass = MyDebugDataAsset->DebugActionsSystemPanelWidgetClass.LoadSynchronous();
	if (PanelWidgetClass == NULL)
		WIZ_RET_LOG(, "failed to load DebugPanelWidget.", Error, LogDebugActionsSystem);

	//Debug Panel Widget
	MyDebugPanelWidget = CreateWidget<UDebugPanelWidgetBase>(MyPlayerController, PanelWidgetClass);
	if (MyDebugPanelWidget == NULL)
		WIZ_RET_LOG(, "DebugPanelWidget is invalid! Request aborted.", Error, LogDebugActionsSystem);

	MyDebugPanelWidget->AddToViewport();
	MyDebugPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
	MyDebugPanelWidget->GenerateDebugMenu(MyDebugDataAsset->DebugActionsArray);

	Internal_SetInputMode();

	bDebugSystemPanelOpened = false;
}

void UDebugSubsystem::Internal_DevelopFirstActions()
{
	for (auto DebugAction : MyDebugDataAsset->DebugActionsArray)
	{
		DebugAction->OnParentFolderIsDeveloped(NULL);
	}
}

void UDebugSubsystem::Internal_SetInputMode()
{
	if (IsValid(MyDebugPanelWidget) == false)
		return;

	if (IsValid(MyPlayerController) == false)
		return;

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(MyDebugPanelWidget.Get()->TakeWidget());
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);

	MyPlayerController->SetInputMode(InputMode);
}

void UDebugSubsystem::Internal_RefreshUI()
{	
	//Clear all DI used
	this->Internal_FreeAllDebugInputs();

	//Clear slots in the UI
	MyDebugPanelWidget->ClearSlotsAssigment();
	
	if (LastFolderClicked && LastFolderClicked.Get()->GetDebugActionState())
	{
		LastFolderClicked.Get()->OpenChildren();
	}
	else
	{
		//Show the first actions
		Internal_DevelopFirstActions();
	}
}
