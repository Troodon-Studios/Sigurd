#pragma once
#include <cstddef>

#include "NoiseGenerator.generated.h"

UCLASS()
class UAdvancedNoise : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	static float SimplexNoise(float X, float Y, float Frequency, float Amplitude, float Lacunarity, float Persistence, const int Seed = 0);
	
private:
	static float CalcContribution(int32_t I, int32_t J, float X, float Y);
    
};
