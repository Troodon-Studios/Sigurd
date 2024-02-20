#pragma once

#include "CoreMinimal.h"
#include "Enemies/PatrolRoute.h"
#include "UObject/Interface.h"
#include "AICoreInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAICoreInterface : public UInterface {
	GENERATED_BODY()
};

class IAICoreInterface{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AI Core")
	APatrolRoute* GetPatrolRoute();

	//UFUNCTION(BlueprintCallable)
	//void SetMovementSpeed(TEnumAsByte<E_MovementSpeed> Speed, double& SpeedValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AI Core")
	void GetIdealRange(double& AttackRadius, double& DefendRadius);

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AI Core")
	//void Attack(AActor* Target);
	
};