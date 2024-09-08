// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedNoiseHUD.h"

#include "Noise/NoiseGenerator.h"
#include "Components/Button.h"
#include "Components/SinglePropertyView.h"
#include "Components/SpinBox.h"

void UAdvancedNoiseHUD::NativePreConstruct()
{
	Super::NativeConstruct();

	if (ExportPath.FilePath.IsEmpty())
	{
		ExportPath.FilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	}
	
	if (ExportPathPropertyView)
	{
		ExportPathPropertyView->SetObject(this); 
		ExportPathPropertyView->SetPropertyName("ExportPath");
	}
	
	if (GenerateNoiseButton)
	{
		GenerateNoiseButton->OnClicked.AddDynamic(this, &UAdvancedNoiseHUD::GenerateNoise);
	}

	// SpinBoxes
	if (FrequencySpinBox)
	{
		FrequencySpinBox->SetMinValue(0.0f);
		FrequencySpinBox->SetMinSliderValue(0.0f);
		FrequencySpinBox->SetMaxSliderValue(10.0f);
	}

	if (AmplitudeSpinBox)
	{
		AmplitudeSpinBox->SetMinValue(0.0f);
		AmplitudeSpinBox->SetMinSliderValue(0.0f);
		AmplitudeSpinBox->SetMaxSliderValue(1.0f);
	}

	if (LacunaritySpinBox)
	{
		LacunaritySpinBox->SetMinValue(0.0f);
		LacunaritySpinBox->SetMinSliderValue(0.0f);
		LacunaritySpinBox->SetMaxSliderValue(10.0f);
	}

	if (PersistenceSpinBox)
	{
		PersistenceSpinBox->SetMinValue(0.0f);
		PersistenceSpinBox->SetMinSliderValue(0.0f);
		PersistenceSpinBox->SetMaxSliderValue(1.0f);
	}

	if (TextureSizeXSpinBox)
	{
		TextureSizeXSpinBox->SetValue(512);
		TextureSizeXSpinBox->SetMinValue(0.0f);
		TextureSizeXSpinBox->SetMinSliderValue(0.0f);
		TextureSizeXSpinBox->SetMaxSliderValue(5000.0f);
	}
	
	if (TextureSizeYSpinBox)
	{
		TextureSizeYSpinBox->SetValue(512);
		TextureSizeYSpinBox->SetMinValue(0.0f);
		TextureSizeYSpinBox->SetMinSliderValue(0.0f);
		TextureSizeYSpinBox->SetMaxSliderValue(5000.0f);
	}
}

void UAdvancedNoiseHUD::GenerateNoise()
{
	if (ExportPath.FilePath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No file path specified"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Generating noise from file: %s"), *ExportPath.FilePath);
	// print noise parameters and texture size
	UE_LOG(LogTemp, Warning, TEXT("Frequency: %f"), FrequencySpinBox->GetValue());
	UE_LOG(LogTemp, Warning, TEXT("Amplitude: %f"), AmplitudeSpinBox->GetValue());
	UE_LOG(LogTemp, Warning, TEXT("Lacunarity: %f"), LacunaritySpinBox->GetValue());
	UE_LOG(LogTemp, Warning, TEXT("Persistence: %f"), PersistenceSpinBox->GetValue());
	UE_LOG(LogTemp, Warning, TEXT("Texture Size: (%f,%f)"), TextureSizeXSpinBox->GetValue(),TextureSizeYSpinBox->GetValue());

	//TODO

	float X, Y, Frequency, Amplitude, Lacunarity, Persistence;
	X = 0.0f;
	Y = 0.0f;
	Frequency = 1.0f;
	Amplitude = 1.0f;
	Lacunarity = 2.0f;
	Persistence = 0.5f;
	FNoise::SimplexNoise(X, Y, Frequency, Amplitude, Lacunarity, Persistence);
}
