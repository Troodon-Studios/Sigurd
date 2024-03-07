// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_ReserveTokens.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/TokenComponent.h"
#include "Enemies/AICBaseEnemy.h"

UBTTask_ReserveTokens::UBTTask_ReserveTokens(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Reserve Tokens");
}

EBTNodeResult::Type UBTTask_ReserveTokens::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    // Get the AttackTarget from the blackboard
    const AActor* AttackTarget = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
    if (!AttackTarget)
    {
        return EBTNodeResult::Failed;
    }

    // Get the TokensComponent from the AttackTarget
    UTokenComponent* AttackTargetTokensComponent = AttackTarget->FindComponentByClass<UTokenComponent>();
    if (!AttackTargetTokensComponent)
    {
        return EBTNodeResult::Failed;
    }

    // Reserve tokens
    const bool bGiven = AttackTargetTokensComponent->ReserveTokens(PawnTokensComponent ,TokenQuantity);

    const EEnemyState NewState = bGiven ? EEnemyState::Combat : EEnemyState::InQueue;
    if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn(); AAICBaseEnemy* AICont = Cast<AAICBaseEnemy>(ControlledPawn))
    {
        AICont->SetStateAs(NewState);
    }
    
    return EBTNodeResult::Succeeded;
}
