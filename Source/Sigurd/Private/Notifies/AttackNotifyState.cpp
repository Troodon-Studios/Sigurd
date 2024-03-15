// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AttackNotifyState.h"

#include "Combat/MeleeWeapon.h"


void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference){
	if (UBoxComponent* WeaponCollider = GetWeaponCollider(MeshComp)){
		WeaponCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponCollider->SetHiddenInGame(false);
		
	}


}


void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference){
	if (UBoxComponent* WeaponCollider = GetWeaponCollider(MeshComp)){
		WeaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponCollider->SetHiddenInGame(true);
	}


}

UCombatComponent* UAttackNotifyState::GetCombatComponent(const USkeletalMeshComponent* MeshComp){
	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (UCombatComponent* CombatComp = Owner->FindComponentByClass<UCombatComponent>())
		{
			return CombatComp;
		}
	}
	return nullptr;
}

UBoxComponent* UAttackNotifyState::GetWeaponCollider(const USkeletalMeshComponent* MeshComp){
	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (UCombatComponent* CombatComp = Owner->FindComponentByClass<UCombatComponent>())
		{
			if (CombatComp->EquippedWeapon)
			{				
				return Cast<AMeleeWeapon>(CombatComp->EquippedWeapon)->WeaponCollider;
			}
		}
	}
	return nullptr;
}
