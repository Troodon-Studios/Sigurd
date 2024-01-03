#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "UObject/NoExportTypes.h"
#include "DamageData.generated.h"

USTRUCT(BlueprintType)
struct FDamageData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType damageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType defenderType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float multiplier;
	
};
