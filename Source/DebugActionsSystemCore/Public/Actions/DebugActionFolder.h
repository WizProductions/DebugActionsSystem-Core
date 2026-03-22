// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugActionBase.h"
#include "DebugActionFolder.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/*
 * A DebugActionFolder can store any debug actions.
*/
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class DEBUGACTIONSSYSTEMCORE_API UDebugActionFolder : public UDebugActionBase {
	GENERATED_BODY()

//#############################################################################
//##------------------------------- ATTRIBUTES ------------------------------##
//#############################################################################
	
public:
	//==== Settings ====\\.
	UPROPERTY(EditDefaultsOnly)
	FText DebugFolderTitle = FText::FromString("DefaultFolder");
	
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UDebugActionBase>> DebugActionsStored;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	virtual EDebugActionResult InitializeDebugAction(TArray<TObjectPtr<UDebugActionBase>>& OutActions, UDebugSubsystem* Subsystem) override;
	virtual void SetDebugActionWidgetVisibility(bool bNewIsCollapsed, int DepthRecursivity) override;
	virtual FText GetDebugActionTitle() const override { return DebugFolderTitle; }
	virtual EDebugActionResult ExecuteDebugAction() override;
	
	void RefreshChildren();
};