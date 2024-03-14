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

	HeavyAttack = NewObject<UCombatAbility>(this, WeaponData.HeavyAttack);
	HeavyAttack->Initialize(Character);
	LightAttack = NewObject<UCombatAbility>(this, WeaponData.LightAttack);
	LightAttack->Initialize(Character);
}
