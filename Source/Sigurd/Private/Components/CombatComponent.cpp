// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Characters/BaseCharacter.h"

void UCombatComponent::BeginPlay(){
	Super::BeginPlay();
}

UCombatComponent::UCombatComponent(){

}

void UCombatComponent::EquipWeapon(FDataTableRowHandle Weapon){
	
	if (EquippedWeapon){
		EquippedWeapon->Destroy();
	}

	FItemData WeaponData = *Weapon.GetRow<FItemData>(FString::Printf(TEXT("%s"), *Weapon.RowName.ToString()));
	
	EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponData.WeaponClass);
	EquippedWeapon->AttachToComponent(Cast<ABaseCharacter>(GetOwner())->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponData.SocketName);
	EquippedWeapon->SetWeaponData(WeaponData, Cast<ABaseCharacter>(GetOwner()));
}

void UCombatComponent::LightAttack(){
	if (EquippedWeapon){
		EquippedWeapon->LightAttack->Execute();		
	}
}

void UCombatComponent::HeavyAttack(){
	if (EquippedWeapon){
		EquippedWeapon->HeavyAttack->Execute();
	}
}
