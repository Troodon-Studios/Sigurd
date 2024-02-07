// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Sigurd/SigurdCharacter.h"

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

void UCombatComponent::ExecuteCurrentWeaponMontage()
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
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Stop Montage")));
					}
					
					AnimInstance->Montage_Play(CurrentWeaponMontage);
					AnimInstance->Montage_JumpToSection(secuenceMap[comboCount], CurrentWeaponMontage);
					increaseComboCount();					
				}
			}
		}
	}
}

void UCombatComponent::Attack(){
	if (canAttack){
		canAttack = false;
		ExecuteCurrentWeaponMontage();
	}
	else if (canQueueAttack){
		comboQueued = true;
		canQueueAttack = false;
		changeWeaponLight(1);
		changeWeaponLightColor(FLinearColor(0, 1, 0, 1));
	}

	
	

}

void UCombatComponent::QueueAttack(){
	if (comboQueued){
		comboQueued = false;
		ExecuteCurrentWeaponMontage();
	}
	else{
		comboCount = 0;
		canQueueAttack = false;
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








