// Copyright (c) Blueshift Interactive Ltd (2025)

using UnrealBuildTool;

public class BlueshiftGameSettings : ModuleRules
{
	public BlueshiftGameSettings(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateDependencyModuleNames.AddRange(new string[] {
			"CoreUObject",
			"DeveloperSettings",
			"Engine",
			"GameplayTags",
			"SlateCore",
			"UMG",
		});

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
		});
	}
}