// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Actor/Weapon.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UENUM()
enum ECombatState : uint8 {
	Idle,
	Attacking,
	QueuingAttack,
	AttackQueued,
	Dodging,
	Blocking,
	Parrying,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGURD_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TEnumAsByte<ECombatState> CombatState;

	int comboCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int currentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemData> weaponInventory;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddWeaponToInventory(FDataTableRowHandle weapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void NextWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PreviousWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ExecuteCurrentWeaponComboMontage(FName _sectionName);
	void ExecuteCurrentWeaponDodgeMontage();
	void ExecuteCurrentWeaponBlockMontage();

	void Dodge();
	void Block();

	void Attack();
	void EndAttack();

	void QueueAttack(FName _sectionName);

	void TakeDamage();

	void increaseComboCount();

	void changeWeaponLight(float intensity);
	void changeWeaponLightColor(FLinearColor color);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
};
