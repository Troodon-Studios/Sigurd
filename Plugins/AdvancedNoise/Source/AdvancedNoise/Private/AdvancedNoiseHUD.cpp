// Fill out your copyright notice in the Description page of Project Settings.

#include "AdvancedNoiseHUD.h"

#include "AdvancedNoiseSettings.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/SinglePropertyView.h"
#include "Components/SpinBox.h"
#include "Texture/TextureGen.h"
#include "UObject/SavePackage.h"



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

    if (RandomizeSeedCheck)
    {
        RandomizeSeedCheck->OnCheckStateChanged.AddDynamic(this, &UAdvancedNoiseHUD::OnRandomizeSeedCheckChanged);
    }

    if (NoiseTypeCombobox)
    {
        NoiseTypeCombobox->OnSelectionChanged.AddDynamic(this, &UAdvancedNoiseHUD::OnNoiseTypeChanged);
    }

}

void UAdvancedNoiseHUD::NativeConstruct()
{
    if (NoiseTypeCombobox)
    {
        NoiseTypeCombobox->AddOption(TEXT("Simplex"));
        NoiseTypeCombobox->AddOption(TEXT("Perlin"));
        NoiseTypeCombobox->AddOption(TEXT("Voronoi"));
    }
    
    if (const UAdvancedNoiseSettings* Settings = GetMutableDefault<UAdvancedNoiseSettings>())
    {
        TextureSizeXSpinBox->SetValue(Settings->TextureSize.X);
        TextureSizeYSpinBox->SetValue(Settings->TextureSize.Y);
        ExportPath.Path = Settings->ExportPath.Path;
        
        FrequencySpinBox->SetValue(Settings->Frequency);
        AmplitudeSpinBox->SetValue(Settings->Amplitude);
        LacunaritySpinBox->SetValue(Settings->Lacunarity);
        PersistenceSpinBox->SetValue(Settings->Persistence);

        NoiseTypeCombobox->SetSelectedIndex(Settings->NoiseType);

        // Initialize SpinBoxes with default values and ranges
        InitializeSpinBox(FrequencySpinBox, 0.0f, 10.0f, Settings->Frequency);
        InitializeSpinBox(AmplitudeSpinBox, 0.0f, 1.0f, Settings->Amplitude);
        InitializeSpinBox(LacunaritySpinBox, 0.0f, 10.0f, Settings->Lacunarity);
        InitializeSpinBox(PersistenceSpinBox, 0.0f, 1.0f, Settings->Persistence);
        InitializeSpinBox(TextureSizeXSpinBox, 0.0f, 5000.0f, Settings->TextureSize.X);
        InitializeSpinBox(TextureSizeYSpinBox, 0.0f, 5000.0f, Settings->TextureSize.Y);
        InitializeSpinBox(CellSizeSpinBox, 0.0f, 5000.0f, Settings->CellSize);
    }
    
    UpdateTable();
}


/**
 * Initializes a SpinBox with specified min, max, and optional default values.
 */
void UAdvancedNoiseHUD::InitializeSpinBox(USpinBox* SpinBox, const float MinValue, const float MaxValue, const float DefaultValue = 0.0f)
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
    
    bIsGenerating = true;
    LoadingBorder->SetVisibility(ESlateVisibility::Visible);
    
    if (RandomizeSeedCheck->IsChecked())
    {
        SeedSpinBox->SetValue(FMath::RandRange(0, 1000));
    }
    
    // Generate noise texture
    const FNoiseParameters NoiseParams = FNoiseParameters(FrequencySpinBox->GetValue(), AmplitudeSpinBox->GetValue(),
                                                          LacunaritySpinBox->GetValue(), PersistenceSpinBox->GetValue(),
                                                          CellSizeSpinBox->GetValue());
    UTextureGen::NewTexture(FVector2D(TextureSizeXSpinBox->GetValue(), TextureSizeYSpinBox->GetValue()),
                            SeedSpinBox->GetValue(), TextureNameEditableText->GetText().ToString() + ".png",
                            ExportPath.Path + "/", NoiseParams, 1, static_cast<ENoiseType>(NoiseTypeCombobox->GetSelectedIndex()));

    bIsGenerating = false;
    LoadingBorder->SetVisibility(ESlateVisibility::Collapsed);

    // Save the settings
    UAdvancedNoiseSettings* Settings = GetMutableDefault<UAdvancedNoiseSettings>();
    if (!Settings) return;
    
    Settings->TextureSize = FVector2f(TextureSizeXSpinBox->GetValue(), TextureSizeYSpinBox->GetValue());
    Settings->ExportPath = ExportPath;
    
    Settings->Frequency = FrequencySpinBox->GetValue();
    Settings->Amplitude = AmplitudeSpinBox->GetValue();
    Settings->Lacunarity = LacunaritySpinBox->GetValue();
    Settings->Persistence = PersistenceSpinBox->GetValue();
    
    Settings->SaveConfig();
}

bool UAdvancedNoiseHUD::SaveToAsset(const UObject* ObjectToSave)
{
    UPackage* Package = ObjectToSave->GetPackage();
    const FString PackageName = Package->GetName();
    const FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
    SaveArgs.SaveFlags = SAVE_NoError;

    if (const bool bSucceeded = UPackage::SavePackage(Package, nullptr, *PackageFileName, SaveArgs); !bSucceeded)
    {
        UE_LOG(LogTemp, Error, TEXT("Package '%s' wasn't saved!"), *PackageName);
        return false;
    }

    UE_LOG(LogTemp, Warning, TEXT("Package '%s' was successfully saved"), *PackageName);
    return true;
}

void UAdvancedNoiseHUD::SaveParameters()
{
    if (!NoiseParametersTable) return;

    const FNoiseParameters NoiseParameters = FNoiseParameters(FrequencySpinBox->GetValue(),
                                                              AmplitudeSpinBox->GetValue(),
                                                              LacunaritySpinBox->GetValue(),
                                                              PersistenceSpinBox->GetValue(),
                                                              CellSizeSpinBox->GetValue());

    NoiseParametersTable->AddRow(FName(*SettingsToSaveNameEditableText->GetText().ToString()), NoiseParameters);
    
    UPackage* Package = NoiseParametersTable->GetOutermost();

    FAssetRegistryModule::AssetCreated(NoiseParametersTable);
    Package->SetDirtyFlag(true);
    SaveToAsset(NoiseParametersTable);
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

void UAdvancedNoiseHUD::OnRandomizeSeedCheckChanged(const bool bIsChecked)
{
    SeedSpinBox->SetIsEnabled(!bIsChecked);
}

void UAdvancedNoiseHUD::OnNoiseTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectedItem.IsEmpty()) return;

    if (SelectedItem == "Simplex")
    {
        CellSizeSpinBox->SetIsEnabled(false);
        AmplitudeSpinBox->SetIsEnabled(true);
        FrequencySpinBox->SetIsEnabled(true);
        LacunaritySpinBox->SetIsEnabled(true);
        PersistenceSpinBox->SetIsEnabled(true);
        return;
    }
    if (SelectedItem == "Perlin")
    {
        CellSizeSpinBox->SetIsEnabled(false);
        AmplitudeSpinBox->SetIsEnabled(true);
        FrequencySpinBox->SetIsEnabled(true);
        LacunaritySpinBox->SetIsEnabled(false);
        PersistenceSpinBox->SetIsEnabled(false);
        return;
    }
    if (SelectedItem == "Voronoi")
    {
        CellSizeSpinBox->SetIsEnabled(true);
        AmplitudeSpinBox->SetIsEnabled(false);
        FrequencySpinBox->SetIsEnabled(false);
        LacunaritySpinBox->SetIsEnabled(false);
        PersistenceSpinBox->SetIsEnabled(false);
        return;
    }
}
