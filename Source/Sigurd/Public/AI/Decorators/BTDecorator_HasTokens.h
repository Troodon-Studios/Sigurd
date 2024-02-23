// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HasTokens.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTDecorator_HasTokens : public UBTDecorator
{

public:

	explicit UBTDecorator_HasTokens(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	int32 TokenQuantity = 1;
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
private:
	GENERATED_BODY()
};
