#pragma once

#include "NoiseSettings.h"
#include "NoiseSettingsTable.h"
using namespace std;
class FTextureGen
{
	static void SaveTexture(UTexture2D* Texture, const FString& Name);
	static void GenerateTexture(const FNoiseValues& TextNoiseValues, const int Seed, const FVector2D& GridSize,FColor* MipData,const float Scatter);
	static void NewTexture(const FVector2D& GridSize, const int Seed, const FString& Name, const FNoiseValues& TextNoiseValues, const float Scatter);
	static void SetRandomTexture(UMaterialInstanceDynamic* ProceduralMat, TArray<FMaterialTextures> Set, const FString& Type);
	
	//make a double float array variable called terrainData
	static TArray<TArray<float>> TerrainData, DetailData, ExtraData;
	static constexpr int ExtraDim = 30;
	
public:
	static void GenerateTextures(const FVector2D& GridSize,TArray<FTextureSetting*> TextureSettings);
	static void GenerateTextures(const FVector2D& GridSize, const FTextureSetting& TextureSetting);
	static void GenerateTextures(const FVector2D& GridSize, const FTextureSettingsTable& TextureSettingsTable);
	static void SetMaterialTextures(UMaterialInstanceDynamic* ProceduralMat, const UDataTable* TextureSetsDataTable);
};
