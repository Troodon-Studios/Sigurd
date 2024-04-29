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

private:
	UPROPERTY()
	ABaseCharacter* Owner;

public:

	virtual void BeginPlay() override;

	UCombatComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	ECombatState CombatState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAttackState AttackState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FDataTableRowHandle DefaultRightWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FDataTableRowHandle DefaultLeftWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	AWeapon* RightHandWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	AWeapon* LeftHandWeapon;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(UObject* DamageType, float Damage);


	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(FDataTableRowHandle Weapon);

	void ActivateAbility(EAttackState Ability);

	void StartLightAbility();
	void StartHeavyAbility();

	void StartFirstAbility();
	void StartSecondAbility();
	void StartThirdAbility();
	void StartFourthAbility();

	void EndAbility();

	void StartDodge();
	void StartParry();
	void StartBlock();
	
	void BlockAbility();
	void DodgeAbility();
	
	void AbilityController(UCombatAbility* Ability, FName SectionName = NAME_None);

	void ChangeWeaponLight(float Intensity);
	void ChangeWeaponLightColor(FLinearColor Color);

	void ProcessAttack(FName SectionName = NAME_None);
	void ProcessChain(FName SectionName = NAME_None);



private:
	void InitializeWeapon(AWeapon*& Weapon, FDataTableRowHandle WeaponRow);

};
