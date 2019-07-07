// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class OnlineSubsystemDiscord : ModuleRules
{
	public OnlineSubsystemDiscord(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
        if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "discordlib/x86/discord_game_sdk.dll.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "discordlib/x86_64/discord_game_sdk.dll.lib"));
        }

        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                // ... add other public dependencies that you statically link with here ...
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                // ... add private dependencies that you statically link with here ...	
                "OnlineSubsystem",
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
                // ... add any modules that your module loads dynamically here ...
            }
			);
	}
}
