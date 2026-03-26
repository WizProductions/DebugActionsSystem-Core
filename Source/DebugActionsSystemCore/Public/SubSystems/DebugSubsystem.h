// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugActionsSystemTagsDefines.h"
#include "Inputs/DebugInput.h"
#include "Structs/FreeDebugInputsLine.h"
#include "Structs/SharedDIMapKey.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugSubsystem.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

//@Upgrade: add a bool in the dataasset to enable/disable DAS
/**
 * The DebugSubsystem is the core of Debug Actions System, it manages all actions, inputs and links to widgets.
 */
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

protected:
	//==== References ====\\.
	UPROPERTY(BlueprintReadOnly, BlueprintGetter="GetDebugDataAsset", Category="References")
	TObjectPtr<class UDebugActionsSystemDataAsset> MyDebugDataAsset = NULL;
	UPROPERTY(BlueprintReadOnly, Category="References")
	TObjectPtr<class UDebugPanelWidgetBase> MyDebugPanelWidget = NULL;
	UPROPERTY(BlueprintReadOnly, Category="References")
	TObjectPtr<class UDebugActionBase> LastFolderClicked = NULL;
	UPROPERTY(BlueprintReadOnly, Category="References")
	TObjectPtr<class APlayerController> MyPlayerController = NULL;

public:
	UPROPERTY()
	TMap<TSubclassOf<UDebugInput>, FFreeDebugInputsLine> FreeDebugsInputs;
	UPROPERTY()
	TArray<TObjectPtr<UDebugInput>> UsedDebugInputs;
	UPROPERTY()
	TMap<FSharedDIMapKey, TObjectPtr<UDebugInput>> SharedDebugInputs;
	
private:
	//==== Flags ====\\.
	bool bDebugSystemOpened : 1 = false;
	bool bDebugSubsSystemInitialized : 1 = false;

//#############################################################################
//##--------------------------------- METHODS -------------------------------##
//#############################################################################
	
public:
	static UDebugSubsystem* Get(const UObject* WorldContextObject);

#pragma region Debug Actions System
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;
private:
	void OnDebugMenuKeyPressed();
public:
	UFUNCTION(BlueprintCallable, Category="References")
	UDebugPanelWidgetBase* GetDebugPanelWidget() const { return MyDebugPanelWidget; }
	UFUNCTION(BlueprintCallable, Category="References")
	UDebugActionsSystemDataAsset* GetDebugDataAsset() const { return MyDebugDataAsset; }
#pragma endregion

#pragma region Debug Actions
public:
	void OnDebugPanelWidgetVisibilityChange(bool bVisible);
	void OnFolderStateChange(bool bIsDeveloped, class UDebugActionBase* InDebugActionFolder);
#pragma endregion

#pragma region Debug Inputs
public:
	/** Request a debug input on specified slot. @param SharedKeyTag: is the id used to get the same input of other DA, both action who requested the same input type with the same key get the exactly same input */
	template <typename T> requires std::is_base_of_v<UDebugInput, T>
	T* RequestDebugInput(const FGameplayTag& SharedKeyTag = DAS_SharedDIKey_Default);
	/** Request a debug input on specified slot. @param SharedKeyTag: is the id used to get the same input of other DA, both action who requested the same input type with the same key get the exactly same input */
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "DebugInputClass", DynamicOutputParam = "OutDebugInput", AutoCreateRefTerm = "SharedKeyTag"))
	bool RequestDebugInput(TSubclassOf<UDebugInput> DebugInputClass, UDebugInput*& OutDebugInput, const FGameplayTag& SharedKeyTag);

	/**
	 * Create a widget T type and directly added to Debug Panel Tree.\n
	 * <b>WARNING:</b> Do not use this method during initialization of the debug panel widget because if the tree is not fully initialized, the game crashes.
	 */
	template <typename T> requires std::is_base_of_v<UWidget, T>
	T* GetNewWidgetInDebugPanel();
	/**
	 * Create a widget T type and directly added to Debug Panel Tree.\n
	 * <b>WARNING:</b> Do not use this method during initialization of the debug panel widget because if the tree is not fully initialized, the game crashes.
	 */
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "WidgetClass", DynamicOutputParam = "OutWidget"))
	void GetNewWidgetInDebugPanel(TSubclassOf<UWidget> WidgetClass, UWidget*& OutWidget);
	
private:
	void Internal_SetupDebugActionsSystem(bool bForceSetup);
	void Internal_RegisterCallbacks();
	void Internal_UnRegisterCallbacks();
	bool Internal_ValidatePrerequisites();
	bool Internal_SetFreeDI(UDebugInput* DI, const FGameplayTag& SharedKeyTag);
	bool Internal_SetUsedDI(UDebugInput* DI, const FGameplayTag& SharedKeyTag);
	void Internal_FreeAllDebugInputs();
#pragma endregion
};

#if CPP
#include "SubSystems/DebugSubsystem.inl"
#endif