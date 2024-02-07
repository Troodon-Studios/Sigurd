// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AttackNotifyState.h"




void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference){
	if (UCombatComponent* CombatComp = GetCombatComponent(MeshComp)){

	}


}


void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference){
	if (UCombatComponent* CombatComp = GetCombatComponent(MeshComp)){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attack Ended"));
		CombatComp->QueueAttack();
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
