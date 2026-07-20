// Copyright Wiz Corporation. All Rights Reserved.

#include "Actions/DebugAction_OpenLevel.h"
#include "DebugActionsSystemHelpersDefine.h"
#include "DebugActionsSystemTagsDefines.h"
#include "Helpers/DASHelpers.h"
#include "Inputs/DebugInput_ObjectSelector.h"
#include "Inputs/DebugInput_StringETB.h"
#include "SubSystems/DebugSubsystem.h"
#include "Engine/Level.h"

UDebugAction_OpenLevel::UDebugAction_OpenLevel()
{
	bColorFeedbackOnSuccess = false;
}

void UDebugAction_OpenLevel::OnParentFolderIsDeveloped_Implementation( UDebugActionFolder* ParentFolder )
{
	Super::OnParentFolderIsDeveloped_Implementation(ParentFolder);

	MyLevelSelector = MyDebugSubsystem->RequestDebugInput<UDebugInput_ObjectSelector>(DAS_SharedDIKey_LevelSelector);
	MyLevelSelector->Setup<UWorld>(FText::FromString("Level"), SearchPathFilters);

	MyStringInput = MyDebugSubsystem->RequestDebugInput<UDebugInput_StringETB>(DAS_SharedDIKey_LevelOptions);
	MyStringInput->Setup(FText::FromString("Level Options"), FText::FromString("listen?port=7777"), false);
}

void UDebugAction_OpenLevel::GetCommand_Implementation( FString& Cmd )
{
	TSoftObjectPtr<ULevel> LevelSelected = MyLevelSelector->GetValue<TSoftObjectPtr<ULevel>>();
	if (LevelSelected.IsNull())
		DAS_RET_LOG_RED(, "Level not selected or invalid", Error);

	FString Options = MyStringInput.Get()->GetValue();

	Cmd = "open " + LevelSelected.GetLongPackageName();

	if (Options.IsEmpty() == false)
		Cmd += "?" + Options;
}

void UDebugAction_OpenLevel::GetMessage_Implementation( ULocalPlayer* Executer, FString& Message )
{
	TSoftObjectPtr<ULevel> LevelSelected = MyLevelSelector->GetValue<TSoftObjectPtr<ULevel>>();
	if (LevelSelected.IsNull())
		DAS_RET_LOG_RED(, "Level not selected or invalid", Error);

	Message += FString::Printf(TEXT("Level %s opened."), *LevelSelected.GetLongPackageName());
}