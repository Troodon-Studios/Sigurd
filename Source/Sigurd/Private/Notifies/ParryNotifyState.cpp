// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/ParryNotifyState.h"

void UParryNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
									 float TotalDuration, const FAnimNotifyEventReference& EventReference){
	if (UCombatComponent* CombatComp = GetCombatComponent(MeshComp)){
		CombatComp->CombatState = ECombatState::Parrying;
	}


}


void UParryNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference){
	if (UCombatComponent* CombatComp = GetCombatComponent(MeshComp)){
		CombatComp->CombatState = ECombatState::Blocking;
	}

}

UCombatComponent* UParryNotifyState::GetCombatComponent(const USkeletalMeshComponent* MeshComp){
	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (UCombatComponent* CombatComp = Owner->FindComponentByClass<UCombatComponent>())
		{
			return CombatComp;
		}
	}
	return nullptr;
}
