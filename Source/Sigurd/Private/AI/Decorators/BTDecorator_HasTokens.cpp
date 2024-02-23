// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_HasTokens.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/TokenComponent.h"

UBTDecorator_HasTokens::UBTDecorator_HasTokens(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Has Tokens?");
}

bool UBTDecorator_HasTokens::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	// Get the AI controller and the pawn
	const AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* Pawn = AIController->GetPawn();

	// Get the TokensComponent from the pawn
	const UTokenComponent* PawnTokensComponent = Pawn->FindComponentByClass<UTokenComponent>();
	if (!PawnTokensComponent)
	{
		return false;
	}

	// Get the AttackTarget from the blackboard
	const AActor* AttackTarget = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (!AttackTarget)
	{
		return false;
	}

	// Get the TokensComponent from the AttackTarget
	UTokenComponent* AttackTargetTokensComponent = AttackTarget->FindComponentByClass<UTokenComponent>();
	if (!AttackTargetTokensComponent)
	{
		return false;
	}

	// Reserve tokens
	return PawnTokensComponent->GetAviablleTokens(AttackTargetTokensComponent) >= TokenQuantity;
	
}
