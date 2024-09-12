// Copyright 2024 Mario GC. All Rights Reserved.

using UnrealBuildTool;

public class AdvancedNoise : ModuleRules
{
	public AdvancedNoise(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
		);
				
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
		);
			
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"UMG",
				"Slate",
				"SlateCore",
				"DeveloperSettings",
				"EditorScriptingUtilities"
			}
		);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"SlateCore",
				"EditorStyle",
				"Blutility",
				"UMGEditor",
				"UMG",
				"UnrealEd",
				"ToolMenus",
				"EditorFramework",
				"ScriptableEditorWidgets",
				"EditorScriptingUtilities",
				"Projects",
				"InputCore",
				"Slate"
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}