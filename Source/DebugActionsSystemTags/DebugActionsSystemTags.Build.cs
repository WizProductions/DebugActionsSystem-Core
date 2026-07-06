using UnrealBuildTool;

public class DebugActionsSystemTags : ModuleRules
{
    public DebugActionsSystemTags(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
	        "Core",
	        "GameplayTags"
        });
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
	            "CoreUObject",
	            "Engine",
	            "DebugActionsSystemHelpers",
	            "Engine",
	            "Projects"
            }
        );
    }
}