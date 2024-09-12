// Copyright 2024 Mario GC. All Rights Reserved.

#include "AdvancedNoise.h"

#include "AdvancedNoiseCommands.h"
#include "AdvancedNoiseSettings.h"
#include "AdvancedNoiseStyle.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "ISettingsModule.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FAdvancedNoiseModule"

UEditorUtilityWidget* WidgetWindow;

void FAdvancedNoiseModule::StartupModule()
{

	FAdvancedNoiseStyle::Initialize();
	FAdvancedNoiseStyle::ReloadTextures();

	FAdvancedNoiseCommands::Register();
    
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAdvancedNoiseCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FAdvancedNoiseModule::PluginButtonClicked),
		FCanExecuteAction(),
		FIsActionChecked::CreateRaw(this, &FAdvancedNoiseModule::IsPluginWindowOpen));

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAdvancedNoiseModule::RegisterMenus));
	
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

void FAdvancedNoiseModule::PluginButtonClicked()
{
	if (false)
	{
		WidgetWindow->Destruct();
		WidgetWindow = nullptr;
	}
	else
	{
		const FString AssetPath = "/AdvancedNoise/UI/EUW_AdvancedNoise.EUW_AdvancedNoise";

		if (const FSoftObjectPath WidgetAssetPath(AssetPath); WidgetAssetPath.IsValid())
		{
			if (UObject* WidgetAsset = WidgetAssetPath.TryLoad())
			{
				if (UEditorUtilityWidgetBlueprint* WidgetBlueprint = Cast<UEditorUtilityWidgetBlueprint>(WidgetAsset))
				{
					UE_LOG(LogTemp, Warning, TEXT("WidgetBlueprint: %s"), *WidgetBlueprint->GetName());

					if (UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
					{
						WidgetWindow = EditorUtilitySubsystem->SpawnAndRegisterTab(WidgetBlueprint);
					}
				}
			}
		}
	}
}

void FAdvancedNoiseModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
		FToolMenuSection& Section = Menu->FindOrAddSection("Tools");
		Section.AddMenuEntryWithCommandList(FAdvancedNoiseCommands::Get().OpenPluginWindow, PluginCommands);
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
		FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FAdvancedNoiseCommands::Get().OpenPluginWindow));
		Entry.SetCommandList(PluginCommands);
	}
}

bool FAdvancedNoiseModule::IsPluginWindowOpen()
{
	return false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAdvancedNoiseModule, AdvancedNoise)