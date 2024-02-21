// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BlackBoardTasks/BTTask_ClearFocus.h"

#include "AIController.h"
#include "Interfaces/AICoreInterface.h"

UBTTask_ClearFocus::UBTTask_ClearFocus(FObjectInitializer const& ObjectInitializer)
{

	NodeName = TEXT("ClearFocus");
	
}

EBTNodeResult::Type UBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Check if the pawn implements the AICoreInterface
	if (AAIController* Controller = OwnerComp.GetAIOwner())
	{
		Controller->ClearFocus(EAIFocusPriority::Gameplay);		
	}

	return EBTNodeResult::Succeeded;
}
