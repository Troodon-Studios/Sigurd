// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_IsHealthBelowThreshold.h"

#include "AIController.h"
#include "Components/HealthComponent.h"

UBTDecorator_IsHealthBelowThreshold::UBTDecorator_IsHealthBelowThreshold(FObjectInitializer const& ObjectInitializer):
	HealthThreshold(35)
{
	NodeName = TEXT("Is Health Below Threshold?");
}

bool UBTDecorator_IsHealthBelowThreshold::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* OwnerPawn = AIController ? AIController->GetPawn() : nullptr;

	if (UHealthComponent* HealthComponent = OwnerPawn ? OwnerPawn->FindComponentByClass<UHealthComponent>() : nullptr)
	{
		return HealthComponent->GetHealthPercentage() < HealthThreshold/100.0f;
	}

	return false;
}
