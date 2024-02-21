// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BlackBoardTasks/BTTask_TourPatrol.h"

UBTTask_TourPatrol::UBTTask_TourPatrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("TourPatrol");
}

EBTNodeResult::Type UBTTask_TourPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
    // Check if the pawn implements the AICoreInterface
    if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn(); ControlledPawn->GetClass()->ImplementsInterface(UAICoreInterface::StaticClass()))
    {
        // Cast the pawn to the IAICoreInterface
        const IAICoreInterface* AICore = Cast<IAICoreInterface>(ControlledPawn);

        // Get the patrol route
        APatrolRoute* PatrolRoute = AICore->Execute_GetPatrolRoute(ControlledPawn);

        // use AI move to to move to the next patrol point
        if (PatrolRoute)
        {
            FVector NextPatrolPoint;
            if (PatrolRoute->GetSplinePointsAsWorldPosition(NextPatrolPoint))
            {
                // set blackboard value to the next patrol point
                OwnerComp.GetBlackboardComponent()->SetValueAsVector("PointOfInterest", NextPatrolPoint);
            }
        }
        
    }

    return EBTNodeResult::Succeeded;
}
