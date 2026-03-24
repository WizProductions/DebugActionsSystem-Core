// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actions/DebugActionBase.h"
#include "DebugActionsSystemDataAsset.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
  * The plugin's primary data source, <b>essential</b> for its proper functioning, it allows the user to configure the Debug Actions System.
*/
UCLASS(BlueprintType)
class DEBUGACTIONSSYSTEMCORE_API UDebugActionsSystemDataAsset : public UPrimaryDataAsset {
	GENERATED_BODY()

//#############################################################################
//##----------------------------- PUBLIC FIELDS -----------------------------##
//#############################################################################
	
public:
	//==== Settings ====\\.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Setup")
	bool bEnableDebugActionsSystem = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Setup|UI", meta = (AllowedClasses = "/Script/DebugActionsSystemCore.DebugPanelWidgetBase", EditCondition = "bEnableDebugActionsSystem", EditConditionHides))
	TSoftClassPtr<UUserWidget> DebugActionsSystemPanelWidgetClass;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="UI|Settings", meta = (EditCondition = "bEnableDebugActionsSystem", EditConditionHides))
	FVector2D FirstDebugActionWidgetPos = { 100.f, 100.f };
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="UI|Settings", meta = (EditCondition = "bEnableDebugActionsSystem", EditConditionHides))
	FVector2D DebugActionsWidgetsOffset = { 300.f, 50.f };
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Keybinds", meta = (EditCondition = "bEnableDebugActionsSystem", EditConditionHides))
	// ReSharper disable once UnrealHeaderToolError //- bug, FKey not found but existing
	TArray<FKey> DASOpenMenuKeys;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Instanced, Category="Setup|ActionsRegistering", meta = (EditCondition = "bEnableDebugActionsSystem", EditConditionHides, TitleProperty = "Private_DataAssetActionTitle"))
	TArray<TObjectPtr<UDebugActionBase>> DebugActionsArray;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

#ifdef WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif	
};