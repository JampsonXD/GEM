// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GEM : ModuleRules
{
	public GEM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "InventorySystem", "HeadMountedDisplay", "QuestSystem" , "UMG"});
		
		PrivateDependencyModuleNames.AddRange(new string[] {"GEMLoadingScreen", "GameplayAbilities", "GameplayTags", "GameplayTasks", "EnhancedInput", "Niagara", "Slate", "SlateCore"});
	}
}
