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
	FVector2f TextureSize;

	UPROPERTY(EditAnywhere, Config, Category = "Export Settings")
	FDirectoryPath ExportPath;

	// Last used noise parameters
	UPROPERTY(Config)
	float Frequency;

	UPROPERTY(Config)
	float Amplitude;

	UPROPERTY(Config)
	float Lacunarity;

	UPROPERTY(Config)
	float Persistence;

	UPROPERTY(Config)
	uint8 NoiseType;
	
private:
	// UObject interface
	virtual void PostInitProperties() override;
};
