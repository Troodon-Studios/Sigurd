#pragma once

#include "CoreMinimal.h"
#include "Enemies/PatrolRoute.h"
#include "Enums/SpeedState.h"
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AI Core")
	void SetMovementSpeed(ESpeedState Speed, double& SpeedValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AI Core")
	void GetIdealRange(double& AttackRadius, double& DefendRadius);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AI Core")
	void EventAttack(AActor* AttackTarget);
	
protected:

	TMap<ESpeedState, int32> SpeedValues;
	// initialize speed values
	void InitializeSpeedValues()
	{
		SpeedValues.Add(ESpeedState::Idle, 100);
		SpeedValues.Add(ESpeedState::Walking, 200);
		SpeedValues.Add(ESpeedState::Jogging, 300);
		SpeedValues.Add(ESpeedState::Sprinting, 400);
	}
	
};