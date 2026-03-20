// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DASSettings.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/*

*/
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Debug Actions System - Core"))
class DEBUGACTIONSSYSTEMCORE_API UDASSettings : public UDeveloperSettings {
	GENERATED_BODY()

//#############################################################################
//##------------------------------- ATTRIBUTES ------------------------------##
//#############################################################################

public:
	UPROPERTY(Config, EditAnywhere, Category = "General", meta = (AllowedClasses = "/Script/DebugActionsSystem.DataAsset"))
	TSoftObjectPtr<UDataAsset> DebugActionsSystemDataAsset;

	UPROPERTY(Config, EditAnywhere, Category = "UI", meta = (AllowedClasses = "/Script/DebugActionsSystem.DebugToolsWidgetBase"))
	TSoftClassPtr<UUserWidget> DebugActionsSystemToolsWidgetClass;
	
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	virtual FName GetSectionName() const override { return FName("Debug Actions System - bCore"); }
};
