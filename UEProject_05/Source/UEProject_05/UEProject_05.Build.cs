// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UEProject_05 : ModuleRules
{
	public UEProject_05(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.Add("Core");
        PublicDependencyModuleNames.Add("CoreUObject");
        PublicDependencyModuleNames.Add("Engine");
        PublicDependencyModuleNames.Add("InputCore");
        PublicDependencyModuleNames.Add("NavigationSystem");
        PublicDependencyModuleNames.Add("AIModule");
        PublicDependencyModuleNames.Add("ProceduralMeshComponent");
    }
}
