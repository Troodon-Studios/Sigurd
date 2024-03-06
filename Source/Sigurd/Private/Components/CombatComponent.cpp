// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	CombatState = ECombatState::Idle;
	ComboCount = 0;
	CurrentWeapon = 0;
	
	
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
	StaminaComponent = GetOwner()->FindComponentByClass<UStaminaComponent>();
	
}


void UCombatComponent::AddWeaponToInventory(FDataTableRowHandle weapon){
	FItemData ItemData = *weapon.GetRow<FItemData>(FString::Printf(TEXT("%s"), *weapon.RowName.ToString()));
	WeaponInventory.Add(ItemData);
	
}

void UCombatComponent::NextWeapon(){
	CurrentWeapon = (CurrentWeapon + 1) % WeaponInventory.Num();
}

void UCombatComponent::PreviousWeapon(){
	CurrentWeapon = (CurrentWeapon - 1 + WeaponInventory.Num()) % WeaponInventory.Num();
}

void UCombatComponent::ExecuteCurrentWeaponComboMontage(FName _sectionName)
{
	if (CurrentWeapon < WeaponInventory.Num())
	{
		UAnimMontage* CurrentWeaponMontage = WeaponInventory[CurrentWeapon].ComboMontage;

		if (CurrentWeaponMontage)
		{
			ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
			
			if (OwnerCharacter)
			{
				UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
				
				if (AnimInstance)
				{
					if (AnimInstance->Montage_IsPlaying(WeaponInventory[CurrentWeapon].ComboMontage))
					{
						AnimInstance->Montage_Stop(0.25f, CurrentWeaponMontage);
					}
					
					AnimInstance->Montage_Play(CurrentWeaponMontage, 1.5);
					if (_sectionName != NAME_None){
						AnimInstance->Montage_JumpToSection(_sectionName, CurrentWeaponMontage);
					}

					IncreaseComboCount();					
				}
			}
		}
	}
}

void UCombatComponent::ExecuteCurrentWeaponDodgeMontage(){

	UAnimMontage* CurrentWeaponMontage = WeaponInventory[CurrentWeapon].DodgeMontage;

	if (CurrentWeaponMontage)
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		
		if (OwnerCharacter)
		{
			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
			
			if (AnimInstance)
			{
				if (AnimInstance->Montage_IsPlaying(WeaponInventory[CurrentWeapon].DodgeMontage))
				{
					AnimInstance->Montage_Stop(0.25f, CurrentWeaponMontage);
				}
				
				AnimInstance->Montage_Play(CurrentWeaponMontage, 1.5);
			}
		}
		
	}
	
}

void UCombatComponent::ExecuteCurrentWeaponBlockMontage(){
	UAnimMontage* CurrentWeaponMontage = WeaponInventory[CurrentWeapon].BlockMontage;

	if (CurrentWeaponMontage)
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		
		if (OwnerCharacter)
		{
			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
			
			if (AnimInstance)
			{
				if (AnimInstance->Montage_IsPlaying(WeaponInventory[CurrentWeapon].BlockMontage))
				{
					AnimInstance->Montage_Stop(0.25f, CurrentWeaponMontage);
				}
				
				AnimInstance->Montage_Play(CurrentWeaponMontage, 1.5);
			}
		}
		
	}
}

void UCombatComponent::Dodge(){
	if ( CombatState == ECombatState::Idle){
		CombatState = ECombatState::Dodging;
		ExecuteCurrentWeaponDodgeMontage();		
	}
}

void UCombatComponent::Block(){
	if (CombatState == ECombatState::Idle){
		CombatState = ECombatState::Blocking;
		ExecuteCurrentWeaponBlockMontage();
	}
}

void UCombatComponent::Attack(){

	if (CombatState == ECombatState::Idle){
		CombatState = ECombatState::Attacking;

		
		// Obtén una referencia al personaje y desactiva su capacidad de moverse
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		if (OwnerCharacter)
		{
			OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 150;
		}
		
		ExecuteCurrentWeaponComboMontage(NAME_None);
	}else if (CombatState == ECombatState::QueuingAttack){
		CombatState = ECombatState::AttackQueued;
		ChangeWeaponLight(1);
		ChangeWeaponLightColor(FLinearColor(0, 1, 0, 1));		
	}
}

void UCombatComponent::EndAttack(){
	// Cambia el estado de combate a Idle
	CombatState = ECombatState::Idle;

	// Obtén una referencia al personaje y reactiva su capacidad de moverse
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 800;
	}
}

void UCombatComponent::QueueAttack(FName _sectionName){
	if (CombatState == ECombatState::AttackQueued){
		CombatState = ECombatState::Attacking;
		ExecuteCurrentWeaponComboMontage(_sectionName);
	}	else{
		ComboCount = 0;
		CombatState = ECombatState::Idle;
	}


}

//TODO break block and stun character if stamina is 0
//TODO break into different functions

void UCombatComponent::TakeDamage(float damage, UObject* DamageType){
	switch (CombatState){
	case ECombatState::Blocking:
		StaminaComponent->DecreaseStamina(damage);
		break;
	case ECombatState::Dodging:
		break;
	case ECombatState::Parrying:
		break;
	default:
		HealthComponent->TakeDamageWithType(DamageType, damage);
		
		break;
	}
	
}


void UCombatComponent::IncreaseComboCount(){
	ComboCount = (ComboCount + 1) % WeaponInventory[CurrentWeapon].MaxComboCount;
}

void UCombatComponent::ChangeWeaponLight(float intensity){

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		UStaticMeshComponent* OwnerMeshComponent = OwnerActor->FindComponentByClass<UStaticMeshComponent>();
		if (OwnerMeshComponent)
		{
			UMaterialInstanceDynamic* OwnerMaterial = OwnerMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
			if (OwnerMaterial)
			{
				OwnerMaterial->SetScalarParameterValue("lum", intensity);
			}
		}
	}
}

void UCombatComponent::ChangeWeaponLightColor(FLinearColor color){
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		UStaticMeshComponent* OwnerMeshComponent = OwnerActor->FindComponentByClass<UStaticMeshComponent>();
		if (OwnerMeshComponent)
		{
			UMaterialInstanceDynamic* OwnerMaterial = OwnerMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
			if (OwnerMaterial)
			{
				OwnerMaterial->SetVectorParameterValue("col", color);
			}
		}
	}
}








