// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BlackBoardTasks/BTTask_Heal.h"

#include "AIController.h"
#include "Components/HealthComponent.h"
#include "Interfaces/AICoreInterface.h"

UBTTask_Heal::UBTTask_Heal(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Heal");
	HealAmount = 2.0f;
}

EBTNodeResult::Type UBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Check if the pawn implements the AICoreInterface
	if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn(); ControlledPawn->GetClass()->ImplementsInterface(UAICoreInterface::StaticClass()))
	{

		// get HealthComponent from the pawn
		if (UHealthComponent* HealthComponent = IAICoreInterface::Execute_GetHealthComponent(ControlledPawn))
		{
			// heal the pawn
			HealthComponent->Heal(HealAmount);
		}
		
	}

	return EBTNodeResult::Succeeded;
}
