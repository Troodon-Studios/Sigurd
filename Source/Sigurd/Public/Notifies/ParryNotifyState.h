// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Components/CombatComponent.h"
#include "ParryNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UParryNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

private:

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	static UCombatComponent* GetCombatComponent(const USkeletalMeshComponent* MeshComp);
};
