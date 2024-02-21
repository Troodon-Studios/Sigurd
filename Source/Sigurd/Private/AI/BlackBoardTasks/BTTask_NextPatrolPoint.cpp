// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BlackBoardTasks/BTTask_NextPatrolPoint.h"


UBTTask_NextPatrolPoint::UBTTask_NextPatrolPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("NextPatrolPoint");
}

EBTNodeResult::Type UBTTask_NextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
	// Check if the pawn implements the AICoreInterface
	if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn(); const IAICoreInterface* AICore = Cast<IAICoreInterface>(ControlledPawn))
	{
		// use AI move to to move to the next patrol point
		if (APatrolRoute* PatrolRoute = AICore->Execute_GetPatrolRoute(ControlledPawn))
		{
			PatrolRoute->IncrementActualIndex();
		}
	}

	return EBTNodeResult::Succeeded;
}