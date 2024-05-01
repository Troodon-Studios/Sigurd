#include "TextureGen.h"

#include "ImageUtils.h"
#include "NoiseGenerator.h"

void FTextureGen::NewTexture(const FVector2D& GridSize, const int ExtraDim, const int Seed, const FString& Name, const FNoiseValues& TextNoiseValues, const float Scatter)
{
	// Crear una textura
	// print generating a new texture and the name
	UE_LOG(LogTemp, Warning, TEXT("Generating a new texture with name: %s"), *Name);
	const int32 Width = GridSize.X * ExtraDim;
	const int32 Height = GridSize.Y * ExtraDim;
	UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

	// Acceder a los datos de la textura
	FColor* MipData = static_cast<FColor*>(Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	GenerateTexture(TextNoiseValues, ExtraDim, Seed, GridSize, MipData, Scatter);
	SaveTexture(Texture, Name);
}

void FTextureGen::GenerateTextures(const FVector2D& GridSize, TArray<FTextureSetting*> TextureSettings)
{
	// Recorrer TextureSettings
	for (const FTextureSetting* TextureSetting : TextureSettings)
	{
		NewTexture(GridSize, TextureSetting->ExtraDim, FMath::RandRange(0, 1000), TextureSetting->Name + "_procText.png", TextureSetting->NoiseValues, TextureSetting->Scatter);
	}
}

void FTextureGen::SaveTexture(UTexture2D* Texture, const FString& Name)
{
	// Save the texture as a PNG file
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
	Texture->UpdateResource();

	// Guardar la textura como un archivo PNG
	FString TextureDirectory = FPaths::ProjectContentDir() + TEXT("ProceduralTextures/");
	FString TextureFilename = TextureDirectory + Name;

	// Convert the texture to a PNG
	FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
	uint8* Pixels = static_cast<uint8*>(Mip.BulkData.Lock(LOCK_READ_ONLY));
	const TArray<FColor>& SrcData = reinterpret_cast<TArray<FColor>&>(Pixels);
	TArray<uint8> CompressedPNG;
	FImageUtils::CompressImageArray(Mip.SizeX, Mip.SizeY, SrcData, CompressedPNG);
	Mip.BulkData.Unlock();

	// Save the PNG to a file
	FFileHelper::SaveArrayToFile(CompressedPNG, *TextureFilename);
}

void FTextureGen::GenerateTexture(const FNoiseValues& TextNoiseValues,const int ExtraDim, const int Seed, const FVector2D& GridSize,FColor* MipData,const float Scatter)
{
	const float MFrequency = TextNoiseValues.Frequency/Scatter;
	const float MAmplitude = TextNoiseValues.Amplitude;
	const float MLacunarity = TextNoiseValues.Lacunarity;
	const float MPersistence = TextNoiseValues.Persistence/Scatter;

	int32 ExtraDimLoc = ExtraDim;
	FColor FillColor = FColor::White;
	int32 Width = GridSize.X * ExtraDim;

	// Recorrer ModuleNumbers
	for (int x = 0; x < GridSize.X; x++)
	{
		for (int y = 0; y < GridSize.Y; y++)
		{
			for (int i = 0; i < ExtraDim; i++)
			{
				for (int j = 0; j < ExtraDim; j++)
				{
					const int32 CurPixelIndex = ((y * ExtraDim + j) * Width) + (x * ExtraDim + i);
					// Calcular el valor de ruido
					float NoiseValue = (FNoise::SimplexNoise(
						((x * ExtraDim + i) / 10.0f) + Seed, ((y * ExtraDim + j) / 10.0f) + Seed, MFrequency,
						MAmplitude, MLacunarity, MPersistence));


					float minValue = -0.024097;
					float maxValue = 0.024268;
					NoiseValue = ((NoiseValue - minValue) / (maxValue - minValue)) * 255;
					
					FillColor = FColor::White;

					FillColor.R += NoiseValue;
					FillColor.G += NoiseValue;
					FillColor.B += NoiseValue;

					MipData[CurPixelIndex] = FillColor;
				}
			}
		}
	}
}
