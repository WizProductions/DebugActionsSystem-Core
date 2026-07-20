// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugActionBase.h"
#include "DebugActionFolder.generated.h"

//#############################################################################
//##---------------------------------- CLASS --------------------------------##
//#############################################################################

/**
 * Can store any debug actions type.
**/
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class DEBUGACTIONSSYSTEMCORE_API UDebugActionFolder : public UDebugActionBase {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################
	
public:
	//==== Settings ====\\.
	UPROPERTY(EditInstanceOnly, Category = "Settings")
	FText DebugFolderTitle = FText::FromString("NewFolder");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, meta = (TitleProperty = "Private_DataAssetActionTitle"), Category = "Settings")
	TArray<TObjectPtr<UDebugActionBase>> DebugActionsStored;

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
#if WITH_EDITOR
	void PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent ) override;
	void RecursiveRequestDataAssetTitleUpdate() override;
#endif
	
	EDebugActionResult InitializeDebugAction(TArray<TObjectPtr<UDebugActionBase>>& OutActions, UDebugSubsystem* Subsystem) override;
	void SetDebugActionWidgetVisibility(ESlateVisibility NewVisibility, int32 DepthRecursivity) override;
	
	void OpenChildren();
	
	FText GetDebugActionTitle_Implementation() const override { return FText::FromString("[F] " + DebugFolderTitle.ToString()); }
	EDebugActionResult OnExecuteDebugAction_Implementation() override;
};