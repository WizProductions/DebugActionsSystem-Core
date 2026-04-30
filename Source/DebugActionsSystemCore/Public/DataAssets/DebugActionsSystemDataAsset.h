// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actions/DebugActionBase.h"
#include "InputCoreTypes.h"
#include "UObject/UnrealType.h"
#include "DebugActionsSystemDataAsset.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

  /**
  * The plugin's primary data source, it is required for its proper functioning, it allows the user to configure the Debug Actions System.
*/
UCLASS(BlueprintType)
class DEBUGACTIONSSYSTEMCORE_API UDebugActionsSystemDataAsset : public UPrimaryDataAsset {
	GENERATED_BODY()

//#############################################################################
//##----------------------------- PUBLIC FIELDS -----------------------------##
//#############################################################################
	
public:
	//==== Settings ====\\.
	/** Do you want to enable the entire debug actions system? */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Setup")
	bool bEnableDebugActionsSystem = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Setup|Classes", meta = (AllowedClasses = "/Script/DebugActionsSystemCore.DebugPanelWidgetBase", EditCondition = "bEnableDebugActionsSystem", EditConditionHides))
	TSoftClassPtr<class UUserWidget> DebugActionsSystemPanelWidgetClass;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Setup|Classes", meta = (AllowedClasses = "/Script/DebugActionsSystemCore.DebugActionWidgetBase", EditCondition = "bEnableDebugActionsSystem", EditConditionHides))
	TSoftClassPtr<UUserWidget> DebugActionWidgetClass;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Setup|Classes", meta = (AllowedClasses = "/Script/DebugActionsSystemCore.DebugInputSlotWidgetBase", EditCondition = "bEnableDebugActionsSystem", EditConditionHides))
	TSoftClassPtr<UUserWidget> DebugInputSlotWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Tags", 
		meta = (
			RequiredAssetDataTags = "RowStructure=/Script/GameplayTags.GameplayTagTableRow",
			EditCondition = "bEnableDebugActionsSystem", 
			EditConditionHides
		))
	TSoftObjectPtr<class UDataTable> DebugInputKeyTagDataTable;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="UI|Settings", meta = (EditCondition = "bEnableDebugActionsSystem", EditConditionHides))
	FVector2D FirstDebugActionWidgetPos = { 100.f, 100.f };
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="UI|Settings", meta = (EditCondition = "bEnableDebugActionsSystem", EditConditionHides))
	FVector2D DebugActionsWidgetsOffset = { 300.f, 50.f };
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="UI|Settings", meta = (EditCondition = "bEnableDebugActionsSystem", EditConditionHides))
	TMap<EDebugActionResult, FLinearColor> DebugActionWidgetExecuteColorFromResult;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Keybinds", meta = (EditCondition = "bEnableDebugActionsSystem", EditConditionHides))
	// ReSharper disable once UnrealHeaderToolError //- BUG, FKey not found but existing
	/** The keys used to open/close the debug panel. */
	TArray<FKey> DASOpenMenuKeys;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Instanced, Category="Setup|ActionsRegistering", meta = (EditCondition = "bEnableDebugActionsSystem", EditConditionHides, TitleProperty = "Private_DataAssetActionTitle"))
	TArray<TObjectPtr<UDebugActionBase>> DebugActionsArray;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif	
};