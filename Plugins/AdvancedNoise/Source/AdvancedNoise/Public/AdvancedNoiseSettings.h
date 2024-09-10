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
	
	UPROPERTY(EditAnywhere, Config, Category = "Noise Settings", meta = (AllowedClasses = "DataTable", RowStructure = "/Script/AdvancedNoise.NoiseParameters"))
	FSoftObjectPath NoiseParametersTable;

	UPROPERTY(EditAnywhere, Config, Category = "Export Settings")
	FVector2f TextureSize = FVector2f(512, 512);

	UPROPERTY(EditAnywhere, Config, Category = "Export Settings")
	FDirectoryPath ExportPath;

	// Last used noise parameters
	UPROPERTY(Config)
	float Frequency = 0.1f;

	UPROPERTY(Config)
	float Amplitude = 0.6f;

	UPROPERTY(Config)
	float Lacunarity = 2.0f;

	UPROPERTY(Config)
	float Persistence = 0.5f;

	UPROPERTY(Config)
	float CellSize = 1.0f;

	UPROPERTY(Config)
	uint8 NoiseType = 0;
	
private:
	// UObject interface
	virtual void PostInitProperties() override;
};
