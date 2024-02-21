// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Enums/SpeedState.h"
#include "BTTask_SetSpeed.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTTask_SetSpeed : public UBTTask_BlackboardBase
{
public:
	explicit UBTTask_SetSpeed(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	ESpeedState NewSpeed;

	
private:
	GENERATED_BODY()};
