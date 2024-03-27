#pragma once

#include "CoreMinimal.h"
#include "Combat/CombatAbility.h"
#include "Combat/MeleeAttack.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "ItemData.generated.h"

class AWeapon;

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> LightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> HeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> FirstAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> SecondAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> ThirdAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> FourthAbility;
};