// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_Focus.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Focus::UBTTask_Focus(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Focus");
}

EBTNodeResult::Type UBTTask_Focus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Check if the pawn implements the AICoreInterface
	if (AAIController* AICont = OwnerComp.GetAIOwner())
	{

		//get actor from blackboard
		if (AActor* FocusActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FocusKey.SelectedKeyName)))
		{
			AICont->SetFocus(FocusActor, EAIFocusPriority::Gameplay);
		}
		// else then get focusKey as a Vector
		else 
		{
			AICont->SetFocalPoint(OwnerComp.GetBlackboardComponent()->GetValueAsVector(FocusKey.SelectedKeyName));
		}

	}

	return EBTNodeResult::Succeeded;
}
