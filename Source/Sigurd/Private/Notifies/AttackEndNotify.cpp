// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AttackEndNotify.h"



void UAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference){

	if (UCombatComponent* CombatComp = GetCombatComponent(MeshComp)){
		CombatComp->EndAttack();
	}
}

UCombatComponent* UAttackEndNotify::GetCombatComponent(const USkeletalMeshComponent* MeshComp){
	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (UCombatComponent* CombatComp = Owner->FindComponentByClass<UCombatComponent>())
		{
			return CombatComp;
		}
	}
	return nullptr;
}