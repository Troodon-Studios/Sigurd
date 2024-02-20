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


void UCombatComponent::AddWeaponToInventory(FDataTableRowHandle Weapon){
	FItemData ItemData = *Weapon.GetRow<FItemData>(FString::Printf(TEXT("%s"), *Weapon.RowName.ToString()));
	WeaponInventory.Add(ItemData);
	
}

void UCombatComponent::NextWeapon(){
	CurrentWeapon = (CurrentWeapon + 1) % WeaponInventory.Num();
}

void UCombatComponent::PreviousWeapon(){
	CurrentWeapon = (CurrentWeapon - 1 + WeaponInventory.Num()) % WeaponInventory.Num();
}

void UCombatComponent::ExecuteCurrentWeaponComboMontage(FName SectionName)
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
					if (SectionName != NAME_None){
						AnimInstance->Montage_JumpToSection(SectionName, CurrentWeaponMontage);
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

void UCombatComponent::QueueAttack(FName SectionName){
	if (CombatState == ECombatState::AttackQueued){
		CombatState = ECombatState::Attacking;
		ExecuteCurrentWeaponComboMontage(SectionName);
	}	else{
		ComboCount = 0;
		CombatState = ECombatState::Idle;
	}


}

//TODO break block and stun character if stamina is 0
//TODO break into different functions

void UCombatComponent::TakeDamage(float Damage, UObject* DamageType){
	switch (CombatState){
	case ECombatState::Blocking:
		StaminaComponent->DecreaseStamina(Damage);
		break;
	case ECombatState::Dodging:
		break;
	case ECombatState::Parrying:
		break;
	default:
		HealthComponent->TakeDamageWithType(DamageType, Damage);
		
		break;
	}
	
}


void UCombatComponent::IncreaseComboCount(){
	ComboCount = (ComboCount + 1) % WeaponInventory[CurrentWeapon].MaxComboCount;
}

void UCombatComponent::ChangeWeaponLight(float Intensity){

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		UStaticMeshComponent* OwnerMeshComponent = OwnerActor->FindComponentByClass<UStaticMeshComponent>();
		if (OwnerMeshComponent)
		{
			UMaterialInstanceDynamic* OwnerMaterial = OwnerMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
			if (OwnerMaterial)
			{
				OwnerMaterial->SetScalarParameterValue("lum", Intensity);
			}
		}
	}
}

void UCombatComponent::ChangeWeaponLightColor(FLinearColor Color){
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		UStaticMeshComponent* OwnerMeshComponent = OwnerActor->FindComponentByClass<UStaticMeshComponent>();
		if (OwnerMeshComponent)
		{
			UMaterialInstanceDynamic* OwnerMaterial = OwnerMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
			if (OwnerMaterial)
			{
				OwnerMaterial->SetVectorParameterValue("col", Color);
			}
		}
	}
}








