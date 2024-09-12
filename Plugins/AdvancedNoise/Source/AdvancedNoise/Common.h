// Copyright 2024 Mario GC. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"


#include "Common.generated.h"

USTRUCT(BlueprintType)
struct FNoiseParameters : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Noise", meta = (ClampMin = "0.0"), DisplayName = "Frequency")
	float Frequency = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Noise", meta = (ClampMin = "0.0"), DisplayName = "Amplitude")
	float Amplitude = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Noise", meta = (ClampMin = "0.0"), DisplayName = "Lacunarity")
	float Lacunarity = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Noise", meta = (ClampMin = "0.0"), DisplayName = "Persistence")
	float Persistence = 0.5f;

	FNoiseParameters() {}
	FNoiseParameters(const float Frequency, const float Amplitude, const float Lacunarity, const float Persistence)
		: Frequency(Frequency), Amplitude(Amplitude), Lacunarity(Lacunarity), Persistence(Persistence) {}
	
};

UENUM(BlueprintType)
enum class EAdvancedNoiseType : uint8
{
	Simplex UMETA(DisplayName = "Simplex"),
	Perlin UMETA(DisplayName = "Perlin"),
	Voronoi UMETA(DisplayName = "Voronoi")
};
