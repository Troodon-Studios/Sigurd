// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Enums/EnemyState.h"
#include "BTTask_SetState.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTTask_SetState : public UBTTask_BlackboardBase
{

public:
	explicit UBTTask_SetState(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	EEnemyState NewState;
	
	
private:
	GENERATED_BODY()
};
