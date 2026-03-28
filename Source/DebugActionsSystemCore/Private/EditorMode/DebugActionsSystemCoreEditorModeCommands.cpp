// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorMode/DebugActionsSystemCoreEditorModeCommands.h"
#include "EditorMode/DebugActionsSystemCoreEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "DebugActionsSystemEditorModeCommands"

FDebugActionsSystemEditorModeCommands::FDebugActionsSystemEditorModeCommands()
	: TCommands<FDebugActionsSystemEditorModeCommands>("DebugActionsSystemEditorMode",
		NSLOCTEXT("DebugActionsSystemEditorMode", "DebugActionsSystemEditorModeCommands", "DebugActionsSystem Editor Mode"),
		NAME_None,
		FAppStyle::GetAppStyleSetName())
{
}

void FDebugActionsSystemEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(SimpleTool, "Show Actor Info", "Opens message box with info about a clicked actor", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

	UI_COMMAND(InteractiveTool, "Measure Distance", "Measures distance between 2 points (click to set origin, shift-click to set end point)", EUserInterfaceActionType::ToggleButton, FInputChord());
	ToolCommands.Add(InteractiveTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FDebugActionsSystemEditorModeCommands::GetCommands()
{
	return FDebugActionsSystemEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
