// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Decorators/BTDecorator_HasPatrolRoute.h"

UBTDecorator_HasPatrolRoute::UBTDecorator_HasPatrolRoute(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Has Patrol Route?");
}

bool UBTDecorator_HasPatrolRoute::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* OwnerPawn = AIController ? AIController->GetPawn() : nullptr;

    if (UObject* AICoreInterface = Cast<UObject>(OwnerPawn))
    {
        return IAICoreInterface::Execute_GetPatrolRoute(AICoreInterface) ? true : false;
    }

    return false;
}