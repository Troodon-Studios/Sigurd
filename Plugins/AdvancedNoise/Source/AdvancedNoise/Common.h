#pragma once
#include "CoreMinimal.h"


#include "Common.generated.h"

USTRUCT(BlueprintType)
struct FNoiseParameters : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"), DisplayName = "Frequency")
	float Frequency = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"), DisplayName = "Amplitude")
	float Amplitude = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"), DisplayName = "Lacunarity")
	float Lacunarity = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"), DisplayName = "Persistence")
	float Persistence = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"), DisplayName = "Cell Size")
	float CellSize = 1.0f;

	FNoiseParameters() {}
	FNoiseParameters(const float Frequency, const float Amplitude, const float Lacunarity, const float Persistence, const float CellSize)
		: Frequency(Frequency), Amplitude(Amplitude), Lacunarity(Lacunarity), Persistence(Persistence), CellSize(CellSize) {}
	
};

UENUM(BlueprintType)
enum class ENoiseType : uint8
{
	Simplex UMETA(DisplayName = "Simplex"),
	Perlin UMETA(DisplayName = "Perlin"),
	Voronoi UMETA(DisplayName = "Voronoi")
};
