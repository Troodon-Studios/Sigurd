﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearFocus.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTTask_ClearFocus : public UBTTask_BlackboardBase
{
public:
	explicit UBTTask_ClearFocus(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	GENERATED_BODY()
};