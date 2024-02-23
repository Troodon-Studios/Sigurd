// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_IsWithinIdealRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsWithinIdealRange::UBTDecorator_IsWithinIdealRange(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Is Within Ideal Range?");
}

bool UBTDecorator_IsWithinIdealRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));

    if (const APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn(); TargetActor && OwnerPawn)
    {
        const float IdealRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(IdealRangeKey.SelectedKeyName);
        const float Distance = FVector::Dist(TargetActor->GetActorLocation(), OwnerPawn->GetActorLocation());

        return Distance <= IdealRange;
    }

    return false;
}