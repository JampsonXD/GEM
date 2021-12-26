// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GEMLoadingScreen : ModuleRules
{
	public GEMLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivatePCHHeaderFile = "Public/GEMLoadingScreen.h";
		
		PCHUsage = PCHUsageMode.UseSharedPCHs;
		
		PrivateIncludePaths.Add("GEMLoadingScreen/Private");

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
		
		PrivateDependencyModuleNames.AddRange(new string[] {"MoviePlayer", "Slate", "SlateCore", "InputCore"});
	}
}
