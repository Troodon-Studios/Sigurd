#pragma once
#include "CoreMinimal.h"
#include "ModuleTile.h"
#include "Engine/DataTable.h"
#include "NoiseSettings.generated.h"


USTRUCT(BlueprintType)
struct FNoiseValues : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"), DisplayName = "Frequency")
	float Frequency = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"), DisplayName = "Amplitude")
	float Amplitude = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "10.0"), DisplayName = "Lacunarity")
	float Lacunarity = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"), DisplayName = "Persistence")
	float Persistence = 0.5f;
	
};

USTRUCT(BlueprintType)
struct FNoiseSetting : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, DisplayName = "Noise Values")
	FNoiseValues NoiseValues;

	// UModuleTile DataAsset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UModuleTile* ModuleTiles;

};

USTRUCT(BlueprintType)
struct FTextureSetting : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, DisplayName = "Name")
	FString Name = "Texture";

	UPROPERTY( EditAnywhere, BlueprintReadWrite, DisplayName = "Extra Pixels Dimension")
	int ExtraDim = 25;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, DisplayName = "Opacity", meta = (ClampMin = "1.0", ClampMax = "100.0"))
	float Opacity = 100;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, DisplayName = "Scatter", meta = (ClampMin = "1.0", ClampMax = "100.0"))
	float Scatter = 10;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, DisplayName = "Noise Values")
	FNoiseValues NoiseValues;

};