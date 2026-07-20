// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/DebugAction_CommandExecutor.h"

void UDebugAction_CommandExecutor::GetCommand_Implementation( FString& Cmd )
{
	Cmd = *Command;
}

void UDebugAction_CommandExecutor::GetMessage_Implementation( ULocalPlayer* Executer, FString& Message )
{
	Message = FString::Printf(TEXT("Command: \"%s\" executed by an Macro/Alias."), *Command);
}
