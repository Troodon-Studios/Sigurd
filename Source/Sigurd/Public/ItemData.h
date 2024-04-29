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
	TSubclassOf<UMeleeAttack> LightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMeleeAttack> HeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;	
};