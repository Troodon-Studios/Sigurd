// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatAbility.h"
#include "Characters/BaseCharacter.h"
#include "Sigurd/SigurdCharacter.h"


UCombatAbility::UCombatAbility(){
	Description = "This is a combat ability";
	Montage = nullptr;


}

void UCombatAbility::StartAbility(FName SectionName){

	if (Owner->GetStaminaComponent()->StaminaState == EStaminaState::Exhausted){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Exhausted");
		Owner->GetCombatComponent()->AttackState = EAttackState::Idle;
		return;
	}
	
	if (IsChargedAbility){		
		Owner->GetCombatComponent()->CombatState = ECombatState::ChargingAttack;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "Execute", SectionName);
		GetWorld()->GetTimerManager().SetTimer(AbilityTimer, Delegate, 1, false, HoldTime);
		
	}
	else{
		Execute(SectionName);
	}



}

void UCombatAbility::EndAbility(){

	if (IsChargedAbility){
		GetWorld()->GetTimerManager().ClearTimer(AbilityTimer);
	
		if (Owner->GetCombatComponent()->CombatState == ECombatState::ChargingAttack || Owner->GetCombatComponent()->CombatState == ECombatState::AttackQueued){
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Ability Canceled");
			Owner->GetCombatComponent()->CombatState = ECombatState::Idle;
			Owner->GetCombatComponent()->AttackState = EAttackState::Idle;
		}
	}

}

void UCombatAbility::Initialize(ABaseCharacter* InOwner){
	this->Owner = InOwner;
}

void UCombatAbility::Execute(FName SectionName){
	Owner->GetCombatComponent()->CombatState = ECombatState::Attacking;

	if (Montage){
		PlayAnimationSection(Montage, SectionName, Owner);
	}
}

// void UCombatAbility::ExecuteSection(FName SectionName){
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Description);
// 	PlayAnimationSection(Montage, SectionName, Owner);
// }

// void UCombatAbility::PlayAnimation(UAnimMontage* InMontage, ABaseCharacter* InOwner){
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayAnimation");
//
// 	if (UAnimInstance* AnimInstance = InOwner->GetMesh()->GetAnimInstance()){
// 		if (AnimInstance->Montage_IsPlaying(NULL)){
// 			AnimInstance->Montage_Stop(0.2f, NULL);
// 		}
//
// 		AnimInstance->Montage_Play(InMontage, 1.5);
//
// 		// Create a variable of type FOnMontageEnded and assign the delegate to it
// 		FOnMontageEnded OnMontageEndedDelegate;
// 		OnMontageEndedDelegate.BindUObject(this, &UCombatAbility::OnAnimationEnded);
//
// 		// Pass the variable to Montage_SetEndDelegate
// 		AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, InMontage);
// 	}
// }

void UCombatAbility::OnAnimationEnded(UAnimMontage* InMontage, bool bInterrupted){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Animation ended");
}

void UCombatAbility::PlayAnimationSection(UAnimMontage* InMontage, FName SectionName, ABaseCharacter* InOwner){
	Owner->GetStaminaComponent()->DecreaseStamina(StaminaCost);
	UAnimInstance* AnimInstance = InOwner->GetMesh()->GetAnimInstance();

	if (!AnimInstance){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No AnimInstance");
		return;
	}

	if (AnimInstance->Montage_IsPlaying(InMontage)){
		StopMontage(AnimInstance, InMontage);
	}

	AnimInstance->Montage_Play(InMontage, 1);
	
	if (SectionName != NAME_None){
		AnimInstance->Montage_JumpToSection(SectionName, InMontage);
	}
	
	// Create a variable of type FOnMontageEnded and assign the delegate to it
	FOnMontageEnded OnMontageEndedDelegate;
	OnMontageEndedDelegate.BindUObject(this, &UCombatAbility::OnAnimationEnded);

	// Pass the variable to Montage_SetEndDelegate
	AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, InMontage);
	
}

void UCombatAbility::StopMontage(UAnimInstance* AnimInstance, UAnimMontage* InMontage){
	AnimInstance->Montage_Stop(0.2f, InMontage);
}
