// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Focus.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTTask_Focus : public UBTTask_BlackboardBase
{

public:
	explicit UBTTask_Focus(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	FBlackboardKeySelector FocusKey;
	
private:
	GENERATED_BODY()
};
