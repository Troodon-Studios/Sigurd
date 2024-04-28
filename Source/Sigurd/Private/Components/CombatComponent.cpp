// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Characters/BaseCharacter.h"

void UCombatComponent::BeginPlay(){
	Super::BeginPlay();
	Owner = Cast<ABaseCharacter>(GetOwner());
	
	EquipWeapon(DefaultRightWeapon);
	EquipWeapon(DefaultLeftWeapon);
}

UCombatComponent::UCombatComponent(){
	CombatState = ECombatState::Idle;
	AttackState = EAttackState::Idle;
}

//TODO break block and stun character if stamina is 0
//TODO break into different functions

void UCombatComponent::TakeDamage(UObject* DamageType, float Damage){
	switch (CombatState){
	case ECombatState::Blocking:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Blocking"));
		Owner->GetStaminaComponent()->DecreaseStamina(Damage);
		break;
	case ECombatState::Dodging:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Dodging"));
		break;
	case ECombatState::Parrying:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Parrying"));
		break;
	default:
		Owner->GetHealthComponent()->TakeDamageWithType(DamageType, Damage);
		

		break;
	}
}

void UCombatComponent::EquipWeapon(FDataTableRowHandle Weapon){

	if (Weapon.IsNull()){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Weapon To Equip"));
		return;
	}
	
	FWeaponData WeaponData = *Weapon.GetRow<FWeaponData>(FString::Printf(TEXT("%s"), *Weapon.RowName.ToString()));

	switch (WeaponData.SocketName){
	case ESocket::RH_Socket:
		InitializeWeapon(RightHandWeapon, Weapon);
		break;
	case ESocket::LH_Socket:
		InitializeWeapon(LeftHandWeapon, Weapon);
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Socket"));
		break;
	}
}

void UCombatComponent::ActivateAbility(EAttackState Ability){
	if (RightHandWeapon){
		if (AttackState == EAttackState::Idle){
			AttackState = Ability;
		}
		ProcessAttack();
	}
}

void UCombatComponent::StartLightAbility(){
	ActivateAbility(EAttackState::LightAbility);
}

void UCombatComponent::StartHeavyAbility(){
	ActivateAbility(EAttackState::HeavyAbility);
}

void UCombatComponent::StartFirstAbility(){
	ActivateAbility(EAttackState::FirstAbility);
}

void UCombatComponent::StartSecondAbility(){
	ActivateAbility(EAttackState::SecondAbility);
}

void UCombatComponent::StartThirdAbility(){
	ActivateAbility(EAttackState::ThirdAbility);
}

void UCombatComponent::StartFourthAbility(){
	ActivateAbility(EAttackState::FourthAbility);
}

void UCombatComponent::BlockAbility(){

	LeftHandWeapon->BlockAbility->Execute();
	
}

void UCombatComponent::DodgeAbility(){
	RightHandWeapon->DodgeAbility->Execute();
}

void UCombatComponent::AbilityController(UCombatAbility* Ability, FName SectionName){
	if (Ability != nullptr){
		switch (CombatState){
		case ECombatState::Idle:
			Ability->StartAbility(SectionName);
			break;
		case ECombatState::QueuingAttack:
			if (Ability->Combable || Ability->Chainable){
				CombatState = ECombatState::AttackQueued;
				ChangeWeaponLightColor(FColor::Green);
			}
			break;
		case ECombatState::ExecuteQueuedAttack:
			Ability->StartAbility(SectionName);
			ChangeWeaponLight(0);
			break;
		}
	} else {
		AttackState = EAttackState::Idle;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Ability"));
	}
}

void UCombatComponent::ChangeWeaponLight(float Intensity){
	if (RightHandWeapon->WeaponMesh){
		UMaterialInstanceDynamic* OwnerMaterial = RightHandWeapon->WeaponMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (OwnerMaterial){
			OwnerMaterial->SetScalarParameterValue("EmissiveIntensity", Intensity);
		}
	}
}

void UCombatComponent::ChangeWeaponLightColor(FLinearColor Color){
	if (RightHandWeapon->WeaponMesh){
		UMaterialInstanceDynamic* OwnerMaterial = RightHandWeapon->WeaponMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (OwnerMaterial){
			OwnerMaterial->SetVectorParameterValue("EmissiveColor", Color);
		}
	}
}

void UCombatComponent::ProcessAttack(FName SectionName){
	switch (AttackState){
	case EAttackState::Idle:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cannot Process Attack"));
		break;
	case EAttackState::LightAbility:
		AbilityController(RightHandWeapon->LightAttackAbility, SectionName);
		break;
	case EAttackState::HeavyAbility:
		AbilityController(RightHandWeapon->HeavyAttackAbility, SectionName);
		break;
	case EAttackState::FirstAbility:
		AbilityController(RightHandWeapon->FirstAbility, SectionName);
		break;
	case EAttackState::SecondAbility:
		AbilityController(RightHandWeapon->SecondAbility, SectionName);
		break;
	case EAttackState::ThirdAbility:
		AbilityController(RightHandWeapon->ThirdAbility, SectionName);
		break;
	case EAttackState::FourthAbility:
		AbilityController(RightHandWeapon->FourthAbility, SectionName);
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Attack State"));
		break;
	}
}

void UCombatComponent::ProcessChain(FName SectionName){
	if (CombatState == ECombatState::AttackQueued){
		CombatState = ECombatState::ExecuteQueuedAttack;
		ProcessAttack(SectionName);
	}
}

void UCombatComponent::EndAbility(){
	
	switch (AttackState){
		case EAttackState::LightAbility:
			RightHandWeapon->LightAttackAbility->EndAbility();
			break;
		case EAttackState::HeavyAbility:
			RightHandWeapon->HeavyAttackAbility->EndAbility();
			break;
		case EAttackState::FirstAbility:
			RightHandWeapon->FirstAbility->EndAbility();
			break;
		case EAttackState::SecondAbility:
			RightHandWeapon->SecondAbility->EndAbility();
			break;
		case EAttackState::ThirdAbility:
			RightHandWeapon->ThirdAbility->EndAbility();
			break;
		case EAttackState::FourthAbility:
			RightHandWeapon->FourthAbility->EndAbility();
			break;
		
	}
}

void UCombatComponent::StartDodge(){
	if (!RightHandWeapon->DodgeAbility){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Dodge Ability"));
		return;
	}
	RightHandWeapon->DodgeAbility->StartAbility();
}

void UCombatComponent::StartParry(){
	if (!RightHandWeapon->ParryAbility){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Parry Ability"));
		return;
	}
	RightHandWeapon->ParryAbility->StartAbility();
}

void UCombatComponent::StartBlock(){
	if (!LeftHandWeapon->BlockAbility){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Block Ability"));
		return;
	}
	LeftHandWeapon->BlockAbility->StartAbility();
}

void UCombatComponent::InitializeWeapon(AWeapon*& Weapon, FDataTableRowHandle WeaponRow){
	if (WeaponRow.IsNull()){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Weapon"));
		return;
	}

	if (Weapon){
		Weapon->Destroy();
	}

	FWeaponData WeaponData = *WeaponRow.GetRow<FWeaponData>(FString::Printf(TEXT("%s"), *WeaponRow.RowName.ToString()));
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponData.WeaponClass);
	Weapon->AttachToComponent(Cast<ABaseCharacter>(GetOwner())->GetMesh(),
	                          FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                          GetSocketName(WeaponData.SocketName));
	Weapon->SetWeaponData(WeaponData, Cast<ABaseCharacter>(GetOwner()));
}
