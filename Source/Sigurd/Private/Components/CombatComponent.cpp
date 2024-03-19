// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Characters/BaseCharacter.h"

void UCombatComponent::BeginPlay(){
	Super::BeginPlay();
}

UCombatComponent::UCombatComponent(){
	CombatState = ECombatState::Idle;
}

void UCombatComponent::EquipWeapon(FDataTableRowHandle Weapon){
	if (EquippedWeapon){
		EquippedWeapon->Destroy();
	}

	FItemData WeaponData = *Weapon.GetRow<FItemData>(FString::Printf(TEXT("%s"), *Weapon.RowName.ToString()));

	EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponData.WeaponClass);
	EquippedWeapon->AttachToComponent(Cast<ABaseCharacter>(GetOwner())->GetMesh(),
	                                  FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponData.SocketName);
	EquippedWeapon->SetWeaponData(WeaponData, Cast<ABaseCharacter>(GetOwner()));
}

void UCombatComponent::LightAttack(){
	if (EquippedWeapon){
		if (CombatState == ECombatState::Idle){
			CombatState = ECombatState::Attacking;
			EquippedWeapon->LightAttack(NAME_None);
		}
		else if (CombatState == ECombatState::QueuingAttack){
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Light Attack Queued"));
			CombatState = ECombatState::LightAttackQueued;
			ChangeWeaponLightColor(FLinearColor::Green);
		}
	}
}

void UCombatComponent::HeavyAttack(){
	if (EquippedWeapon){
		if (CombatState == ECombatState::Idle){
			CombatState = ECombatState::Attacking;
			EquippedWeapon->HeavyAttack(NAME_None);
		}
		else if (CombatState == ECombatState::QueuingAttack){
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Heavy Attack Queued"));
			CombatState = ECombatState::HeavyAttackQueued;
			ChangeWeaponLightColor(FLinearColor::Green);
		}
	}
}

void UCombatComponent::ChainAttack(FName SectionName){
	if (EquippedWeapon){
		if (CombatState == ECombatState::LightAttackQueued){
			ChangeWeaponLight(0);
			EquippedWeapon->LightAttack(SectionName);
		}
		else{
			ChangeWeaponLightColor(FLinearColor::Red);
			CombatState = ECombatState::Attacking;
		}
	}
}

void UCombatComponent::ChangeWeaponLight(float Intensity){
	if (EquippedWeapon->WeaponMesh){
		UMaterialInstanceDynamic* OwnerMaterial = EquippedWeapon->WeaponMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (OwnerMaterial){
			OwnerMaterial->SetScalarParameterValue("EmissiveIntensity", Intensity);
		}
	}
}

void UCombatComponent::ChangeWeaponLightColor(FLinearColor Color){
	if (EquippedWeapon->WeaponMesh){
		UMaterialInstanceDynamic* OwnerMaterial = EquippedWeapon->WeaponMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (OwnerMaterial){
			OwnerMaterial->SetVectorParameterValue("EmissiveColor", Color);
		}
	}
}
