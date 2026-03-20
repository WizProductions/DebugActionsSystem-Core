// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugActionBase.h"
#include "DebugActionFolder.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/*
 * Default.
*/
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class DEBUGACTIONSSYSTEMCORE_API UDebugActionFolder : public UDebugActionBase {
	GENERATED_BODY()

//#############################################################################
//##------------------------------- ATTRIBUTES ------------------------------##
//#############################################################################

	//@Upgrade: add a specific behavior for activate?
	//- LeftClick: Entity specific execute
	//- RightClick Entity class execute
	
public:
	//==== Settings ====\\.
	UPROPERTY(EditDefaultsOnly)
	FText DebugFolderTitle = FText::FromString("DefaultFolder");
	
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UDebugActionBase>> DebugActionsStored;
	//==== Properties ====\\.

//#############################################################################
//##------------------------------- FUNCTIONS -------------------------------##
//#############################################################################

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	virtual EDebugActionResult InitializeDebugAction(TArray<TObjectPtr<UDebugActionBase>>& OutActions, UDebugSubsystem* Subsystem) override;
	virtual void SetDebugActionWidgetVisibility(bool bNewIsCollapsed, int DepthRecursivity) override;
	virtual FText GetDebugActionTitle() const override { return DebugFolderTitle; }
	virtual EDebugActionResult ExecuteDebugAction() override;
};