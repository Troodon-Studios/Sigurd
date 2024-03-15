// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon.h"

#include "Characters/BaseCharacter.h"


// Sets default values
AWeapon::AWeapon(){
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
}

void AWeapon::SetWeaponData(FItemData NewWeaponData, ABaseCharacter* Character){

	Owner = Character;
	WeaponData = NewWeaponData;

	HeavyAttackAbility = NewObject<UMeleeAttack>(this, WeaponData.HeavyAttack);
	HeavyAttackAbility->Initialize(Character);
	LightAttackAbility = NewObject<UMeleeAttack>(this, WeaponData.LightAttack);
	LightAttackAbility->Initialize(Character);
}

void AWeapon::LightAttack(){
	LightAttackAbility->Execute();
}

void AWeapon::HeavyAttack(){
	HeavyAttackAbility->Execute();
}
