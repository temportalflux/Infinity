// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Infinity : ModuleRules
{
	public Infinity(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
            // Required for /defaults/
            "Core", "CoreUObject", "Engine", "InputCore",

            // Misc default dependencies
            "HeadMountedDisplay", "Paper2D",
            
            // Required for GAS
            "GameplayAbilities", "GameplayTags", "GameplayTasks",
            // Required for Discord
            "Http", "Json", "JsonUtilities",

        });
	}
}
