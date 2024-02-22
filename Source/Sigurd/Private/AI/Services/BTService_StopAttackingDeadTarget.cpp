// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_StopAttackingDeadTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HealthComponent.h"
#include "Enemies/BaseEnemy.h"

UBTService_StopAttackingDeadTarget::UBTService_StopAttackingDeadTarget(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Stop Attacking if Target is Dead");
	Interval = 1.0f;
	bNotifyBecomeRelevant = true;
}

void UBTService_StopAttackingDeadTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
    if (!TargetActor) return;

    if (UHealthComponent* HealthComponent = TargetActor->FindComponentByClass<UHealthComponent>(); !HealthComponent || HealthComponent->GetHealthPercentage() > 0) return;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return;

    AAICBaseEnemy* AicBaseEnemy = Cast<AAICBaseEnemy>(AIController);
    if (!AicBaseEnemy) return;

    AicBaseEnemy->SetStateAs(EEnemyState::Passive);
}