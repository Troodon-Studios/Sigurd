// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdvancedNoise.h"

#include "AdvancedNoiseSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FAdvancedNoiseModule"

void FAdvancedNoiseModule::StartupModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "AdvancedNoise",
			LOCTEXT("AdvancedNoiseSettingsName", "Advanced Noise"),
			LOCTEXT("AdvancedNoiseSettingsDescription", "Configure the Advanced Noise plugin settings"),
			GetMutableDefault<UAdvancedNoiseSettings>()
		);
	}
}

void FAdvancedNoiseModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "AdvancedNoise");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAdvancedNoiseModule, AdvancedNoise)