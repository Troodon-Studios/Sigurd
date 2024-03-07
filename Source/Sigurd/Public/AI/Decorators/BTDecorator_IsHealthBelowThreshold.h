// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsHealthBelowThreshold.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTDecorator_IsHealthBelowThreshold : public UBTDecorator
{
public:
	explicit UBTDecorator_IsHealthBelowThreshold(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	float HealthThreshold;

private:
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
