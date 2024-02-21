// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CanSeeTarget.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTDecorator_CanSeeTarget : public UBTDecorator_BlackboardBase
{

public:

	explicit UBTDecorator_CanSeeTarget(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent const& OwnerComp, uint8* NodeMemory) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	FBlackboardKeySelector TargetKey;
	
private:
	GENERATED_BODY()
};
