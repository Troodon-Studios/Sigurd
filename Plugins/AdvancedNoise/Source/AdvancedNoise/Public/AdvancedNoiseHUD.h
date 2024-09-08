// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Noise/NoiseGenerator.h"
#include "AdvancedNoiseHUD.generated.h"
/**
 * 
 */

UCLASS()
class ADVANCEDNOISE_API UAdvancedNoiseHUD : public UEditorUtilityWidget
{

	GENERATED_BODY()

public:

	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	void GenerateNoise();
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GenerateNoiseButton;

	// Property views
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USinglePropertyView* ExportPathPropertyView;
	
	// Noise Parameters
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox * FrequencySpinBox;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox * AmplitudeSpinBox;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox * LacunaritySpinBox;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox * PersistenceSpinBox;

	// Texture Params
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox * TextureSizeXSpinBox;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox * TextureSizeYSpinBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilePath ExportPath;
	
};
