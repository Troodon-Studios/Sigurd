// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AdvancedNoiseSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Advanced Noise Settings"))
class ADVANCEDNOISE_API UAdvancedNoiseSettings : public UObject
{
	GENERATED_UCLASS_BODY()
	
	/** The DataTable for storing saved noise parameters */ 
	UPROPERTY(EditAnywhere, Config, Category = "Noise Settings", meta = (AllowedClasses = "DataTable", RowStructure = "/Script/AdvancedNoise.NoiseParameters"))
	FSoftObjectPath NoiseParametersTable;

	///Script/AdvancedNoise.NoiseParameters
private:
	// UObject interface
	virtual void PostInitProperties() override;
};
