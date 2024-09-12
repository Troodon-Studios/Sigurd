// Copyright 2024 Mario GC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AdvancedNoiseStyle.h"

class FAdvancedNoiseCommands : public TCommands<FAdvancedNoiseCommands>
{
public:

	FAdvancedNoiseCommands()
		: TCommands<FAdvancedNoiseCommands>(TEXT("AdvancedNoise"), NSLOCTEXT("Contexts", "Advanced Noise", "Advanced Noise Plugin"), NAME_None, FAdvancedNoiseStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};