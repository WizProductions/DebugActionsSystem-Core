// Copyright Wiz Corporation. All Rights Reserved.

#include "Actions/DebugAction_SeamlessTravel.h"
#include "DebugActionsSystemCoreDefines.h"
#include "DebugActionsSystemHelpersDefine.h"
#include "Helpers/DASHelpers.h"
#include "Inputs/DebugInput_ObjectSelector.h"
#include "Engine/World.h"
#include "Engine/Level.h"

bool UDebugAction_SeamlessTravel::CanExecuteCommand_Implementation( UWorld* ContextWorld ) const
{
	ENetMode WorldNetMode = ContextWorld->GetNetMode();
	if (WorldNetMode != ENetMode::NM_DedicatedServer && WorldNetMode != ENetMode::NM_ListenServer)
		DAS_RET_LOG_RED(false, WorldNetMode == ENetMode::NM_Client ? "Must be host to travel." : "Cannot travel on standalone mode, use Open level instead.", Warning);

	return true;
}
void UDebugAction_SeamlessTravel::GetCommand_Implementation( FString& Cmd )
{
	TSoftObjectPtr<ULevel> LevelSelected = MyLevelSelector->GetValue<TSoftObjectPtr<ULevel>>();
	if (LevelSelected.IsNull())
		DAS_RET_LOG_RED(, "Level not selected or invalid", Error);

	Cmd = "servertravel " + LevelSelected.GetLongPackageName();
}

void UDebugAction_SeamlessTravel::GetMessage_Implementation( ULocalPlayer* Executer, FString& Message )
{
	TSoftObjectPtr<ULevel> LevelSelected = MyLevelSelector->GetValue<TSoftObjectPtr<ULevel>>();
	if (LevelSelected.IsNull())
		DAS_RET_LOG_RED(, "Level not selected or invalid", Error);

	Message = FString::Printf(TEXT("Seamless travel to level %s."), *LevelSelected.GetLongPackageName());
}
