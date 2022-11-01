// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyUE5Project : ModuleRules
{
	public MyUE5Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Add the "MetasoundEngine" module to use Metasound Source.
		// Add the "AudioMixer" module to use the Quartz Subsystem. 
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "MetasoundEngine", "AudioMixer"});
	}
}
