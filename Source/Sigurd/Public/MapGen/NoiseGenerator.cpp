#include "NoiseGenerator.h"

static inline int32_t Fastfloor(const float FP) {
    const int32_t i = static_cast<int32_t>(FP);
    return (FP < i) ? (i - 1) : (i);
}

/**
 * Permutation table. This is just a random jumble of all numbers 0-255.
 *
 * This produce a repeatable pattern of 256, but Ken Perlin stated
 * that it is not a problem for graphic texture as the noise features disappear
 * at a distance far enough to be able to see a repeatable pattern of 256.
 *
 * This needs to be exactly the same for all instances on all platforms,
 * so it's easiest to just keep it as static explicit data.
 * This also removes the need for any initialisation of this class.
 *
 * Note that making this an uint32_t[] instead of a uint8_t[] might make the
 * code run faster on platforms with a high penalty for unaligned single
 * byte addressing. Intel x86 is generally single-byte-friendly, but
 * some other CPUs are faster with 4-aligned reads.
 * However, a char[] is smaller, which avoids cache trashing, and that
 * is probably the most important aspect on most architectures.
 * This array is accessed a *lot* by the noise functions.
 * A vector-valued noise over 3D accesses it 96 times, and a
 * float-valued 4D noise 64 times. We want this to fit in the cache!
 */
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

/**
 * Helper function to hash an integer using the above permutation table
 *
 *  This inline function costs around 1ns, and is called N+1 times for a noise of N dimension.
 *
 *  Using a real hash function would be better to improve the "repeatability of 256" of the above permutation table,
 * but fast integer Hash functions uses more time and have bad random properties.
 *
 * @param[in] I Integer value to hash
 *
 * @return 8-bits hashed value
 */
static inline uint8_t Hash(const int32_t I) {
    return GPerm[static_cast<uint8_t>(I)];
}

/* NOTE Gradient table to test if lookup-table are more efficient than calculs
static const float gradients1D[16] = {
        -8.f, -7.f, -6.f, -5.f, -4.f, -3.f, -2.f, -1.f,
         1.f,  2.f,  3.f,  4.f,  5.f,  6.f,  7.f,  8.f
};
*/

/**
 * Helper functions to compute gradients-dot-residual vectors (2D)
 *
 * @param[in] Hash  hash value
 * @param[in] X     x coord of the distance to the corner
 * @param[in] Y     y coord of the distance to the corner
 *
 * @return gradient value
 */
static float Grad(const int32_t Hash, const float X, const float Y) {
    const int32_t h = Hash & 0x3F;  // Convert low 3 bits of hash code
    const float u = h < 4 ? X : Y;  // into 8 simple gradient directions,
    const float v = h < 4 ? Y : X;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v); // and compute the dot product with (x,y).
}

/**
 * 2D Perlin simplex noise
 *
 *  Takes around 150ns on an AMD APU.
 *
 * @param[in] X float coordinate
 * @param[in] Y float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float FSimplexNoise::SimplexNoise(const float X, const float Y, float frequency, float amplitude, float lacunarity, float persistence) {
    float total = 0.0f;
    const float F2 = 0.366025403f; // Define F2
    const float G2 = 0.211324865f; // Define G2

    for (int i = 0; i < 4; ++i) { // 4 octaves
        float x = X * frequency;
        float y = Y * frequency;

        // Skew the input space to determine which simplex cell we're in
        const float s = (x + y) * F2;
        const int32_t iFast = Fastfloor(x + s);
        const int32_t j = Fastfloor(y + s);

        // Unskew the cell origin back to (x,y) space
        const float t = static_cast<float>(iFast + j) * G2;
        const float x0 = x - (iFast - t);
        const float y0 = y - (j - t);

        // Determine which simplex we are in
        int32_t I1 = x0 > y0 ? 1 : 0;
        int32_t J1 = x0 > y0 ? 0 : 1;

        // Calculate the contribution from the three corners
        float N0 = CalcContribution(iFast, j, x0, y0);
        float N1 = CalcContribution(iFast + I1, j + J1, x0 - I1 + G2, y0 - J1 + G2);
        float N2 = CalcContribution(iFast + 1, j + 1, x0 - 1.0f + 2.0f * G2, y0 - 1.0f + 2.0f * G2);

        // Add contributions from each corner to get the final noise value
        total += (N0 + N1 + N2) * amplitude;

        // Increase the frequency and decrease the amplitude for the next octave
        frequency *= lacunarity;
        amplitude *= persistence;
    }
    
    return total;
}
float FSimplexNoise::CalcContribution(int32_t i, int32_t j, float x, float y) {
    float t = 0.5f - x*x - y*y;
    if (t < 0.0f) {
        return 0.0f;
    } else {
        int GI = Hash(i + Hash(j));
        t *= t;
        return t * t * Grad(GI, x, y);
    }
}
