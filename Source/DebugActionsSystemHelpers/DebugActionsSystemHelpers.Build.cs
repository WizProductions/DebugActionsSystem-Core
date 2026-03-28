using UnrealBuildTool;

public class DebugActionsSystemHelpers : ModuleRules
{
    public DebugActionsSystemHelpers(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UMG"
            }
        );
        
        if (Target.bBuildEditor)
        {
	        PrivateDependencyModuleNames.Add(new string("UnrealEd"));;
        }
    }
}