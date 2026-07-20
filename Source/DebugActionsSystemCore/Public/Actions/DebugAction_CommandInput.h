// Copyright Wiz Corporation. All Rights Reserved.

#pragma once

#include "DebugAction_CommandBase.h"
#include "DebugAction_CommandInput.generated.h"


class UDebugInput_StringETB;

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
 * An action allows user to execute specific command by a string input.
 */
UCLASS(BlueprintType, EditInlineNew)
class DEBUGACTIONSSYSTEMCORE_API UDebugAction_CommandInput : public UDebugAction_CommandBase {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################
	
protected:
	//==== Properties ====\\.
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<UDebugInput_StringETB> MyStringInput = NULL; 
	
//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################
	
public:
	FText GetDebugActionTitle_Implementation() const override { return FText::FromString("CommandInput"); }
	void OnParentFolderIsDeveloped_Implementation(UDebugActionFolder* ParentFolder) override;
	
	void GetCommand_Implementation(FString& Cmd) override;
	void GetMessage_Implementation(ULocalPlayer* Executer, FString& Message) override;
};