// Copyright Epic Games, Inc. & Wiz Corporation All Rights Reserved.

#include "DebugActionsSystemTags.h"
#include "DebugActionsSystemHelpersDefine.h"
#include "GameplayTagsManager.h"
#include "DebugActionsSystemHelpers/Public/Helpers/DASHelpers.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FDebugActionsSystemTagsModule"

void FDebugActionsSystemTagsModule::StartupModule()
{
	//Add my tag ini file path in the manager, to allow unreal to found it.
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("DebugActionsSystemCore"));
	if (Plugin.IsValid() == false)
		WIZ_RET_LOG( , "Plugin not found, please check the name above", Error, LogDebugActionsSystem);
	
	FString CustomTagsDirectoryPath = Plugin->GetBaseDir() / TEXT("Config/Tags");
	UGameplayTagsManager::Get().AddTagIniSearchPath(CustomTagsDirectoryPath);
	
	WIZ_LOG(FString::Printf(TEXT("Tags folder %s loaded."), *CustomTagsDirectoryPath), Log, LogDebugActionsSystem);
}

void FDebugActionsSystemTagsModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDebugActionsSystemTagsModule, DebugActionsSystemTags)