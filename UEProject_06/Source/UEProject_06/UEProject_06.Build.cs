using UnrealBuildTool;

public class UEProject_06 : ModuleRules
{
	public UEProject_06(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	    
        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.Add("Core");
        PublicDependencyModuleNames.Add("CoreUObject");
        PublicDependencyModuleNames.Add("Engine");
        PublicDependencyModuleNames.Add("InputCore");
        PublicDependencyModuleNames.Add("Niagara");
        PublicDependencyModuleNames.Add("AIModule");
    }
}
