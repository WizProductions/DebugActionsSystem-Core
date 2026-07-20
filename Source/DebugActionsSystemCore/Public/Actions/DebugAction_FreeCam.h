// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugAction_CommandBase.h"
#include "DebugAction_FreeCam.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
 * A simple debug action that enables FreeCam mode with a simple click.
 */
UCLASS(BlueprintType, EditInlineNew, meta=(ToolTip="Toggles freecam mode for selected player"))
class DEBUGACTIONSSYSTEMCORE_API UDebugAction_FreeCam : public UDebugAction_CommandBase {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
public:
	FText GetDebugActionTitle_Implementation() const override { return FText::FromString("FreeCam"); }

	void GetCommand_Implementation(FString& Cmd) override;
	void GetMessage_Implementation(ULocalPlayer* Executer, FString& Message) override;
};