#pragma once
#include <cstddef>

class FNoise {
public:

	// 2D simplex noise
	static float SimplexNoise(const float X, const float Y, float Frequency, float Amplitude, float Lacunarity, float Persistence);
	static float CalcContribution(int32_t I, int32_t J, float X, float Y);
    
};
