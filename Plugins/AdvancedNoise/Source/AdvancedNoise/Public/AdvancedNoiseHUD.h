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

/** 
 * Button to trigger noise generation 
 * @property GenerateNoiseButton
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class UButton* GenerateNoiseButton;

/** 
 * Button to save noise parameters 
 * @property SaveParametersButton
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class UButton* SaveParametersButton;

/** 
 * Combobox for selecting noise type 
 * @property NoiseTypeCombobox
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class UComboBoxString* NoiseTypeCombobox;

/** 
 * Combobox for loading saved parameters 
 * @property LoadParametersCombobox
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class UComboBoxString* LoadParametersCombobox;

/** 
 * Border displayed while noise is being generated 
 * @property LoadingBorder
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class UBorder* LoadingBorder;

/** 
 * View for selecting the export path 
 * @property ExportPathPropertyView
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class USinglePropertyView* ExportPathPropertyView;

/** 
 * View for selecting the datatable save path 
 * @property SaveDatatablePropertyView
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class USinglePropertyView* SaveDatatablePropertyView;

/** 
 * SpinBox for setting the noise frequency 
 * @property FrequencySpinBox
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class USpinBox* FrequencySpinBox;

/** 
 * SpinBox for setting the noise amplitude 
 * @property AmplitudeSpinBox
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class USpinBox* AmplitudeSpinBox;

/** 
 * SpinBox for setting the noise lacunarity 
 * @property LacunaritySpinBox
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class USpinBox* LacunaritySpinBox;

/** 
 * SpinBox for setting the noise persistence 
 * @property PersistenceSpinBox
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class USpinBox* PersistenceSpinBox;

/** 
 * SpinBox for setting the texture width 
 * @property TextureSizeXSpinBox
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class USpinBox* TextureSizeXSpinBox;

/** 
 * SpinBox for setting the texture height 
 * @property TextureSizeYSpinBox
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class USpinBox* TextureSizeYSpinBox;

/** 
 * Editable text box for setting the texture name 
 * @property TextureNameEditableText
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class UEditableTextBox* TextureNameEditableText;

/** 
 * Editable text box for saving the settings 
 * @property SettingsToSaveNameEditableText
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class UEditableTextBox* SettingsToSaveNameEditableText;

/** 
 * Checkbox for randomizing the seed 
 * @property RandomizeSeedCheck
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class UCheckBox* RandomizeSeedCheck;

/** 
 * SpinBox for setting the seed value 
 * @property SeedSpinBox
 */
UPROPERTY(EditAnywhere, Category = "Noise Generation", meta = (BindWidget))
class USpinBox* SeedSpinBox;

// Properties

/** 
 * Path where the generated texture will be exported 
 * @property ExportPath
 */
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Generation")
FDirectoryPath ExportPath;

/** 
 * Flag indicating whether noise generation is in progress 
 * @property bIsGenerating
 */
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Generation")
bool bIsGenerating = false;

/** 
 * Table containing noise parameters 
 * @property NoiseParametersTable
 */
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Generation")
TObjectPtr<UDataTable> NoiseParametersTable;

private:
/**
 * Called when the randomize seed checkbox state changes.
 * @param bIsChecked - Whether the checkbox is checked.
 */
UFUNCTION()
void OnRandomizeSeedCheckChanged(bool bIsChecked);

/**
 * Called when the noise type is changed in the combobox.
 * @param SelectedItem - The selected noise type.
 * @param SelectionType - The type of selection.
 */
UFUNCTION()
void OnNoiseTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

// Functions

/**
 * Called before the widget is constructed.
 */
virtual void NativePreConstruct() override;

/**
 * Called when the widget is constructed.
 */
virtual void NativeConstruct() override;

/**
 * Generates noise based on the configured parameters.
 */
UFUNCTION(BlueprintCallable, Category = "Noise Generation")
void GenerateNoise();

/**
 * Saves the specified object to an asset.
 * @param ObjectToSave - The object to save.
 * @return True if the asset was saved successfully, false otherwise.
 */
static bool SaveToAsset(const UObject* ObjectToSave);

/**
 * Saves the current noise parameters.
 */
UFUNCTION(BlueprintCallable, Category = "Noise Generation")
void SaveParameters();

/**
 * Loads the selected noise parameters.
 * @param SelectedItem - The selected item from the combobox.
 * @param SelectionType - The type of selection.
 */
UFUNCTION(BlueprintCallable, Category = "Noise Generation")
void LoadParameters(FString SelectedItem, ESelectInfo::Type SelectionType);

/**
 * Updates the noise parameters table.
 */
UFUNCTION(BlueprintCallable, Category = "Noise Generation")
void UpdateTable();

/**
 * Called when a property is edited in the editor.
 * @param Event - The property change event.
 */
virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;

/**
 * Initializes a SpinBox with specified min, max, and optional default values.
 * @param SpinBox - The SpinBox to initialize.
 * @param MinValue - The minimum value for the SpinBox.
 * @param MaxValue - The maximum value for the SpinBox.
 * @param DefaultValue - The default value for the SpinBox.
 */
static void InitializeSpinBox(USpinBox* SpinBox, float MinValue, float MaxValue, float DefaultValue);
};
