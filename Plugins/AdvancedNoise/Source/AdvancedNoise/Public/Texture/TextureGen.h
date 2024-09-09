#pragma once

#include "../Common.h"

#include "TextureGen.generated.h"

UCLASS()
class UTextureGen : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	static void NewTexture(const FVector2D& GridSize, const int Seed, const FString& Name, const FString& TextureDirectory, const FNoiseParameters& TextNoiseValues, const float Scatter);
private:
	static void SaveTexture(UTexture2D* Texture, const FString& Name,const FString& TextureDirectory);
	static void GenerateTexture(const FNoiseParameters& TextNoiseValues, const FVector2D& GridSize, FColor* MipData,float Scatter, const int Seed);
};
