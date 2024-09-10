#include "Noise/NoiseGenerator.h"

/** Common Functions **/
// Permutation table for hash values.
static const uint8_t GPerm[256] = {
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225,
    140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148,
    247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
    57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175,
    74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122,
    60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54,
    65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
    200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3,
    64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82,
    85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223,
    183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155,
    167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224,
    232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238,
    210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
    107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115,
    121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29,
    24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

// Common fast floor function.
static inline int32_t Fastfloor(const float FP) {
    const int32_t i = static_cast<int32_t>(FP);
    return (FP < i) ? (i - 1) : i;
}

// Common hash function.
static inline uint8_t Hash(const int32_t I) {
    return GPerm[static_cast<uint8_t>(I)];
}

// Common gradient calculation function.
static float Grad(const int32_t Hash, const float X, const float Y) {
    const int32_t h = Hash & 0x3F;
    const float u = h < 4 ? X : Y;
    const float v = h < 4 ? Y : X;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

// Common fade function for smoothing interpolation.
static float Fade(const float T) {
    return T * T * T * (T * (T * 6 - 15) + 10);
}

// Common linear interpolation function.
static float Lerp(const float T, const float A, const float B) {
    return A + T * (B - A);
}

// Calculates the contribution of a given point (X, Y) in noise generation.
float UAdvancedNoise::CalcContribution(const int32_t I, const int32_t J, const float X, const float Y) {
    float t = 0.5f - X * X - Y * Y;
    if (t < 0.0f) return 0.0f;
    const int32_t GI = Hash(I + Hash(J));
    t *= t;
    return t * t * Grad(GI, X, Y);
}

/** Noise Functions **/
// Generates simplex noise for a given point (X, Y) and parameters.
float UAdvancedNoise::SimplexNoise(float X, float Y, float Frequency, float Amplitude, const float Lacunarity, const float Persistence, const int Seed) {
    float Total = 0.0f;
    
    X += Seed;
    Y += Seed;

    for (int i = 0; i < 4; ++i) {
        constexpr float F2 = 0.366025403f;
        constexpr float G2 = 0.211324865f;
        const float x = X * Frequency;
        const float y = Y * Frequency;
        const float s = (x + y) * F2;
        const int32_t I0 = Fastfloor(x + s);
        const int32_t J0 = Fastfloor(y + s);
        const float t = static_cast<float>(I0 + J0) * G2;
        const float X0 = x - (I0 - t);
        const float Y0 = y - (J0 - t);
        const int32_t I1 = X0 > Y0 ? 1 : 0;
        const int32_t J1 = X0 > Y0 ? 0 : 1;
        const float N0 = CalcContribution(I0, J0, X0, Y0);
        const float N1 = CalcContribution(I0 + I1, J0 + J1, X0 - I1 + G2, Y0 - J1 + G2);
        const float N2 = CalcContribution(I0 + 1, J0 + 1, X0 - 1.0f + 2.0f * G2, Y0 - 1.0f + 2.0f * G2);
        Total += (N0 + N1 + N2) * Amplitude;
        Frequency *= Lacunarity;
        Amplitude *= Persistence;
    }
    return Total;
}

static float DotGrad(int hash, float x, float y) {
    switch (hash & 7) {
    case 0: return x + y;
    case 1: return x;
    case 2: return x - y;
    case 3: return -y;
    case 4: return -x - y;
    case 5: return -x;
    case 6: return -x + y;
    case 7: return y;
    default: return 0.0f;
    }
}

// Calculates the contribution of a given point (X, Y) in noise generation.
float UAdvancedNoise::PerlinNoise(float X, float Y,const float Frequency,const float Amplitude, const int Seed) {

    X *= Frequency;
    Y *= Frequency;
    X += Seed;
    Y += Seed;

    // Calculate grid cell coordinates
    const int32_t X0 = Fastfloor(X);
    const int32_t Y0 = Fastfloor(Y);
    const int32_t X1 = X0 + 1;
    const int32_t Y1 = Y0 + 1;

    // Relative coordinates within the grid cell
    const float x0 = X - X0;
    const float y0 = Y - Y0;
    const float x1 = x0 - 1.0f;
    const float y1 = y0 - 1.0f;

    // Compute fade curves for x and y
    const float u = Fade(x0);
    const float v = Fade(y0);

    // Hash coordinates of the four corners
    const uint8_t hash00 = Hash(X0 + Hash(Y0));
    const uint8_t hash01 = Hash(X0 + Hash(Y1));
    const uint8_t hash10 = Hash(X1 + Hash(Y0));
    const uint8_t hash11 = Hash(X1 + Hash(Y1));

    // Compute gradients at the four corners
    const float grad00 = Grad(hash00, x0, y0);
    const float grad01 = Grad(hash01, x0, y1);
    const float grad10 = Grad(hash10, x1, y0);
    const float grad11 = Grad(hash11, x1, y1);

    // Compute the dot product between the gradients and the vector from the grid point
    const float dot00 = grad00 * x0 + grad00 * y0;
    const float dot01 = grad01 * x0 + grad01 * y1;
    const float dot10 = grad10 * x1 + grad10 * y0;
    const float dot11 = grad11 * x1 + grad11 * y1;

    // Compute interpolated values
    const float lerpX0 = Lerp(u, dot00, dot10);
    const float lerpX1 = Lerp(u, dot01, dot11);
    return Lerp(v, lerpX0, lerpX1) * Amplitude;
}


// Voronoi distance function.
static float VoronoiDistance(const FVector2D& Point, const FVector2D& Site) {
    return (Point - Site).Size(); // Calculate the Euclidean distance
}

// Update Voronoi minima.
static float UpdateVoronoiMinima(const float Minima, const float Distance) {
    return FMath::Min(Minima, Distance); // Update the minimum distance
}

// Voronoi noise generation function.
static float VoronoiNoise(float X, float Y, float Frequency, int Seed){
    const FVector2D& Position = FVector2D(X, Y);
    const int32_t I0 = Fastfloor(Position.X * Frequency);
    const int32_t J0 = Fastfloor(Position.Y * Frequency);

    float Minima = FLT_MAX; // Initialize minima with the maximum possible float value

    for (int32_t u = -1; u <= 1; ++u) {
        for (int32_t v = -1; v <= 1; ++v) {
            const int32_t HashValue = Hash(I0 + u + Hash(J0 + v));
            const FVector2D Site(
                (u + HashValue % 256) * Frequency, 
                (v + HashValue % 256) * Frequency
            ); // Generate site coordinates
            float Distance = VoronoiDistance(Position, Site);
            Minima = UpdateVoronoiMinima(Minima, Distance);
        }
    }

    return Minima; // Return the minimum distance as noise value
}
