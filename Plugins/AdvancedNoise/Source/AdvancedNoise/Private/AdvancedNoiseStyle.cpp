// Copyright 2024 Mario GC. All Rights Reserved.

#include "AdvancedNoiseStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FAdvancedNoiseStyle::StyleInstance = nullptr;

void FAdvancedNoiseStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FAdvancedNoiseStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FAdvancedNoiseStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("AdvancedNoiseStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FAdvancedNoiseStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("AdvancedNoiseStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("AdvancedNoise")->GetBaseDir() / TEXT("Resources"));

	Style->Set("AdvancedNoise.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("AdvancedNoiseButton"), Icon20x20));

	return Style;
}

void FAdvancedNoiseStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FAdvancedNoiseStyle::Get()
{
	return *StyleInstance;
}
