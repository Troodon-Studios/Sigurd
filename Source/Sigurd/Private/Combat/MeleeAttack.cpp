// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/MeleeAttack.h"
#include "Characters/BaseCharacter.h"

void UMeleeAttack::ExecuteAttack(UBoxComponent* Collider){
	if (WeaponCollider->OnComponentBeginOverlap.IsAlreadyBound(this, &UMeleeAttack::OnWeaponColliderOverlap)){
		bSelfInterrupt = true;
	}
	PlayAttackAnimation(Montage, Owner);
}

void UMeleeAttack::Initialize(ABaseCharacter* InOwner, UBoxComponent* InCollider){
	Owner = InOwner;
	WeaponCollider = InCollider;
}

void UMeleeAttack::CancelAttack(){
	if (WeaponCollider){
		WeaponCollider->OnComponentBeginOverlap.RemoveDynamic(this, &UMeleeAttack::OnWeaponColliderOverlap);
	}
}

void UMeleeAttack::OnAnimationEnded(UAnimMontage* InMontage, bool bInterrupted){
	if (bSelfInterrupt){
		bSelfInterrupt = false;
		return;
	}
	CancelAttack();
}

void UMeleeAttack::OnWeaponColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if (OtherActor == Cast<AActor>(Owner))
		return;

	if (!WeaponCollider)
		return;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Description);
}

void UMeleeAttack::PlayAttackAnimation(UAnimMontage* InMontage, ABaseCharacter* InOwner){
	UAnimInstance* AnimInstance = InOwner->GetMesh()->GetAnimInstance();

	if (AnimInstance){
		if (AnimInstance->Montage_IsPlaying(InMontage)){
			AnimInstance->Montage_Stop(0.2f, InMontage);
			CancelAttack();
		}
		
		if (!WeaponCollider->OnComponentBeginOverlap.IsAlreadyBound(this, &UMeleeAttack::OnWeaponColliderOverlap)){
			WeaponCollider->OnComponentBeginOverlap.AddDynamic(this, &UMeleeAttack::OnWeaponColliderOverlap);
		}

		AnimInstance->Montage_Play(InMontage, 1.5);

		// Create a variable of type FOnMontageEnded and assign the delegate to it
		FOnMontageEnded OnMontageEndedDelegate;
		OnMontageEndedDelegate.BindUObject(this, &UCombatAbility::OnAnimationEnded);

		// Pass the variable to Montage_SetEndDelegate
		AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, InMontage);
	}
}
