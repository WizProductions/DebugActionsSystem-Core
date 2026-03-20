// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugActionsSystemCoreDefines.h"
#include "DebugActionsSystemTagsDefines.h"
#include "Inputs/DebugInput.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugSubsystem.generated.h"

//Replace the direct link to debug tools menu and DataAsset to find? Get default asset in c++? try add a new debug action in project folder (not plugin)

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

//@Upgrade: add a bool in the dataasset to enable/disable DAS
/**
 * DebugSystem, manage DebugActions and DebugInputs
 */
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################
	
private:
	bool bDebugSystemInitialized : 1 = false;
	bool bDebugSystemOpened : 1 = false;

	/* Assets */
	UPROPERTY(Category="Debug Properties", BlueprintGetter="GetDebugDataAsset")
	TObjectPtr<class UDebugActionsSystemDataAsset> MyDebugDataAsset = nullptr;
	UPROPERTY()
	TObjectPtr<class UDebugToolsWidgetBase> MyDebugToolsWidget = nullptr;
	UPROPERTY()
	TObjectPtr<class UDebugActionBase> LastFolderClicked = nullptr;

public:
	/* Debug Inputs */
	TMap<TSubclassOf<UDebugInput>, TSet<UDebugInput*>> FreeDebugsInputs;
	UPROPERTY()
	TArray<UDebugInput*> UsedDebugInputs;
	UPROPERTY()
	TMap<struct FGameplayTag, UDebugInput*> SharedDebugInputs;

//#############################################################################
//##-------------------------------- FUNCTIONS ------------------------------##
//#############################################################################
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	static UDebugSubsystem* Get(const UObject* WorldContextObject);

#pragma region Debug Actions System
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;
private:
	void OnDebugMenuKeyPressed();
public:
	UDebugToolsWidgetBase* GetDebugToolsWidget() const { return MyDebugToolsWidget; }
	UFUNCTION(Category="Debug Action", BlueprintCallable)
	UDebugActionsSystemDataAsset* GetDebugDataAsset() const { return MyDebugDataAsset; }
#pragma endregion

#pragma region Debug Actions
public:
	void OnDebugToolsWidgetVisibilityChange(bool bVisible);
	void OnFolderStateChange(bool bIsDeveloped, class UDebugActionBase* InDebugActionFolder);
#pragma endregion

#pragma region Debug Inputs
public:
	/** Request a debug input on specified slot. @param SharedID: is the id used to get the same input of other DA */
	template <typename T> requires std::is_base_of_v<UDebugInput, T>
	T* RequestDebugInput(const FGameplayTag& SharedKeyTag = DAS_SharedDIKey_UnShared);
	template <typename T> requires std::is_base_of_v<UDebugInput, T>
	T* Internal_RegisterNewDI();
	/**
	 * Create a widget T type and can be directly added to DebugTools Tree.\n
	 * <b>WARNING:</b> Do not use this method during initialization of the debug tools widget because if the tree is not fully initialized, the game crashes.
	 */
	template <typename T> requires std::is_base_of_v<UWidget, T>
	T* GetNewWidgetInDebugTools();
	
private:
	bool Internal_SetFreeDI(UDebugInput* DI, const FGameplayTag& SharedKeyTag = DAS_SharedDIKey_UnShared);
	bool Internal_SetUsedDI(UDebugInput* DI, const FGameplayTag& SharedKeyTag = DAS_SharedDIKey_UnShared);
	void Internal_FreeAllDebugInputs();
#pragma endregion
};

#if CPP
#include "SubSystems/DebugSubsystem.inl"
#endif