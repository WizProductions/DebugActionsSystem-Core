// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorMode/DebugActionsSystemCoreEditorMode.h"
#include "EditorMode/DebugActionsSystemCoreEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "EditorMode/DebugActionsSystemCoreEditorModeCommands.h"
#include "Modules/ModuleManager.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/DebugActionsSystemCoreSimpleTool.h"
#include "Tools/DebugActionsSystemCoreInteractiveTool.h"

// step 2: register a ToolBuilder in FDebugActionsSystemEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "DebugActionsSystemEditorMode"

const FEditorModeID UDebugActionsSystemEditorMode::EM_DebugActionsSystemEditorModeId = TEXT("EM_DebugActionsSystemEditorMode");

FString UDebugActionsSystemEditorMode::SimpleToolName = TEXT("DebugActionsSystem_ActorInfoTool");
FString UDebugActionsSystemEditorMode::InteractiveToolName = TEXT("DebugActionsSystem_MeasureDistanceTool");


UDebugActionsSystemEditorMode::UDebugActionsSystemEditorMode()
{
	//FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	// Info = FEditorModeInfo(UDebugActionsSystemEditorMode::EM_DebugActionsSystemEditorModeId,
	// 	LOCTEXT("ModeName", "DebugActionsSystem"),
	// 	FSlateIcon(),
	// 	true);
}


UDebugActionsSystemEditorMode::~UDebugActionsSystemEditorMode()
{
}


void UDebugActionsSystemEditorMode::ActorSelectionChangeNotify()
{
}

void UDebugActionsSystemEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FDebugActionsSystemEditorModeCommands& SampleToolCommands = FDebugActionsSystemEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UDebugActionsSystemSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UDebugActionsSystemInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UDebugActionsSystemEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FDebugActionsSystemEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UDebugActionsSystemEditorMode::GetModeCommands() const
{
	return FDebugActionsSystemEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
