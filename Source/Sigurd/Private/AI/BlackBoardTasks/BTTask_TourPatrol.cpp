// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BlackBoardTasks/BTTask_TourPatrol.h"

UBTTask_TourPatrol::UBTTask_TourPatrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("TourPatrol");
}

EBTNodeResult::Type UBTTask_TourPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
    // Check if the pawn implements the AICoreInterface
    if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn(); const IAICoreInterface* AICore = Cast<IAICoreInterface>(ControlledPawn))
    {
        
        if (APatrolRoute* PatrolRoute = AICore->Execute_GetPatrolRoute(ControlledPawn))
        {
            if (FVector NextPatrolPoint; PatrolRoute->GetSplinePointsAsWorldPosition(NextPatrolPoint))
            {
                // set blackboard value to the next patrol point
                OwnerComp.GetBlackboardComponent()->SetValueAsVector("PointOfInterest", NextPatrolPoint);
            }
        }
    }
    return EBTNodeResult::Succeeded;
}
