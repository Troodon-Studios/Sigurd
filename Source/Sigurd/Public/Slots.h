#pragma once

#include "CoreMinimal.h"
#include "Slots.generated.h"

USTRUCT(BlueprintType)
struct FSlots : public FTableRowBase{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle Item;
	
};
