// Copyright Wiz Corporation. All Rights Reserved.

#include "Actions/DebugAction_CommandBase.h"
#include "DebugActionsSystemCoreDefines.h"
#include "Enumerations/EDebugActionResult.h"
#include "Inputs/DebugInput_LocalPlayerSelector.h"
#include "SubSystems/DebugSubsystem.h"

void UDebugAction_CommandBase::OnParentFolderIsDeveloped_Implementation( UDebugActionFolder* ParentFolder )
{
	Super::OnParentFolderIsDeveloped_Implementation(ParentFolder);

	MyPlayerSelector = MyDebugSubsystem->RequestDebugInput<UDebugInput_LocalPlayerSelector>(DAS_SharedDIKey_LocalPlayerSelector_Default);
	MyPlayerSelector->Setup(FText::FromString("Command Executor"));
}

bool UDebugAction_CommandBase::CanExecuteCommand_Implementation( UWorld* ContextWorld ) const
{
	//For derived implementation.
	return true;
}

void UDebugAction_CommandBase::GetCommand_Implementation( FString& Cmd )
{
	//For derived implementation.
}

void UDebugAction_CommandBase::GetMessage_Implementation( ULocalPlayer* Executer, FString& Message )
{
	//For derived implementation.
}

void UDebugAction_CommandBase::PreCommandExecute_Implementation( const FString& Cmd )
{
	//For derived implementation.
}

EDebugActionResult UDebugAction_CommandBase::OnExecuteDebugAction_Implementation()
{
	UWorld* World = GetWorld();
	if (World == NULL)
		DAS_RET_LOG_RED(EDebugActionResult::Fail, "World is invalid.", Error);

	if (CanExecuteCommand(World) == false)
		return EDebugActionResult::Fail;

	ULocalPlayer* LocalPlayer = MyPlayerSelector->GetValue();
	if (LocalPlayer == NULL)
		DAS_RET_LOG_RED(EDebugActionResult::Fail, "LocalPlayer is invalid.", Error);

	FString Cmd;
	GetCommand(Cmd);
	if (Cmd.IsEmpty())
		DAS_RET_LOG_RED(EDebugActionResult::Fail, "Command is empty.", Error);
	
	PreCommandExecute(Cmd);
	LocalPlayer->ConsoleCommand(Cmd);
	
	FString Message;
	GetMessage(LocalPlayer, Message);
	if (Message.IsEmpty())
		WIZ_RET_LOG(bColorFeedbackOnSuccess ? EDebugActionResult::Success : EDebugActionResult::SuccessWithoutColorFeedback;, "Message is empty.", Warning, LogDebugActionsSystem);

	DAS_LOG_YELLOW(Message, Log);
	DAS_LOG(FString::Printf(TEXT("Command executed: \"%s\""), *Cmd), Log, LogDebugActionsSystem);

	return bColorFeedbackOnSuccess ? EDebugActionResult::Success : EDebugActionResult::SuccessWithoutColorFeedback;
}
