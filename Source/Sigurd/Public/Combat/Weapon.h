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

	UCombatAbility* LightAttackAbility;
	UCombatAbility* HeavyAttackAbility;
	UCombatAbility* FirstAbility;
	UCombatAbility* SecondAbility;
	UCombatAbility* ThirdAbility;
	UCombatAbility* FourthAbility;	

	virtual void SetWeaponData(FItemData NewWeaponData, ABaseCharacter* Character);

private:
	void InitializeAbility(UCombatAbility*& CombatAbility, TSubclassOf<UCombatAbility> Ability);
};
