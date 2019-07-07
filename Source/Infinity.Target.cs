// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class InfinityTarget : TargetRules
{
	public InfinityTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Infinity");
        //GlobalDefinitions.Add("UE4_PROJECT_DISCORDCLIENTID=574661303034183690");
    }
}
