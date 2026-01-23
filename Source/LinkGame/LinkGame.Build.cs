// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LinkGame : ModuleRules
{
	public LinkGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			// Input
			"InputCore", 
			"EnhancedInput",
			// 모듈형 컴포넌트 
			"GameFeatures",
			"ModularGameplay",
			"ModularGameplayActors",
			// GAS Tag
			"GameplayTags",
			// GAS Ability System
			"GameplayTasks",
			"GameplayAbilities",
			// CommonUser
			"CommonUser",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(new string[] { "LinkGame" });


		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
