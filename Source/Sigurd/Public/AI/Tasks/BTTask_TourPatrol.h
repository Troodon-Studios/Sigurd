// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interfaces/AICoreInterface.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_TourPatrol.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTTask_TourPatrol : public UBTTask_BlackboardBase
{
public:
	
	explicit UBTTask_TourPatrol(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	GENERATED_BODY()
};
