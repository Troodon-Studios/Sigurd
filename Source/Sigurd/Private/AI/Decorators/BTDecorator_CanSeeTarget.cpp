// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_CanSeeTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CanSeeTarget::UBTDecorator_CanSeeTarget(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("CanSeeTarget");
}

bool UBTDecorator_CanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	if (const AAIController* AICont = OwnerComp.GetAIOwner())
	{
		//get actor from blackboard
		if (const AActor* FocusActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName)))
		{
			// check if the actor is in the AI's line of sight
			if (AICont->LineOfSightTo(FocusActor))
			{
				return true;
			}
		}
	}
	return false;
}


