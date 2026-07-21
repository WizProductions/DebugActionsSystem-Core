// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugAction_OpenLevel.h"
#include "DebugAction_SeamlessTravel.generated.h"

enum ENetMode : int;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
 * A simple debug action that seamless travel with a simple click.
 */
UCLASS(BlueprintType, EditInlineNew, meta=(ToolTip="Fire a seamless travel to selected level."))
class DEBUGACTIONSSYSTEMCORE_API UDebugAction_SeamlessTravel : public UDebugAction_OpenLevel {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################

//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
public:
	FText GetDebugActionTitle_Implementation() const override { return FText::FromString("SeamLess Travel" ); }

	bool CanExecuteCommand_Implementation(UWorld* ContextWorld) const override;
	void GetCommand_Implementation(FString& Cmd) override;
	void GetMessage_Implementation(ULocalPlayer* Executer, FString& Message) override;
};