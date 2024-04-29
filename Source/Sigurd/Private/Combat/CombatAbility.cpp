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

void UCombatAbility::PlayAnimation(UAnimMontage* InMontage, ABaseCharacter* InOwner){
	UAnimInstance* AnimInstance = InOwner->GetMesh()->GetAnimInstance();

	if (AnimInstance){
		if (AnimInstance->Montage_IsPlaying(NULL)){
			AnimInstance->Montage_Stop(0.2f, NULL);
		}

		AnimInstance->Montage_Play(InMontage, 1.5);

		// Create a variable of type FOnMontageEnded and assign the delegate to it
		FOnMontageEnded OnMontageEndedDelegate;
		OnMontageEndedDelegate.BindUObject(this, &UCombatAbility::OnAnimationEnded);

		// Pass the variable to Montage_SetEndDelegate
		AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, InMontage);
	}
}

void UCombatAbility::OnAnimationEnded(UAnimMontage* InMontage, bool bInterrupted){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Animation ended");
}

void UCombatAbility::PlayAnimationSection(UAnimMontage* InMontage, FName SectionName, ABaseCharacter* InOwner){

	UAnimInstance* AnimInstance = InOwner->GetMesh()->GetAnimInstance();

	if (AnimInstance){

		if (AnimInstance->Montage_IsPlaying(InMontage)){
			AnimInstance->Montage_Stop(0.2f, InMontage);
		}

		AnimInstance->Montage_Play(InMontage, 1.5); //TODO dejar en 1 
		if (SectionName != NAME_None){
			AnimInstance->Montage_JumpToSection(SectionName, InMontage);
		}
	}
}
