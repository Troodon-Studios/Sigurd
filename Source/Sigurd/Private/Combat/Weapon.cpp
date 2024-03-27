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
	
	InitializeAbility(LightAttackAbility, WeaponData.LightAttack);
	InitializeAbility(HeavyAttackAbility, WeaponData.HeavyAttack);
	InitializeAbility(FirstAbility, WeaponData.FirstAbility);
	InitializeAbility(SecondAbility, WeaponData.SecondAbility);
	InitializeAbility(ThirdAbility, WeaponData.ThirdAbility);
	InitializeAbility(FourthAbility, WeaponData.FourthAbility);
}

void AWeapon::InitializeAbility(UCombatAbility*& CombatAbility, TSubclassOf<UCombatAbility> Ability){
	CombatAbility = NewObject<UCombatAbility>(this, Ability);
	CombatAbility->Initialize(Owner);
}
