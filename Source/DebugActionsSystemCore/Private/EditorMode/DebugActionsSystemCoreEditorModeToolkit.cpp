// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorMode/DebugActionsSystemCoreEditorModeToolkit.h"
#include "EditorMode/DebugActionsSystemCoreEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "DebugActionsSystemEditorModeToolkit"

FDebugActionsSystemEditorModeToolkit::FDebugActionsSystemEditorModeToolkit()
{
}

void FDebugActionsSystemEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FDebugActionsSystemEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FDebugActionsSystemEditorModeToolkit::GetToolkitFName() const
{
	return FName("DebugActionsSystemEditorMode");
}

FText FDebugActionsSystemEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "DebugActionsSystemEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
