// Copyright Wiz Corporation. All Rights Reserved.

#include "Actions/DebugAction_FreeCam.h"
#include "Engine/LocalPlayer.h"
#include "Engine/DebugCameraController.h"

void UDebugAction_FreeCam::GetCommand_Implementation( FString& Cmd )
{
	Cmd = "ToggleDebugCamera";
}

void UDebugAction_FreeCam::GetMessage_Implementation( ULocalPlayer* Executer, FString& Message )
{
	bool bOnFreeCam = Executer->PlayerController.IsA(ADebugCameraController::StaticClass());
	Message         = FString::Printf(TEXT("FreeCam mode [%s] for Player_%d"), bOnFreeCam
		                                                                   ? TEXT("ON")
		                                                                   : TEXT("OFF"), Executer->GetPlatformUserIndex());
}
