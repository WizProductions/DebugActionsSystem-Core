// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugActionBase.h"
#include "DebugAction_FreeCam.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
 * A simple debug action that enables FreeCam mode with a simple click.
 */
UCLASS(BlueprintType, EditInlineNew, meta=(ToolTip="Toggles freecam mode for selected player"))
class DEBUGACTIONSSYSTEMCORE_API UDebugAction_FreeCam : public UDebugActionBase {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

protected:
	//==== References ====\\.
	UPROPERTY(BlueprintReadWrite, Category = "References")
	class UDI_LocalPlayerSelector* MyPlayerSelector = NULL; 

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
public:
	virtual FText GetDebugActionTitle() const override { return FText::FromString(FString("FreeCam")); }
	virtual void OnParentFolderIsDeveloped(UDebugActionFolder* ParentFolder) override;
	virtual EDebugActionResult ExecuteDebugAction() override;
};