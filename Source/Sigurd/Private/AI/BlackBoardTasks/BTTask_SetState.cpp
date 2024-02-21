// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BlackBoardTasks/BTTask_SetState.h"

#include "AIController.h"
#include "Enemies/AICBaseEnemy.h"
#include "Interfaces/AICoreInterface.h"

UBTTask_SetState::UBTTask_SetState(FObjectInitializer const& ObjectInitializer): NewState(EEnemyState::Passive)
{
	NodeName = TEXT("SetState");
}

EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Check if the pawn implements the AICoreInterface
	if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn(); AAICBaseEnemy* AICont = Cast<AAICBaseEnemy>(ControlledPawn))
	{
		AICont->SetStateAs(NewState);
	}

	return EBTNodeResult::Succeeded;
}
