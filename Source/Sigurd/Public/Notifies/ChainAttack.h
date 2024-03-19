// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Components/CombatComponent.h"
#include "ChainAttack.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UChainAttack : public UAnimNotifyState{
	GENERATED_BODY()

private:
	UCombatComponent* CombatComponent;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FName sectionName;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	UCombatComponent* GetCombatComponent(USkeletalMeshComponent* MeshComp);
	
};
