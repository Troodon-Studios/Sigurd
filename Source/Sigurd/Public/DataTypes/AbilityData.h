#pragma once

#include "CoreMinimal.h"
#include "AbilityData.generated.h"

class UCombatAbility;

USTRUCT(BlueprintType)
struct FAbilityData : public FTableRowBase{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> AbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name = FText::FromString("Ability");

	// TODO: Sustituir por imagen
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color = FLinearColor::White;
	
};