// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actions/DebugActionBase.h"
#include "DebugActionsSystemDataAsset.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/*

*/
UCLASS(BlueprintType)
class DEBUGACTIONSSYSTEMCORE_API UDebugActionsSystemDataAsset : public UPrimaryDataAsset {
	GENERATED_BODY()

//##############################################################################
//##------------------------------- ATTRIBUTES -------------------------------##-
//##############################################################################
	
public:
	//==== Settings ====\\.
	UPROPERTY(Category="Actions", EditInstanceOnly, Instanced)
	TArray<TObjectPtr<UDebugActionBase>> DebugActionsArray;
	UPROPERTY(Category="Keybinds", EditInstanceOnly, BlueprintReadOnly)
	TArray<FKey> DASOpenMenuKeys;
	UPROPERTY(Category="UI", EditInstanceOnly, BlueprintReadOnly)
	FVector2D FirstDebugActionWidgetPos = { 100.f, 100.f };
	UPROPERTY(Category="UI", EditInstanceOnly, BlueprintReadOnly)
	FVector2D DebugActionsWidgetsOffset = { 300.f, 50.f };

//#############################################################################
//##------------------------------- FUNCTIONS -------------------------------##
//#############################################################################

public:
	UDebugActionsSystemDataAsset() {}


#ifdef WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

//#############################################################################
//##------------------------------- REFLECTED -------------------------------##
//#############################################################################
	
};