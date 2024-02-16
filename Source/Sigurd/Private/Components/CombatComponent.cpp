// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();	
}


void UCombatComponent::AddWeaponToInventory(FDataTableRowHandle weapon){
	FItemData ItemData = *weapon.GetRow<FItemData>(FString::Printf(TEXT("%s"), *weapon.RowName.ToString()));
	weaponInventory.Add(ItemData);
	
}

void UCombatComponent::NextWeapon(){
	currentWeapon = (currentWeapon + 1) % weaponInventory.Num();
}

void UCombatComponent::PreviousWeapon(){
	currentWeapon = (currentWeapon - 1 + weaponInventory.Num()) % weaponInventory.Num();
}

void UCombatComponent::ExecuteCurrentWeaponComboMontage(FName _sectionName)
{
	if (currentWeapon < weaponInventory.Num())
	{
		UAnimMontage* CurrentWeaponMontage = weaponInventory[currentWeapon].ComboMontage;

		if (CurrentWeaponMontage)
		{
			ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
			
			if (OwnerCharacter)
			{
				UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
				
				if (AnimInstance)
				{
					if (AnimInstance->Montage_IsPlaying(weaponInventory[currentWeapon].ComboMontage))
					{
						AnimInstance->Montage_Stop(0.25f, CurrentWeaponMontage);
					}
					
					AnimInstance->Montage_Play(CurrentWeaponMontage, 2.5);
					if (_sectionName != NAME_None){
						AnimInstance->Montage_JumpToSection(_sectionName, CurrentWeaponMontage);
					}

					increaseComboCount();					
				}
			}
		}
	}
}

void UCombatComponent::ExecuteCurrentWeaponDodgeMontage(){

	UAnimMontage* CurrentWeaponMontage = weaponInventory[currentWeapon].DodgeMontage;

	if (CurrentWeaponMontage)
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		
		if (OwnerCharacter)
		{
			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
			
			if (AnimInstance)
			{
				if (AnimInstance->Montage_IsPlaying(weaponInventory[currentWeapon].DodgeMontage))
				{
					AnimInstance->Montage_Stop(0.25f, CurrentWeaponMontage);
				}
				
				AnimInstance->Montage_Play(CurrentWeaponMontage, 1.5);
			}
		}
		
	}
	
}

void UCombatComponent::ExecuteCurrentWeaponBlockMontage(){
	UAnimMontage* CurrentWeaponMontage = weaponInventory[currentWeapon].BlockMontage;

	if (CurrentWeaponMontage)
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		
		if (OwnerCharacter)
		{
			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
			
			if (AnimInstance)
			{
				if (AnimInstance->Montage_IsPlaying(weaponInventory[currentWeapon].BlockMontage))
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
		changeWeaponLight(1);
		changeWeaponLightColor(FLinearColor(0, 1, 0, 1));		
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
		comboCount = 0;
		CombatState = ECombatState::Idle;
	}


}

void UCombatComponent::TakeDamage(){
	switch (CombatState){
	case ECombatState::Blocking:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Blocked"));
		break;
	case ECombatState::Dodging:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Dodged"));
		break;
	case ECombatState::Parrying:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Parried"));
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Took Damage"));
		break;
	}
	
}


void UCombatComponent::increaseComboCount(){
	comboCount = (comboCount + 1) % weaponInventory[currentWeapon].MaxComboCount;
}

void UCombatComponent::changeWeaponLight(float intensity){

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

void UCombatComponent::changeWeaponLightColor(FLinearColor color){
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








