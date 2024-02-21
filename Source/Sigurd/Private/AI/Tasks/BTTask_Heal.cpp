// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_Heal.h"

#include "AIController.h"
#include "Components/HealthComponent.h"

UBTTask_Heal::UBTTask_Heal(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Heal");
	HealAmount = 2.0f;
}

EBTNodeResult::Type UBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Check if the pawn implements the AICoreInterface
	if (const APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		// get HealthComponent from the pawn
		if (UHealthComponent* HealthComponent = ControlledPawn->FindComponentByClass<UHealthComponent>())
		{
			// heal the pawn
			HealthComponent->Heal(HealAmount);
		}
	}
	return EBTNodeResult::Succeeded;
}
