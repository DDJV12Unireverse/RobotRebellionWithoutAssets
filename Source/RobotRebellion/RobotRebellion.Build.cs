// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RobotRebellion : ModuleRules
{
	public RobotRebellion(TargetInfo Target)
	{
        UEBuildConfiguration.bForceEnableExceptions = true;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG",
           "OnlineSubsystem", "OnlineSubsystemUtils", "Slate", "SlateCore", "AIModule", "GameplayTasks" });
		
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
