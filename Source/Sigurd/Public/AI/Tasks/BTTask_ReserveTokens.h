// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ReserveTokens.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTTask_ReserveTokens : public UBTTask_BlackboardBase
{
public:
	explicit UBTTask_ReserveTokens(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	int32 TokenQuantity = 1;
	
private:
	GENERATED_BODY()
};
