// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Characters/BaseCharacter.h"

void UCombatComponent::BeginPlay() {
	Super::BeginPlay();
}

UCombatComponent::UCombatComponent() {
	CombatState = ECombatState::Idle;
	AttackState = EAttackState::Idle;
}

void UCombatComponent::EquipWeapon(FDataTableRowHandle Weapon) {
	FItemData WeaponData = *Weapon.GetRow<FItemData>(FString::Printf(TEXT("%s"), *Weapon.RowName.ToString()));

	switch (WeaponData.SocketName) {
		case ESocket::RH_Socket:
			if (RightHandWeapon) {
				RightHandWeapon->Destroy();
			}
			RightHandWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponData.WeaponClass);
			RightHandWeapon->AttachToComponent(Cast<ABaseCharacter>(GetOwner())->GetMesh(),
			                                   FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                                   GetSocketName(WeaponData.SocketName));
			RightHandWeapon->SetWeaponData(WeaponData, Cast<ABaseCharacter>(GetOwner()));
			break;
		case ESocket::LH_Socket:
			if (LeftHandWeapon) {
				LeftHandWeapon->Destroy();
			}
			LeftHandWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponData.WeaponClass);
			LeftHandWeapon->AttachToComponent(Cast<ABaseCharacter>(GetOwner())->GetMesh(),
			                                  FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                                  GetSocketName(WeaponData.SocketName));
			LeftHandWeapon->SetWeaponData(WeaponData, Cast<ABaseCharacter>(GetOwner()));
			break;
	}
}

void UCombatComponent::ActivateAbility(EAttackState Ability) {
	if (RightHandWeapon) {
		if (AttackState == EAttackState::Idle) {
			AttackState = Ability;
		}
		ProcessAttack();
	}
}

void UCombatComponent::LightAttack() {
	ActivateAbility(EAttackState::LightAttack);
}

void UCombatComponent::HeavyAttack() {
	ActivateAbility(EAttackState::HeavyAttack);
}

void UCombatComponent::FirstAbility() {
	ActivateAbility(EAttackState::FirstAbility);
}

void UCombatComponent::SecondAbility() {
	ActivateAbility(EAttackState::SecondAbility);
}

void UCombatComponent::ThirdAbility() {
	ActivateAbility(EAttackState::ThirdAbility);
}

void UCombatComponent::FourthAbility() {
	ActivateAbility(EAttackState::FourthAbility);
}

void UCombatComponent::BlockAbility() {
}

void UCombatComponent::AbilityController(UCombatAbility* Ability, FName SectionName) {
	switch (CombatState) {
	case ECombatState::Idle:
		Ability->Execute(SectionName);
		CombatState = ECombatState::Attacking;
		break;
	case ECombatState::QueuingAttack:
		if (Ability->combable || Ability->chainable) {
			CombatState = ECombatState::AttackQueued;
			ChangeWeaponLightColor(FColor::Green);
		}
		break;
	case ECombatState::ExecuteQueuedAttack:
		Ability->Execute(SectionName);
		CombatState = ECombatState::Attacking;
		break;
	}
}

void UCombatComponent::ChangeWeaponLight(float Intensity) {
	if (RightHandWeapon->WeaponMesh) {
		UMaterialInstanceDynamic* OwnerMaterial = RightHandWeapon->WeaponMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (OwnerMaterial) {
			OwnerMaterial->SetScalarParameterValue("EmissiveIntensity", Intensity);
		}
	}
}

void UCombatComponent::ChangeWeaponLightColor(FLinearColor Color) {
	if (RightHandWeapon->WeaponMesh) {
		UMaterialInstanceDynamic* OwnerMaterial = RightHandWeapon->WeaponMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (OwnerMaterial) {
			OwnerMaterial->SetVectorParameterValue("EmissiveColor", Color);
		}
	}
}

void UCombatComponent::ProcessAttack(FName SectionName) {
	switch (AttackState) {
	case EAttackState::Idle:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Idle"));
		break;
	case EAttackState::LightAttack:
		AbilityController(RightHandWeapon->LightAttackAbility, SectionName);
		break;
	case EAttackState::HeavyAttack:
		AbilityController(RightHandWeapon->HeavyAttackAbility, SectionName);
		break;
	case EAttackState::FirstAbility:
		AbilityController(RightHandWeapon->FirstAbility, SectionName);
		AttackState = EAttackState::Idle;
		break;
	case EAttackState::SecondAbility:
		AbilityController(RightHandWeapon->SecondAbility, SectionName);
		AttackState = EAttackState::Idle;
		break;
	case EAttackState::ThirdAbility:
		AbilityController(RightHandWeapon->ThirdAbility, SectionName);
		AttackState = EAttackState::Idle;
		break;
	case EAttackState::FourthAbility:
		AbilityController(RightHandWeapon->FourthAbility, SectionName);
		AttackState = EAttackState::Idle;
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Attack State"));
		break;
	}
}

void UCombatComponent::ProcessChain(FName SectionName) {
	if (CombatState == ECombatState::AttackQueued) {
		CombatState = ECombatState::ExecuteQueuedAttack;
		ProcessAttack(SectionName);
	}
}
