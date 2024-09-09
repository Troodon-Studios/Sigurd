#pragma once

#include "NoiseSettings.h"
#include "NoiseSettingsTable.h"
using namespace std;
class UTextureGen
{
	static void SaveTexture(UTexture2D* Texture, const FString& Name);
	static void GenerateTexture(const FNoiseValues& TextNoiseValues, const int Seed, const FVector2D& GridSize,FColor* MipData,const float Scatter);
	static void NewTexture(const FVector2D& GridSize, const int Seed, const FString& Name, const FNoiseValues& TextNoiseValues, const float Scatter);
	static void SetRandomTexture(UMaterialInstanceDynamic* ProceduralMat, TArray<FMaterialTextures> Set, const FString& Type);
	static float GetTextureColorAtActorPosition(const FVector& ActorPosition, const FColor* TextureData, const FVector2D& TextureSize, const float WorldSize);
	
	static FColor* TerrainData;
	static FColor* DetailData;
	static FColor* ExtraData;

	static FElementsValue TerrainElements;
	static FElementsValue DetailElements;
	static FElementsValue ExtraElements;	
	
	static constexpr int ExtraDim = 30;
	
public:
	static void GenerateTextures(const FVector2D& GridSize,TArray<FTextureSetting*> TextureSettings);
	static void GenerateTextures(const FVector2D& GridSize, const FTextureSetting& TextureSetting);
	static void GenerateTextures(const FVector2D& GridSize, const FTextureSettingsTable& TextureSettingsTable);
	static void SetMaterialTextures(UMaterialInstanceDynamic* ProceduralMat, const UDataTable* TextureSetsDataTable);
	static FElementsValue GetSoundValues(const AActor* Actor);
};
