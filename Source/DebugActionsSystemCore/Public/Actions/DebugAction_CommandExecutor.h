// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DebugAction_CommandBase.h"
#include "DebugAction_CommandExecutor.generated.h"

//#############################################################################
//##--------------------------------- CLASS ---------------------------------##
//#############################################################################

/**
* An action allows user to execute specific command predefined in the DataAsset, like a shortcut or a macro.
 */
UCLASS()
class DEBUGACTIONSSYSTEMCORE_API UDebugAction_CommandExecutor : public UDebugAction_CommandBase {
	GENERATED_BODY()

//#############################################################################
//##--------------------------------- FIELDS --------------------------------##
//#############################################################################
	
	UPROPERTY(EditInstanceOnly, Category = "Settings")
	FText AliasName;
	UPROPERTY(EditInstanceOnly, Category = "Settings")
	FString Command;
	
//#############################################################################
//##-------------------------------- METHODS --------------------------------##
//#############################################################################

public:
	FText GetDebugActionTitle_Implementation() const override { return (AliasName.IsEmpty() ? FText::FromString("CommandExecutor") : AliasName); }

	void GetCommand_Implementation( FString& Cmd ) override;
	void GetMessage_Implementation( ULocalPlayer* Executer, FString& Message ) override;
};
