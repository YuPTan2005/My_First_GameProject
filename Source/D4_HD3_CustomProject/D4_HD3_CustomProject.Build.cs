// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class D4_HD3_CustomProject : ModuleRules
{
	public D4_HD3_CustomProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"D4_HD3_CustomProject",
			"D4_HD3_CustomProject/Variant_Platforming",
			"D4_HD3_CustomProject/Variant_Platforming/Animation",
			"D4_HD3_CustomProject/Variant_Combat",
			"D4_HD3_CustomProject/Variant_Combat/AI",
			"D4_HD3_CustomProject/Variant_Combat/Animation",
			"D4_HD3_CustomProject/Variant_Combat/Gameplay",
			"D4_HD3_CustomProject/Variant_Combat/Interfaces",
			"D4_HD3_CustomProject/Variant_Combat/UI",
			"D4_HD3_CustomProject/Variant_SideScrolling",
			"D4_HD3_CustomProject/Variant_SideScrolling/AI",
			"D4_HD3_CustomProject/Variant_SideScrolling/Gameplay",
			"D4_HD3_CustomProject/Variant_SideScrolling/Interfaces",
			"D4_HD3_CustomProject/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
