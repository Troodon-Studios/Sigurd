// Copyright 2024 Mario GC. All Rights Reserved.

#include "AdvancedNoiseCommands.h"

#define LOCTEXT_NAMESPACE "FAdvancedNoiseModule"

void FAdvancedNoiseCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Advanced Noise", "Bring up AdvancedNoise window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
