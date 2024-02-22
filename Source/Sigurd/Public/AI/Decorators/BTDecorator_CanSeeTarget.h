// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "BTDecorator_CanSeeTarget.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTDecorator_CanSeeTarget : public UBTDecorator_BlackboardBase
{

public:

	explicit UBTDecorator_CanSeeTarget(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	FBlackboardKeySelector TargetKey;
	
private:
	GENERATED_BODY()
};
