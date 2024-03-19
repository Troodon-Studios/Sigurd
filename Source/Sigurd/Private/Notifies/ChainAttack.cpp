// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/ChainAttack.h"

void UChainAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference){

	CombatComponent = GetCombatComponent(MeshComp);
	
	if(CombatComponent){
		CombatComponent->CombatState = ECombatState::QueuingAttack;
		CombatComponent->ChangeWeaponLight(10);
		CombatComponent->ChangeWeaponLightColor(FLinearColor::Blue);
	}
	
}

void UChainAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference){
	if (CombatComponent){

		CombatComponent->ChainAttack(sectionName);
	}
}

UCombatComponent* UChainAttack::GetCombatComponent(USkeletalMeshComponent* MeshComp){
	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (UCombatComponent* CombatComp = Owner->FindComponentByClass<UCombatComponent>())
		{
			return CombatComp;
		}
	}
	return nullptr;
}
