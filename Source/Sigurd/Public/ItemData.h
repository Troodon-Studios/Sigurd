#pragma once

#include "CoreMinimal.h"
#include "Combat/CombatAbility.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxComboCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* BlockMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> LightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> HeavyAttack;
	
};