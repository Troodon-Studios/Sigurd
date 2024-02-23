// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsWithinIdealRange.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTDecorator_IsWithinIdealRange : public UBTDecorator
{
public:
	explicit UBTDecorator_IsWithinIdealRange(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	FBlackboardKeySelector IdealRangeKey;

private:
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
