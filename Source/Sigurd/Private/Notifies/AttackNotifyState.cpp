// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AttackNotifyState.h"




void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference){
	if (UCombatComponent* CombatComp = GetCombatComponent(MeshComp)){
		// CombatComp->CombatState = ECombatState::QueuingAttack;
		// CombatComp->ChangeWeaponLightColor(FLinearColor(0, 0, 1, 1));
		// CombatComp->ChangeWeaponLight(1);
	}


}


void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference){
	if (UCombatComponent* CombatComp = GetCombatComponent(MeshComp)){
		// CombatComp->QueueAttack(sectionName);
		// CombatComp->ChangeWeaponLight(0);
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
