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
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################
	
public:
	//==== Settings ====\\.
	UPROPERTY(EditDefaultsOnly)
	FText DebugFolderTitle = FText::FromString("DefaultFolder");
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, meta = (TitleProperty = "Private_DataAssetActionTitle"))
	TArray<TObjectPtr<UDebugActionBase>> DebugActionsStored;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

#if WITH_EDITOR
	virtual void UpdateEditorDataAssetTitle() override;
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	virtual EDebugActionResult InitializeDebugAction(TArray<TObjectPtr<UDebugActionBase>>& OutActions, UDebugSubsystem* Subsystem) override;
	virtual void SetDebugActionWidgetVisibility(bool bNewIsCollapsed, int32 DepthRecursivity) override;
	
	void RefreshChildren();
	
	virtual FText GetDebugActionTitle_Implementation() const override { return FText::FromString("[F] " + DebugFolderTitle.ToString()); }
	virtual EDebugActionResult ExecuteDebugAction_Implementation() override;
};