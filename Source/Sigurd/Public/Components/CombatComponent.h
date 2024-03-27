// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapon.h"
#include "DataTypes/CombatState.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGURD_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UCombatComponent();

	TEnumAsByte<ECombatState> CombatState;

	TEnumAsByte<EAttackState> AttackState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AWeapon> DefaultWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	AWeapon* RightHandWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	AWeapon* LeftHandWeapon;

	// UFUNCTION(BlueprintCallable, Category = "Combat")
	// void TakeDamage(float Damage, UObject* DamageType);


	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(FDataTableRowHandle Weapon);

	void ActivateAbility(EAttackState Ability);

	void LightAttack();
	void HeavyAttack();

	void FirstAbility();
	void SecondAbility();
	void ThirdAbility();
	void FourthAbility();

	void BlockAbility();
	
	void AbilityController(UCombatAbility* Ability, FName SectionName);

	void ChangeWeaponLight(float Intensity);
	void ChangeWeaponLightColor(FLinearColor Color);

	void ProcessAttack(FName SectionName = NAME_None);
	void ProcessChain(FName SectionName = NAME_None);
};
