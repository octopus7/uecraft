// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class NyangCraft : ModuleRules
{
	public NyangCraft(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG", "Slate", "SlateCore" });

        // Ensure module root is on include path so headers under RTS/... resolve with
        // #include "RTS/.../Header.h" style across the module
        PublicIncludePaths.AddRange(new string[] {
            ModuleDirectory
        });

        PrivateIncludePaths.AddRange(new string[] {
            ModuleDirectory
        });
    }
}
