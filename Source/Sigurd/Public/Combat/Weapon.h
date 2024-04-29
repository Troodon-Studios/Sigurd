// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "ItemData.h"
#include "MeleeAttack.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SIGURD_API AWeapon : public AActor{
	GENERATED_BODY()

protected:
	ABaseCharacter* Owner;

public:
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FItemData WeaponData;

	UPROPERTY()
	UCombatAbility* LightAttackAbility;
	UPROPERTY()
	UCombatAbility* HeavyAttackAbility;

	virtual void SetWeaponData(FItemData NewWeaponData, ABaseCharacter* Character);

	virtual void LightAttack(FName SectionName);
	virtual void HeavyAttack(FName SectionName);
};
