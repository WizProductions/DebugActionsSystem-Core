// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DebugActionsSystemCore : ModuleRules
{

	public DebugActionsSystemCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
				// ... add public include paths required here ...
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DeveloperSettings",
				"DebugActionsSystemHelpers",
				"DebugActionsSystemTags"
				// ... add other public dependencies that you statically link with here ...
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"InputCore",
				"UMG",
				"GameplayTags",
				//Unused but included (used in header files)
				"Slate",
				"SlateCore",
				"InteractiveToolsFramework",
				// ... add private dependencies that you statically link with here ...	
			}
		);
		
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"LevelEditor",
					"UMGEditor",
					"EditorStyle",
					"EditorFramework",
					"EditorInteractiveToolsFramework"
				}
			);
			
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"UnrealEd"
				});
		}
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}

}