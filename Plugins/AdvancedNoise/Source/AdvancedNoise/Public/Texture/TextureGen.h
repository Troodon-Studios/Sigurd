// Copyright 2024 Mario GC. All Rights Reserved.

#pragma once

#include "../Common.h"

#include "TextureGen.generated.h"

/**
 * UTextureGen class responsible for generating and saving textures.
 */
UCLASS()
class UTextureGen : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Creates a new texture based on the provided parameters.
	 * @param GridSize - The size of the texture grid.
	 * @param Seed - The seed value for noise generation.
	 * @param Name - The name of the texture.
	 * @param TextureDirectory - The directory where the texture will be saved.
	 * @param TextNoiseValues - The noise parameters for texture generation.
	 * @param Scatter - The scatter value for noise generation.
	 * @param NoiseType - The type of noise to generate.
	 */
	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	static void NewTexture(const FVector2D& GridSize, const int Seed, const FString& Name, const FString& TextureDirectory, const FNoiseParameters& TextNoiseValues, const float Scatter, const EAdvancedNoiseType NoiseType);
private:
	/**
	 * Saves the generated texture to the specified directory.
	 * @param Texture - The texture to save.
	 * @param Name - The name of the texture file.
	 * @param TextureDirectory - The directory where the texture will be saved.
	 */
	static void SaveTexture(UTexture2D* Texture, const FString& Name, const FString& TextureDirectory);

	/**
	 * Generates the texture based on the provided noise parameters.
	 * @param TextNoiseValues - The noise parameters for texture generation.
	 * @param GridSize - The size of the texture grid.
	 * @param MipData - The mip data of the texture.
	 * @param Scatter - The scatter value for noise generation.
	 * @param Seed - The seed value for noise generation.
	 * @param NoiseType - The type of noise to generate.
	 */
	static void GenerateTexture(const FNoiseParameters& TextNoiseValues, const FVector2D& GridSize, FColor* MipData, const float Scatter, const int Seed, const EAdvancedNoiseType NoiseType);
};