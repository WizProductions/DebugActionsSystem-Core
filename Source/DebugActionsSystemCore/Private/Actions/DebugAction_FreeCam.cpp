// Copyright Wiz Corporation. All Rights Reserved.

#include "Actions/DebugAction_FreeCam.h"
#include "DebugActionsSystemCoreDefines.h"
#include "Engine/DebugCameraController.h"
#include "Enumerations/EDebugActionResult.h"
#include "Inputs/DI_LocalPlayerSelector.h"
#include "SubSystems/DebugSubsystem.h"

void UDebugAction_FreeCam::OnParentFolderIsDeveloped_Implementation(UDebugActionFolder* ParentFolder) {
	Super::OnParentFolderIsDeveloped_Implementation(ParentFolder);
	
	MyPlayerSelector = MyDebugSubsystem->RequestDebugInput<UDI_LocalPlayerSelector>();
}

EDebugActionResult UDebugAction_FreeCam::ExecuteDebugAction_Implementation() {

	DAS_EXECUTE_ACTION_SUPER_FIRST_CHECK	
			
	ULocalPlayer* LocalPlayer = MyPlayerSelector->GetValue();
	if (LocalPlayer == NULL)
		WIZ_RET_LOG(EDebugActionResult::Fail, "LocalPlayer invalid or unselected", Error, LogDebugActionsSystem);
	
	LocalPlayer->ConsoleCommand("ToggleDebugCamera");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	
	bool bOnFreeCam = LocalPlayer->PlayerController.IsA(ADebugCameraController::StaticClass());
	DAS_WIZ_LOG_YELLOW(FString::Printf(TEXT("FreeCam mode [%s] for Player_%d"), bOnFreeCam ? TEXT("ON") : TEXT("OFF"), LocalPlayer->GetPlatformUserIndex()), Log);

	return EDebugActionResult::Success;
}
