#pragma once

#include "NoiseGenerator.generated.h"

UCLASS()
class UAdvancedNoise : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	static float SimplexNoise(float X, float Y, float Frequency, float Amplitude, float Lacunarity, float Persistence, const int Seed = 0);

	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	static float PerlinNoise(const float X,const float Y,const float Frequency, const float Amplitude,const int Seed);

	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	static float VoronoiNoise(float X, float Y, float CellSize, int Seed);

private:
	static float CalcContribution(int32_t I, int32_t J, float X, float Y);
};
