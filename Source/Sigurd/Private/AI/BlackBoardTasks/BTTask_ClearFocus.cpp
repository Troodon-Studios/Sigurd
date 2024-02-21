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
	if (const APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn(); ControlledPawn->GetClass()->ImplementsInterface(UAICoreInterface::StaticClass()))
	{
		OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);		
	}

	return EBTNodeResult::Succeeded;
}
