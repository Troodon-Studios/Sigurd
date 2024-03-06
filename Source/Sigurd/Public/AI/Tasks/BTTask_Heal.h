// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Heal.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTTask_Heal : public UBTTask_BlackboardBase
{
public:

	explicit UBTTask_Heal(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Heal")
	float HealAmount;

private:
	GENERATED_BODY()
};
