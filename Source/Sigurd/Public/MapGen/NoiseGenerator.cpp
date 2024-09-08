#include "NoiseGenerator.h"

// Fastfloor function is used to quickly calculate the floor of a floating point number.
static inline int32_t Fastfloor(const float FP) {
    const int32_t i = static_cast<int32_t>(FP);
    return (FP < i) ? (i - 1) : (i);
}

// GPerm is a permutation table used for generating noise.
static const uint8_t GPerm[256] = {
    151, 160, 137, 91, 90, 15,
    131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
    190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
    77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
    102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
    135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
    5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
    223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
    129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
    251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
    49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

// Hash function is used to generate a hash value from an integer.
static inline uint8_t Hash(const int32_t I) {
    return GPerm[static_cast<uint8_t>(I)];
}

// Grad function is used to calculate the gradient at a given point (X, Y) using a hash value.
static float Grad(const int32_t Hash, const float X, const float Y) {
    const int32_t h = Hash & 0x3F;
    const float u = h < 4 ? X : Y;
    const float v = h < 4 ? Y : X;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

// SimplexNoise function generates simplex noise for a given point (X, Y) and parameters frequency, amplitude, lacunarity, and persistence.
float FNoise::SimplexNoise(const float X, const float Y, float Frequency, float Amplitude, const float Lacunarity, const float Persistence) {
    float Total = 0.0f;

    for (int i = 0; i < 4; ++i) {
        constexpr float G2 = 0.211324865f;
        constexpr float F2 = 0.366025403f;
        const float x = X * Frequency;
        const float y = Y * Frequency;
        const float s = (x + y) * F2;
        const int32_t IFast = Fastfloor(x + s);
        const int32_t j = Fastfloor(y + s);
        const float t = static_cast<float>(IFast + j) * G2;
        const float X0 = x - (IFast - t);
        const float Y0 = y - (j - t);
        const int32_t I1 = X0 > Y0 ? 1 : 0;
        const int32_t J1 = X0 > Y0 ? 0 : 1;
        const float N0 = CalcContribution(IFast, j, X0, Y0);
        const float N1 = CalcContribution(IFast + I1, j + J1, X0 - I1 + G2, Y0 - J1 + G2);
        const float N2 = CalcContribution(IFast + 1, j + 1, X0 - 1.0f + 2.0f * G2, Y0 - 1.0f + 2.0f * G2);
        Total += (N0 + N1 + N2) * Amplitude;
        Frequency *= Lacunarity;
        Amplitude *= Persistence;
    }

    return Total;
}

// CalcContribution function calculates the contribution of a given point (x, y) in the noise generation.
float FNoise::CalcContribution(const int32_t I, int32_t j, const float X, const float Y) {
    if (float t = 0.5f - X*X - Y*Y; t < 0.0f) {
        return 0.0f;
    } else {
        const int GI = Hash(I + Hash(j));
        t *= t;
        return t * t * Grad(GI, X, Y);
    }
}