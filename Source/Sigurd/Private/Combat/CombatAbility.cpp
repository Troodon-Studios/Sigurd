// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatAbility.h"
#include "Characters/BaseCharacter.h"
#include "Sigurd/SigurdCharacter.h"


UCombatAbility::UCombatAbility(){
	Description = "This is a combat ability";
	Montage = nullptr;


}

void UCombatAbility::Initialize(ABaseCharacter* InOwner){
	this->Owner = InOwner;
}

void UCombatAbility::Execute(){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Description);
}

void UCombatAbility::ExecuteSection(FName SectionName){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Description);
	PlayAnimationSection(Montage, SectionName, Owner);
}

// Add default functionality here for any ICombatAbility functions that are not pure virtual.
void UCombatAbility::PlayAnimation(UAnimMontage* Montage, ABaseCharacter* Owner){

	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();

	if (AnimInstance){

		if (AnimInstance->Montage_IsPlaying(Montage)){
			AnimInstance->Montage_Stop(0.2f, Montage);
		}

		AnimInstance->Montage_Play(Montage, 1.5);
	}
	
}

void UCombatAbility::PlayAnimationSection(UAnimMontage* Montage, FName SectionName, ABaseCharacter* Owner){

	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();

	if (AnimInstance){

		if (AnimInstance->Montage_IsPlaying(Montage)){
			AnimInstance->Montage_Stop(0.2f, Montage);
		}

		AnimInstance->Montage_Play(Montage, 1.5); //TODO dejar en 1 
		if (SectionName != NAME_None){
			AnimInstance->Montage_JumpToSection(SectionName, Montage);
		}
	}
}
