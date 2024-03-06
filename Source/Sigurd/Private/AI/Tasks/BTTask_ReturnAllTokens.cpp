// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_ReturnAllTokens.h"

#include "AIController.h"
#include "Components/TokenComponent.h"

UBTTask_ReturnAllTokens::UBTTask_ReturnAllTokens(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Return All Tokens");
}

EBTNodeResult::Type UBTTask_ReturnAllTokens::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller and the pawn
	const AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* Pawn = AIController->GetPawn();

	// Get the TokensComponent from the pawn
	UTokenComponent* PawnTokensComponent = Pawn->FindComponentByClass<UTokenComponent>();
	if (!PawnTokensComponent)
	{
		return EBTNodeResult::Failed;
	}

	PawnTokensComponent->ReturnAllTokens();
    
	return EBTNodeResult::Succeeded;
}
