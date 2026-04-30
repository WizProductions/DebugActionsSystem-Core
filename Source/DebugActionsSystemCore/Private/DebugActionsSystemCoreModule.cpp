// Copyright Epic Games, Inc. & Wiz Corporation All Rights Reserved.

#include "DebugActionsSystemCoreModule.h"
//#include "EditorMode/DebugActionsSystemCoreEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "DebugActionsSystemModule"

void FDebugActionsSystemCoreModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	//FDebugActionsSystemEditorModeCommands::Register();
}

void FDebugActionsSystemCoreModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	//FDebugActionsSystemEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDebugActionsSystemCoreModule, DebugActionsSystemCore)