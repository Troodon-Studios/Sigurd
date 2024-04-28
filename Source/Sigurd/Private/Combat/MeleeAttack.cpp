// Fill out your copyright notice in the Description page of Project Settings.

//Bind and unbind is necessary to avoid multiple bindings in the same or different abilities


#include "Combat/MeleeAbility.h"
#include "Characters/BaseCharacter.h"
#include "Combat/MeleeWeapon.h"

// void UMeleeAttack::ExecuteAttack(UBoxComponent* Collider, FName SectionName){
// 	if (WeaponCollider->OnComponentBeginOverlap.IsAlreadyBound(this, &UMeleeAttack::OnWeaponColliderOverlap)){
// 		bSelfInterrupt = true;
// 	}
// 	PlayAttackAnimation(Montage, Owner, SectionName);
// }

void UMeleeAttack::Initialize(ABaseCharacter* InOwner){
	Super::Initialize(InOwner);

	if (!GetOuter()->IsA<AMeleeWeapon>()){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Melee Ability in non melee weapon");
	}
		
	WeaponCollider = Cast<AMeleeWeapon>(GetOuter())->WeaponCollider;
}

void UMeleeAttack::Execute(FName SectionName){
	
	//If already bound means that ability was interrupted with another ability and should not remove overlap
	//If ability is interrupted new ability execute will be called before old animation ends
	if (WeaponCollider->OnComponentBeginOverlap.IsAlreadyBound(this, &UMeleeAttack::OnWeaponColliderOverlap)){
		bShouldRemoveOverlap = false;
	}
	
	Super::Execute(SectionName);
}

void UMeleeAttack::RemoveOverlap(){
	if (WeaponCollider){
		WeaponCollider->OnComponentBeginOverlap.RemoveDynamic(this, &UMeleeAttack::OnWeaponColliderOverlap);
	}
}

void UMeleeAttack::OnAnimationEnded(UAnimMontage* InMontage, bool bInterrupted){
	if (!bShouldRemoveOverlap){
		bShouldRemoveOverlap = true;
		return;
	}
	RemoveOverlap();
}

void UMeleeAttack::OnWeaponColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	//TODO Create map of actors if collide and not in map damage, add to map, if in map, ignore
	if (OtherActor == Cast<AActor>(Owner))
		return;

	if (!WeaponCollider)
		return;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Hit");
}

void UMeleeAttack::PlayAnimationSection(UAnimMontage* InMontage, FName SectionName, ABaseCharacter* InOwner){
	
	if (!WeaponCollider->OnComponentBeginOverlap.IsAlreadyBound(this, &UMeleeAttack::OnWeaponColliderOverlap)){
		WeaponCollider->OnComponentBeginOverlap.AddDynamic(this, &UMeleeAttack::OnWeaponColliderOverlap);
	}

	Super::PlayAnimationSection(InMontage, SectionName, InOwner);
	
	
}

void UMeleeAttack::StopMontage(UAnimInstance* AnimInstance, UAnimMontage* InMontage){
	Super::StopMontage(AnimInstance, InMontage);
	RemoveOverlap();
}
