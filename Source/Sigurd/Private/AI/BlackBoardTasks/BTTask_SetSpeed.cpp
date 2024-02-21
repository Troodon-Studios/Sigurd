// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BlackBoardTasks/BTTask_SetSpeed.h"

#include "AIController.h"
#include "Interfaces/AICoreInterface.h"


UBTTask_SetSpeed::UBTTask_SetSpeed(FObjectInitializer const& ObjectInitializer): NewSpeed(ESpeedState::Idle)
{
	NodeName = TEXT("SetSpeed");
}

EBTNodeResult::Type UBTTask_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
	// Check if the pawn implements the AICoreInterface
	if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn(); const IAICoreInterface* AICore = Cast<IAICoreInterface>(ControlledPawn))
	{
		double Speed = 0;
		AICore->Execute_SetMovementSpeed(ControlledPawn,NewSpeed,Speed);
	}

	return EBTNodeResult::Succeeded;
}
