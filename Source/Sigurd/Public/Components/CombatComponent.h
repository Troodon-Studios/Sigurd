// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "ItemData.h"
#include "DataTypes/CombatState.h"
#include "StaminaComponent.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGURD_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TEnumAsByte<ECombatState> CombatState;

	int ComboCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemData> WeaponInventory;

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
	
	UStaminaComponent* StaminaComponent;
	UHealthComponent* HealthComponent;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Dodge();
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Block();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();
	void EndAttack();

	void QueueAttack(FName _sectionName);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(float damage, UObject* DamageType);

	void IncreaseComboCount();

	void ChangeWeaponLight(float intensity);
	void ChangeWeaponLightColor(FLinearColor color);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
};
