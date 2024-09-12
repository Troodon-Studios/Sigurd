// Copyright 2024 Mario GC. All Rights Reserved.

#include "Texture/TextureGen.h"

#include "ImageUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Noise/NoiseGenerator.h"

void UTextureGen::NewTexture(const FVector2D& GridSize, const int Seed, const FString& Name, const FString& TextureDirectory, const FNoiseParameters& TextNoiseValues, const float Scatter, const EAdvancedNoiseType NoiseType)
{
    const int32 Width = GridSize.X;
    const int32 Height = GridSize.Y;
    UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

    FColor* MipData = static_cast<FColor*>(Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

    GenerateTexture(TextNoiseValues, GridSize, MipData, Scatter, Seed, NoiseType);
    SaveTexture(Texture, Name, TextureDirectory);
}

void UTextureGen::SaveTexture(UTexture2D* Texture, const FString& Name, const FString& TextureDirectory)
{
    Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
    Texture->UpdateResource();
    
    const FString TextureFilename = TextureDirectory + Name;
    
    // Convert the texture to a PNG
    FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
    uint8* Pixels = static_cast<uint8*>(Mip.BulkData.Lock(LOCK_READ_ONLY));
    const TArray<FColor>& SrcData = reinterpret_cast<TArray<FColor>&>(Pixels);
    TArray<uint8> CompressedPNG;

    FImageUtils::ThumbnailCompressImageArray(Mip.SizeX, Mip.SizeY, SrcData, CompressedPNG);
    Mip.BulkData.Unlock();

    // Save the PNG to a file
    FFileHelper::SaveArrayToFile(CompressedPNG, *TextureFilename);
    // save the directory
    FAssetRegistryModule::AssetCreated(Texture);
    Texture->PostEditChange();
}

void UTextureGen::GenerateTexture(const FNoiseParameters& TextNoiseValues, const FVector2D& GridSize, FColor* MipData, const float Scatter, const int Seed, const EAdvancedNoiseType NoiseType)
{
    const float MFrequency = TextNoiseValues.Frequency / Scatter;
    const float MAmplitude = TextNoiseValues.Amplitude;
    const float MLacunarity = TextNoiseValues.Lacunarity;
    const float MPersistence = TextNoiseValues.Persistence / Scatter;

    const int32 Width = GridSize.X;
    const int32 Height = GridSize.Y;

    constexpr float MinValue = -1;
    constexpr float MaxValue = 1;
    constexpr float ValueRange = MaxValue - MinValue;

    ParallelFor(Width * Height, [&](const int32 Index)
    {
        const int32 x = Index % Width;
        const int32 y = Index / Width;
        const int32 CurPixelIndex = (y * Width) + x;

        float NoiseValue = 0.0f;
        switch (NoiseType)
        {
        case EAdvancedNoiseType::Simplex:
            NoiseValue = UAdvancedNoise::SimplexNoise(
                (x / 10.0f), (y / 10.0f), MFrequency, MAmplitude, MLacunarity, MPersistence, Seed);
            break;
        case EAdvancedNoiseType::Perlin:
            NoiseValue = UAdvancedNoise::PerlinNoise(
                (x / 10.0f), (y / 10.0f),MFrequency, MAmplitude, Seed);
            break;
        case EAdvancedNoiseType::Voronoi:
            NoiseValue = UAdvancedNoise::VoronoiNoise(
                (x / 10.0f), (y / 10.0f), MFrequency, MAmplitude, Seed, Width, Height);
            break;
        }

        NoiseValue = ((NoiseValue - MinValue) / ValueRange) * 255.0f;

        FColor FillColor = FColor::White;
        FillColor.R += NoiseValue;
        FillColor.G += NoiseValue;
        FillColor.B += NoiseValue;

        MipData[CurPixelIndex] = FillColor;
    }, EParallelForFlags::Unbalanced);
}
