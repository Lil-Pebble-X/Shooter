// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class demo : ModuleRules
{
	public demo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" , "GameplayAbilities", "GameplayTags" });

		PrivateDependencyModuleNames.AddRange(new string[] { "EnhancedInput", "GameplayTasks"});

        PublicIncludePaths.Add(Path.Combine(EngineDirectory, "Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public"));
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
