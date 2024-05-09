#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NoiseSettingsTable.generated.h"

USTRUCT(BlueprintType)
struct FNoiseSettingsTable: public FTableRowBase{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle Setting;

};

USTRUCT(BlueprintType)
struct FTextureSettingsTable: public FTableRowBase{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle BaseSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle DetailSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle ExtraSettings;

};

USTRUCT(BlueprintType)
struct FMaterialTexturesTable: public FTableRowBase{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle Setting;

};