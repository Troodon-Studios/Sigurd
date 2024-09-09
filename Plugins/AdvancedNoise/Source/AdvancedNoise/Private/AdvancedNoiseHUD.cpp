// Fill out your copyright notice in the Description page of Project Settings.

#include "AdvancedNoiseHUD.h"

#include "AdvancedNoiseSettings.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/SinglePropertyView.h"
#include "Components/SpinBox.h"
#include "Texture/TextureGen.h"

void UAdvancedNoiseHUD::NativePreConstruct()
{
    Super::NativeConstruct();

    if (const UAdvancedNoiseSettings* Settings = GetMutableDefault<UAdvancedNoiseSettings>())
    {
        if (Settings->NoiseParametersTable.IsValid())
        {
            NoiseParametersTable = Cast<UDataTable>(Settings->NoiseParametersTable.TryLoad());
        }
    }
    if (IsValid(NoiseParametersTable))
    {
        if (NoiseParametersTable->RowStruct != FNoiseParameters::StaticStruct())
        {
            UE_LOG(LogTemp, Error, TEXT("NoiseParametersTable is not valid"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NoiseParametersTable is not valid"));
    }
    
    LoadingBorder->SetVisibility(ESlateVisibility::Collapsed);

    // Set default export path if not specified
    if (ExportPath.Path.IsEmpty())
    {
        ExportPath.Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
    }

    // Initialize ExportPathPropertyView
    if (ExportPathPropertyView)
    {
        ExportPathPropertyView->SetObject(this);
        ExportPathPropertyView->SetPropertyName("ExportPath");
    }

    // Initialize ExportPathPropertyView
    if (SaveDatatablePropertyView)
    {
        SaveDatatablePropertyView->SetObject(this);
        SaveDatatablePropertyView->SetPropertyName("NoiseParametersTable");
    }
    
    // Bind GenerateNoiseButton click event
    if (GenerateNoiseButton)
    {
        GenerateNoiseButton->OnClicked.AddDynamic(this, &UAdvancedNoiseHUD::GenerateNoise);
    }

    if (SaveParametersButton)
    {
        SaveParametersButton->OnClicked.AddDynamic(this, &UAdvancedNoiseHUD::SaveParameters);
    }

    if (LoadParametersCombobox)
    {
        LoadParametersCombobox->OnSelectionChanged.AddDynamic(this, &UAdvancedNoiseHUD::LoadParameters);
    }

    
    // Initialize SpinBoxes with default values and ranges
    InitializeSpinBox(FrequencySpinBox, 0.0f, 10.0f, 0.5f);
    InitializeSpinBox(AmplitudeSpinBox, 0.0f, 1.0f, 0.3f);
    InitializeSpinBox(LacunaritySpinBox, 0.0f, 10.0f, 2.0f);
    InitializeSpinBox(PersistenceSpinBox, 0.0f, 1.0f, 0.5f);
    InitializeSpinBox(TextureSizeXSpinBox, 0.0f, 5000.0f, 512);
    InitializeSpinBox(TextureSizeYSpinBox, 0.0f, 5000.0f, 512);
}

/**
 * Initializes a SpinBox with specified min, max, and optional default values.
 */
void UAdvancedNoiseHUD::InitializeSpinBox(USpinBox* SpinBox, float MinValue, float MaxValue, float DefaultValue = 0.0f)
{
    if (SpinBox)
    {
        SpinBox->SetMinValue(MinValue);
        SpinBox->SetMinSliderValue(MinValue);
        SpinBox->SetMaxSliderValue(MaxValue);
        if (DefaultValue != 0.0f)
        {
            SpinBox->SetValue(DefaultValue);
        }
    }
}

void UAdvancedNoiseHUD::GenerateNoise()
{
    if (bIsGenerating) return;

    bIsGenerating = true;
    LoadingBorder->SetVisibility(ESlateVisibility::Visible);
    
    // Validate export path
    if (ExportPath.Path.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No file path specified"));
        return;
    }

    FString FullPath = (ExportPath.Path);
    if (const FString ContentDir = FPaths::ProjectContentDir(); !FPaths::MakePathRelativeTo(FullPath, *ContentDir))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to make path relative to content directory"));
        return;
    }
    FullPath = FullPath + "/";

    // Validate texture name
    if (TextureNameEditableText->GetText().IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No texture name specified"));
        return;
    }

    // Generate noise texture
    const FNoiseParameters NoiseParams = FNoiseParameters(FrequencySpinBox->GetValue(), AmplitudeSpinBox->GetValue(),
                                                          LacunaritySpinBox->GetValue(), PersistenceSpinBox->GetValue());
    FTextureGen::NewTexture(FVector2D(TextureSizeXSpinBox->GetValue(), TextureSizeYSpinBox->GetValue()),
                            0, TextureNameEditableText->GetText().ToString() + ".png", ExportPath.Path + "/", NoiseParams, 1);

    bIsGenerating = false;
    LoadingBorder->SetVisibility(ESlateVisibility::Collapsed);
}

void UAdvancedNoiseHUD::SaveParameters()
{
    if (!NoiseParametersTable) return;
    
    const FNoiseParameters NoiseParameters = FNoiseParameters(FrequencySpinBox->GetValue(),
                                                              AmplitudeSpinBox->GetValue(),
                                                              LacunaritySpinBox->GetValue(),
                                                              PersistenceSpinBox->GetValue());
    NoiseParametersTable->AddRow(FName(*SettingsToSaveNameEditableText->GetText().ToString()), NoiseParameters);
    UpdateTable();
}

void UAdvancedNoiseHUD::LoadParameters(const FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (!NoiseParametersTable) return;

    if (SelectedItem.IsEmpty()) return;

    if (const FNoiseParameters* NoiseParameters = NoiseParametersTable->FindRow<FNoiseParameters>(FName(*SelectedItem), ""))
    {
        FrequencySpinBox->SetValue(NoiseParameters->Frequency);
        AmplitudeSpinBox->SetValue(NoiseParameters->Amplitude);
        LacunaritySpinBox->SetValue(NoiseParameters->Lacunarity);
        PersistenceSpinBox->SetValue(NoiseParameters->Persistence);
    }
}

void UAdvancedNoiseHUD::UpdateTable()
{
    if (!NoiseParametersTable) return;
    
    UAdvancedNoiseSettings* Settings = GetMutableDefault<UAdvancedNoiseSettings>();
    Settings->NoiseParametersTable = NoiseParametersTable;
    Settings->SaveConfig();
        
    LoadParametersCombobox->ClearOptions();
    LoadParametersCombobox->ClearSelection();
    
    for (const auto& Row : NoiseParametersTable->GetRowMap())
    {
        LoadParametersCombobox->AddOption(Row.Key.ToString());
    }
}

void UAdvancedNoiseHUD::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
    if (const FName PropertyName = (Event.Property != nullptr) ? Event.Property->GetFName() : NAME_None; PropertyName ==
        GET_MEMBER_NAME_CHECKED(UAdvancedNoiseHUD, NoiseParametersTable))
    {
        UpdateTable();
    }
    Super::PostEditChangeProperty(Event);
    
}
