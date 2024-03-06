#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NoiseSettings.h"
#include "NoiseSettingsTable.generated.h"

USTRUCT(BlueprintType)
struct FNoiseSettingsTable: public FTableRowBase{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle Setting;

};