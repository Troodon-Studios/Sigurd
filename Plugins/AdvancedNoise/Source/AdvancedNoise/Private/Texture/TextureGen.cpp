#include "Texture/TextureGen.h"

#include "ImageUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Noise/NoiseGenerator.h"

void FTextureGen::NewTexture(const FVector2D& GridSize, const int Seed, const FString& Name, const FString& TextureDirectory, const FNoiseParameters& TextNoiseValues, const float Scatter)
{

	const int32 Width = GridSize.X;
	const int32 Height = GridSize.Y;
	UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

	FColor* MipData = static_cast<FColor*>(Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	GenerateTexture(TextNoiseValues, GridSize, MipData, Scatter, Seed);
	SaveTexture(Texture, Name, TextureDirectory);
}

void FTextureGen::SaveTexture(UTexture2D* Texture, const FString& Name, const FString& TextureDirectory)
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

void FTextureGen::GenerateTexture(const FNoiseParameters& TextNoiseValues, const FVector2D& GridSize, FColor* MipData, const float Scatter,const int Seed)
{
	const float MFrequency = TextNoiseValues.Frequency / Scatter;
	const float MAmplitude = TextNoiseValues.Amplitude;
	const float MLacunarity = TextNoiseValues.Lacunarity;
	const float MPersistence = TextNoiseValues.Persistence / Scatter;

	const int32 Width = GridSize.X;

	for (int x = 0; x < GridSize.X; x++)
	{
		for (int y = 0; y < GridSize.Y; y++)
		{
			const int32 CurPixelIndex = (y * Width) + x;

			float NoiseValue = UAdvancedNoise::SimplexNoise(
				(x / 10.0f), (y / 10.0f), MFrequency, MAmplitude, MLacunarity, MPersistence, Seed);
            
			constexpr float MinValue = -0.024097f;
			constexpr float MaxValue = 0.024268f;
			NoiseValue = ((NoiseValue - MinValue) / (MaxValue - MinValue)) * 255;

			FColor FillColor = FColor::White;

			FillColor.R += NoiseValue;
			FillColor.G += NoiseValue;
			FillColor.B += NoiseValue;

			MipData[CurPixelIndex] = FillColor;
		}
	}
}