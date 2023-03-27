// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UEProject_05 : ModuleRules
{
	public UEProject_05(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
    }
}
