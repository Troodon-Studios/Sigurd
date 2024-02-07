// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Actor/Weapon.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGURD_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	int comboCount = 0;
	bool canAttack = true;
	bool canQueueAttack = false;
	bool comboQueued = false;
	
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
	void ExecuteCurrentWeaponMontage(FName _sectionName);

	void Attack();

	void QueueAttack(FName _sectionName);

	void increaseComboCount();

	void changeWeaponLight(float intensity);
	void changeWeaponLightColor(FLinearColor color);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
};
