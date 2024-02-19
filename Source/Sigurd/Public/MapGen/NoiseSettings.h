#pragma once
#include "CoreMinimal.h"
#include "ModuleTile.h"
#include "Engine/DataTable.h"
#include "NoiseSettings.generated.h"

USTRUCT(BlueprintType)
struct FNoiseSetting : public FTableRowBase
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

	// UModuleTile DataAsset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UModuleTile* ModuleTiles;

};