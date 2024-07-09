// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Dust : ModuleRules
{
	public Dust(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.Add(ModuleDirectory);
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", 
            "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemUtils", "UMG" });

	}
}
