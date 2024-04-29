// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "..\WeaponData.h"
#include "MeleeAbility.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SIGURD_API AWeapon : public AActor{
	GENERATED_BODY()

protected:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

public:
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FWeaponData WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCombatAbility* LightAttackAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCombatAbility* HeavyAttackAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCombatAbility* FirstAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCombatAbility* SecondAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCombatAbility* ThirdAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCombatAbility* FourthAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCombatAbility* BlockAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCombatAbility* DodgeAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCombatAbility* ParryAbility;

	virtual void SetWeaponData(FWeaponData NewWeaponData, ABaseCharacter* Character);

private:
	void InitializeAbility(UCombatAbility*& CombatAbility, TSubclassOf<UCombatAbility> Ability);
};
