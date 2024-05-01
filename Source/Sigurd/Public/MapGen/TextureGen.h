#pragma once
#include "NoiseSettings.h"

class FTextureGen
{
	static void SaveTexture(UTexture2D* Texture, const FString& Name);
	static void GenerateTexture(const FNoiseValues& TextNoiseValues,const int ExtraDim, const int Seed, const FVector2D& GridSize,FColor* MipData,const float Scatter);
	static void NewTexture(const FVector2D& GridSize, const int ExtraDim, const int Seed, const FString& Name, const FNoiseValues& TextNoiseValues, const float Scatter);

public:
	static void GenerateTextures(const FVector2D& GridSize,TArray<FTextureSetting*> TextureSettings);
};
