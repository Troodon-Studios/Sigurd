#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "AIController.h"
#include "Interfaces/AICoreInterface.h"
#include "BTDecorator_HasPatrolRoute.generated.h"

/**
 * 
 */
UCLASS()
class SIGURD_API UBTDecorator_HasPatrolRoute : public UBTDecorator
{

public:
	explicit UBTDecorator_HasPatrolRoute(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	
private:
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
