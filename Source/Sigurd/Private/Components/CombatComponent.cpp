// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Characters/BaseCharacter.h"

void UCombatComponent::BeginPlay(){
	Super::BeginPlay();
}

UCombatComponent::UCombatComponent(){
	CombatState = ECombatState::Idle;
	AttackState = EAttackState::Idle;
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

void UCombatComponent::ActivateAbility(EAttackState Ability){
	if (EquippedWeapon){
		if (AttackState == EAttackState::Idle){
			AttackState = Ability;
		}
		ProcessAttack();
	}
}

void UCombatComponent::LightAttack(){
	ActivateAbility(EAttackState::LightAttack);
}

void UCombatComponent::HeavyAttack(){
	ActivateAbility(EAttackState::HeavyAttack);
}

void UCombatComponent::FirstAbility(){
	ActivateAbility(EAttackState::FirstAbility);
}

void UCombatComponent::SecondAbility(){
	ActivateAbility(EAttackState::SecondAbility);
}

void UCombatComponent::ThirdAbility(){
	ActivateAbility(EAttackState::ThirdAbility);
}

void UCombatComponent::FourthAbility(){
	ActivateAbility(EAttackState::FourthAbility);
}

void UCombatComponent::AbilityController(UCombatAbility* Ability, FName SectionName){
	switch (CombatState){
	case ECombatState::Idle:
		Ability->Execute(SectionName);
		CombatState = ECombatState::Attacking;
		break;
	case ECombatState::QueuingAttack:
		if (Ability->combable || Ability->chainable){
			CombatState = ECombatState::AttackQueued;
			ChangeWeaponLightColor(FColor::Green);
		}
		break;
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



void UCombatComponent::ProcessAttack(FName SectionName){
	switch (AttackState){
	case EAttackState::Idle:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Idle"));
		break;
	case EAttackState::LightAttack:
		AbilityController(EquippedWeapon->LightAttackAbility, SectionName);
		break;
	case EAttackState::HeavyAttack:
		AbilityController(EquippedWeapon->HeavyAttackAbility, SectionName);
		break;
	case EAttackState::FirstAbility:
		AbilityController(EquippedWeapon->FirstAbility, SectionName);
		AttackState = EAttackState::Idle;
		break;
	case EAttackState::SecondAbility:
		AbilityController(EquippedWeapon->SecondAbility, SectionName);		AttackState = EAttackState::Idle;
		break;
	case EAttackState::ThirdAbility:
		AbilityController(EquippedWeapon->ThirdAbility, SectionName);		AttackState = EAttackState::Idle;
		break;
	case EAttackState::FourthAbility:
		AbilityController(EquippedWeapon->FourthAbility, SectionName);		AttackState = EAttackState::Idle;
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Attack State"));
		break;
	}
}

void UCombatComponent::ProcessChain(FName SectionName){
	if (CombatState == ECombatState::AttackQueued){
		switch (AttackState){
		case EAttackState::Idle:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Idle"));
			break;
		case EAttackState::LightAttack:
			EquippedWeapon->LightAttackAbility->Execute(SectionName);
			break;
		case EAttackState::HeavyAttack:
			EquippedWeapon->HeavyAttackAbility->Execute(SectionName);
			break;
		case EAttackState::FirstAbility:
		AbilityController(EquippedWeapon->FirstAbility, SectionName);
			break;
		case EAttackState::SecondAbility:
		AbilityController(EquippedWeapon->SecondAbility, SectionName);
			break;
		case EAttackState::ThirdAbility:
		AbilityController(EquippedWeapon->ThirdAbility, SectionName);
			break;
		case EAttackState::FourthAbility:
		AbilityController(EquippedWeapon->FourthAbility, SectionName);
			break;
		default:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Attack State"));
			break;
		}
		CombatState = ECombatState::Attacking;
		ChangeWeaponLight(0);
	}
	else{
		ChangeWeaponLightColor(FColor::Red);
		
	}
}
