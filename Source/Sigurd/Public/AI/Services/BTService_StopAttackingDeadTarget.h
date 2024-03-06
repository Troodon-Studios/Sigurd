// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_StopAttackingDeadTarget.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTService_StopAttackingDeadTarget : public UBTService
{
public:
	explicit UBTService_StopAttackingDeadTarget(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	FBlackboardKeySelector TargetKey;
	
private:
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
