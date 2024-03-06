// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AIController.h"
#include "Interfaces/AICoreInterface.h"
#include "BTTask_NextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTTask_NextPatrolPoint : public UBTTask_BlackboardBase
{
public:
	explicit UBTTask_NextPatrolPoint(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	GENERATED_BODY()
};
