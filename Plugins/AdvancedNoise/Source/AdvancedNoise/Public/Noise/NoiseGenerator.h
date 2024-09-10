#pragma once

#include "NoiseGenerator.generated.h"

/**
 * UAdvancedNoise class responsible for generating various types of noise.
 */
UCLASS()
class UAdvancedNoise : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Generates simplex noise for a given point (X, Y) and parameters.
	 * @param X - The X coordinate.
	 * @param Y - The Y coordinate.
	 * @param Frequency - The frequency of the noise.
	 * @param Amplitude - The amplitude of the noise.
	 * @param Lacunarity - The lacunarity of the noise.
	 * @param Persistence - The persistence of the noise.
	 * @param Seed - The seed value for noise generation (default is 0).
	 * @return The generated simplex noise value.
	 */
	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	static float SimplexNoise(float X, float Y, float Frequency, float Amplitude, float Lacunarity, float Persistence, const int Seed = 0);

	/**
	 * Generates Perlin noise for a given point (X, Y) and parameters.
	 * @param X - The X coordinate.
	 * @param Y - The Y coordinate.
	 * @param Frequency - The frequency of the noise.
	 * @param Amplitude - The amplitude of the noise.
	 * @param Seed - The seed value for noise generation.
	 * @return The generated Perlin noise value.
	 */
	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	static float PerlinNoise(const float X, const float Y, const float Frequency, const float Amplitude, const int Seed);

	/**
	 * Generates Voronoi noise for a given point (X, Y) and parameters.
	 * @param X - The X coordinate.
	 * @param Y - The Y coordinate.
	 * @param CellSize - The size of the Voronoi cells.
	 * @param Amplitude - The amplitude of the noise.
	 * @param Seed - The seed value for noise generation.
	 * @param Width - The width of the noise grid.
	 * @param Height - The height of the noise grid.
	 * @return The generated Voronoi noise value.
	 */
	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	static float VoronoiNoise(float X, float Y, float CellSize, float Amplitude, int Seed, int Width, int Height);

private:
	/**
	 * Calculates the contribution of a given point (X, Y) in noise generation.
	 * @param I - The grid cell X coordinate.
	 * @param J - The grid cell Y coordinate.
	 * @param X - The X coordinate within the cell.
	 * @param Y - The Y coordinate within the cell.
	 * @return The contribution value.
	 */
	static float CalcContribution(int32_t I, int32_t J, float X, float Y);
};