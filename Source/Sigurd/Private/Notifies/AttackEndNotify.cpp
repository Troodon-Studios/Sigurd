// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AttackEndNotify.h"

#include "Characters/BaseCharacter.h"


void UAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference){

	if (UCombatComponent* CombatComp = GetCombatComponent(MeshComp)){
		CombatComp->CombatState = ECombatState::Idle;
		CombatComp->AttackState = EAttackState::Idle;
		CombatComp->ChangeWeaponLight(0);
	}
}

UCombatComponent* UAttackEndNotify::GetCombatComponent(const USkeletalMeshComponent* MeshComp){

	if (const ABaseCharacter* Owner = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		if (UCombatComponent* CombatComp = Owner->GetCombatComponent())
		{
			Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			return CombatComp;
		}
	}

	
	// if (const AActor* Owner = MeshComp->GetOwner())
	// {
	// 	if (UCombatComponent* CombatComp = Owner->FindComponentByClass<UCombatComponent>())
	// 	{
	// 		return CombatComp;
	// 	}
	// }
	return nullptr;
}