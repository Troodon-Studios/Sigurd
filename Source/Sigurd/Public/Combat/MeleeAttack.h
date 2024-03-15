// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UMeleeAttack : public UCombatAbility{
	GENERATED_BODY()

	bool bSelfInterrupt = false;
private:

	UBoxComponent* WeaponCollider;

public:

	virtual void ExecuteAttack(UBoxComponent* Collider);

	virtual void Initialize(ABaseCharacter* InOwner, UBoxComponent* InCollider);

	void CancelAttack();

	virtual void OnAnimationEnded(UAnimMontage* InMontage, bool bInterrupted) override;

	UFUNCTION()
	void OnWeaponColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayAttackAnimation(UAnimMontage* InMontage, ABaseCharacter* InOwner);
};
