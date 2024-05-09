#include "TextureGen.h"

#include <string>

#include "ImageUtils.h"
#include "NoiseGenerator.h"

void FTextureGen::NewTexture(const FVector2D& GridSize, const int Seed, const FString& Name, const FNoiseValues& TextNoiseValues, const float Scatter)
{
	// Crear una textura
	// print generating a new texture and the name
	UE_LOG(LogTemp, Warning, TEXT("Generating a new texture with name: %s"), *Name);
	const int32 Width = GridSize.X * ExtraDim;
	const int32 Height = GridSize.Y * ExtraDim;
	UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

	// Acceder a los datos de la textura
	FColor* MipData = static_cast<FColor*>(Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	GenerateTexture(TextNoiseValues, Seed, GridSize, MipData, Scatter);
	SaveTexture(Texture, Name);
}

void FTextureGen::SetRandomTexture(UMaterialInstanceDynamic* ProceduralMat, TArray<FMaterialTextures> Set, const FString& Type)
{
	const FString Diff = "Diffuse_" + Type;
	const FString ARM = "ARM_" + Type;
	const FString Normal = "Normal_" + Type;
	const FString Scale = "Scale_" + Type;
	const FString Power = "Power_" + Type;
	const float PowerValue = FMath::RandRange(0.5f, 1.5f);

	//print Diff name
	UE_LOG(LogTemp, Warning, TEXT("Diff name: %s"), *FName(Diff).ToString());
	UE_LOG(LogTemp, Warning, TEXT("ARM name: %s"), *FName(ARM).ToString());
	UE_LOG(LogTemp, Warning, TEXT("Normal name: %s"), *FName(Normal).ToString());

	const FMaterialTextures* SelectedSet = &Set[FMath::RandRange(0, Set.Num() - 1)];
	ProceduralMat->SetTextureParameterValue(FName(Diff), SelectedSet->Diffuse);
	ProceduralMat->SetTextureParameterValue(FName(ARM), SelectedSet->ARM);
	ProceduralMat->SetTextureParameterValue(FName(Normal), SelectedSet->Normal);
	ProceduralMat->SetScalarParameterValue(FName(Scale), SelectedSet->Scale);
	ProceduralMat->SetScalarParameterValue(FName(Power), PowerValue);

	UE_LOG(LogTemp, Warning, TEXT("Selected set name: %s"), *SelectedSet->Diffuse->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Power value: %f"), PowerValue);

}

void FTextureGen::SetMaterialTextures(UMaterialInstanceDynamic* ProceduralMat, const UDataTable* TextureSetsDataTable)
{

	auto DTNames = TextureSetsDataTable->GetRowNames();
	TArray<FMaterialTextures> BaseSets, DetailSets, ExtraSets;

	for (const auto DTName : DTNames)
	{
		FString ContextString;
		if (const auto Set = TextureSetsDataTable->FindRow<FMaterialTextures>(DTName, ContextString))
		{
			if (Set->Type == EMaterialTextureType::Base)
			{
				BaseSets.Add(*Set);
			}
			else if (Set->Type == EMaterialTextureType::Detail)
			{
				DetailSets.Add(*Set);
			}
			else if (Set->Type == EMaterialTextureType::Extra)
			{
				ExtraSets.Add(*Set);
			}
			else
			{
				BaseSets.Add(*Set);
				DetailSets.Add(*Set);
				ExtraSets.Add(*Set);
			}
		}
	}

	SetRandomTexture(ProceduralMat, BaseSets, "1");
	SetRandomTexture(ProceduralMat, DetailSets, "2");
	SetRandomTexture(ProceduralMat, ExtraSets, "3");
	
}

void FTextureGen::GenerateTextures(const FVector2D& GridSize, TArray<FTextureSetting*> TextureSettings)
{
	// Recorrer TextureSettings
	for (const FTextureSetting* TextureSetting : TextureSettings)
	{
		NewTexture(GridSize, FMath::RandRange(0, 1000), TextureSetting->Name + "_procText.png", TextureSetting->NoiseValues, TextureSetting->Scatter);
	}
}

void FTextureGen::GenerateTextures(const FVector2D& GridSize, const FTextureSetting& TextureSetting)
{
	NewTexture(GridSize, FMath::RandRange(0, 1000), TextureSetting.Name + "_procText.png", TextureSetting.NoiseValues, TextureSetting.Scatter);
}

void FTextureGen::GenerateTextures(const FVector2D& GridSize, const FTextureSettingsTable& TextureSettingsTable)
{
	
	auto TextureSettings = TextureSettingsTable.BaseSettings.GetRow<FTextureSetting>(FString::Printf(TEXT("%s"), *TextureSettingsTable.BaseSettings.RowName.ToString()));
	NewTexture(GridSize, FMath::RandRange(0, 1000),  "Terrain_procText.png", TextureSettings->NoiseValues, TextureSettings->Scatter);

	TextureSettings = TextureSettingsTable.DetailSettings.GetRow<FTextureSetting>(FString::Printf(TEXT("%s"), *TextureSettingsTable.DetailSettings.RowName.ToString()));
	NewTexture(GridSize, FMath::RandRange(0, 1000), "Detail_procText.png", TextureSettings->NoiseValues, TextureSettings->Scatter);

	TextureSettings = TextureSettingsTable.ExtraSettings.GetRow<FTextureSetting>(FString::Printf(TEXT("%s"), *TextureSettingsTable.ExtraSettings.RowName.ToString()));
	NewTexture(GridSize, FMath::RandRange(0, 1000), "Extra_procText.png", TextureSettings->NoiseValues, TextureSettings->Scatter);
	
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

void FTextureGen::GenerateTexture(const FNoiseValues& TextNoiseValues, const int Seed, const FVector2D& GridSize,FColor* MipData,const float Scatter)
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
