// Copyright Wiz Corporation. All Rights Reserved.

#include "Actions/DebugAction_CommandInput.h"
#include "Inputs/DebugInput_StringETB.h"
#include "DebugActionsSystemTagsDefines.h"
#include "SubSystems/DebugSubsystem.h"

void UDebugAction_CommandInput::OnParentFolderIsDeveloped_Implementation( UDebugActionFolder* ParentFolder )
{
	Super::OnParentFolderIsDeveloped_Implementation(ParentFolder);

	MyStringInput = MyDebugSubsystem->RequestDebugInput<UDebugInput_StringETB>(DAS_SharedDIKey_StringETB_Default);
	MyStringInput->Setup(FText::FromString("Command"), FText::FromString("stat fps"), false);
}

void UDebugAction_CommandInput::GetCommand_Implementation( FString& Cmd )
{
	Cmd = MyStringInput.Get()->GetValue();
}

void UDebugAction_CommandInput::GetMessage_Implementation( ULocalPlayer* Executer, FString& Message )
{
	Message = FString::Printf(TEXT("Command: \"%s\" executed from input."), *MyStringInput.Get()->GetValue());
}