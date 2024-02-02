#pragma once
#include <cstddef>

class FSimplexNoise {
public:

    // 2D Perlin simplex noise
    static float SimplexNoise(const float X, const float Y, float frequency, float amplitude, float lacunarity, float persistence);
    static float CalcContribution(int32_t i, int32_t j, float x, float y);
    
};