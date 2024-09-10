// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "AdvancedNoiseHUD.generated.h"

class USpinBox;
/**
 * UAdvancedNoiseHUD is a custom editor utility widget for generating noise textures.
 * It provides various UI elements for configuring noise parameters and texture properties.
 */
UCLASS()
class ADVANCEDNOISE_API UAdvancedNoiseHUD : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	
	// Elements
	/** Button to trigger noise generation */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GenerateNoiseButton;

	/** Button to trigger noise generation */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* SaveParametersButton;

	/** Combobox with parameter options */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UComboBoxString* NoiseTypeCombobox;
	
	/** Combobox with parameter options */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UComboBoxString* LoadParametersCombobox;

	/** Border displayed while noise is being generated */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* LoadingBorder;

	/** View for selecting the export path */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USinglePropertyView* ExportPathPropertyView;

	/** View for selecting the export path */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USinglePropertyView* SaveDatatablePropertyView;
	
	/** SpinBox for setting the noise frequency */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox* FrequencySpinBox;

	/** SpinBox for setting the noise amplitude */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox* AmplitudeSpinBox;

	/** SpinBox for setting the noise lacunarity */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox* LacunaritySpinBox;

	/** SpinBox for setting the noise persistence */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox* PersistenceSpinBox;

	/** SpinBox for setting the noise persistence */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox* CellSizeSpinBox;
	
	/** SpinBox for setting the texture width */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox* TextureSizeXSpinBox;

	/** SpinBox for setting the texture height */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox* TextureSizeYSpinBox;

	/** Editable text box for setting the texture name */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* TextureNameEditableText;

	/** Editable text box for save the settings */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* SettingsToSaveNameEditableText;

	/** Editable text box for save the settings */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCheckBox* RandomizeSeedCheck;

	/** Editable text box for save the settings */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USpinBox* SeedSpinBox;
	
	// Properties
	/** Path where the generated texture will be exported */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDirectoryPath ExportPath;

	/** Flag indicating whether noise generation is in progress */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGenerating = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> NoiseParametersTable;

private:

	UFUNCTION()
	void OnRandomizeSeedCheckChanged(bool bIsChecked);

	UFUNCTION()
	void OnNoiseTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	// Functions
	/** Called before the widget is constructed */
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	/** Generates noise based on the configured parameters */
	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	void GenerateNoise();
	static bool SaveToAsset(const UObject* ObjectToSave);

	/** Generates noise based on the configured parameters */
	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	void SaveParameters();

	/** Generates noise based on the configured parameters */
	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	void LoadParameters(FString SelectedItem, ESelectInfo::Type SelectionType);

	/** Generates noise based on the configured parameters */
	UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
	void UpdateTable();
	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
	static void InitializeSpinBox(USpinBox* SpinBox, float MinValue, float MaxValue, float DefaultValue);

};
