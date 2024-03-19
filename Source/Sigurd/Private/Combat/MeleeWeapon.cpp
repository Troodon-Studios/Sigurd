// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/MeleeWeapon.h"

#include "Characters/BaseCharacter.h"



// Sets default values
AMeleeWeapon::AMeleeWeapon(){
	WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollider"));
	WeaponCollider->SetupAttachment(WeaponMesh);
	WeaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

void AMeleeWeapon::SetWeaponData(FItemData NewWeaponData, ABaseCharacter* Character){
	Owner = Character;
	WeaponData = NewWeaponData;

	LightAttackAbility = NewObject<UMeleeAttack>(this, WeaponData.LightAttack);
	Cast<UMeleeAttack>(LightAttackAbility)->Initialize(Character, WeaponCollider);
	HeavyAttackAbility = NewObject<UMeleeAttack>(this, WeaponData.HeavyAttack);
	Cast<UMeleeAttack>(HeavyAttackAbility)->Initialize(Character, WeaponCollider);


}

void AMeleeWeapon::LightAttack(FName SectionName){
	if (LightAttackAbility) {
		Cast<UMeleeAttack>(LightAttackAbility)->ExecuteAttack(WeaponCollider, SectionName);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("LightAttackAbility is null."));
	}
}

void AMeleeWeapon::HeavyAttack(FName SectionName){
	Cast<UMeleeAttack>(HeavyAttackAbility)->ExecuteAttack(WeaponCollider, SectionName);
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay(){
	Super::BeginPlay();
	
}

