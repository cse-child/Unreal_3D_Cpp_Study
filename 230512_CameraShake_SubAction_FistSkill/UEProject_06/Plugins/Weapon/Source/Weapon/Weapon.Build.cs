// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Weapon : ModuleRules
{
	public Weapon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.Add("Core");

        PrivateDependencyModuleNames.Add("UEProject_06");

        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("Slate");
        PrivateDependencyModuleNames.Add("SlateCore");

        PrivateDependencyModuleNames.Add("EditorStyle");
        PrivateDependencyModuleNames.Add("Projects");
        PrivateDependencyModuleNames.Add("UnrealEd");
        PrivateDependencyModuleNames.Add("InputCore");
    }
}
