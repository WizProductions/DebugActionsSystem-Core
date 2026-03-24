// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DASSettings.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/*
* The developer settings of the plugin, allows to add a line in the Project/Engine Settings.
*/
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Debug Actions System - Core"))
class DEBUGACTIONSSYSTEMCORE_API UDASSettings : public UDeveloperSettings {
	GENERATED_BODY()

//#############################################################################
//##------------------------------- ATTRIBUTES ------------------------------##
//#############################################################################

public:
	UPROPERTY(Config, EditAnywhere, Category = "Setup", meta = (AllowedClasses = "/Script/DebugActionsSystemCore.DebugActionsSystemDataAsset"))
	TSoftObjectPtr<UDataAsset> DebugActionsSystemDataAsset;
	
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	virtual FName GetSectionName() const override { return FName("Debug Actions System - bCore"); }
};
