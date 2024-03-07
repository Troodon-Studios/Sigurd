#pragma once

#include "CoreMinimal.h"
#include "Enemies/PatrolRoute.h"
#include "Enums/SpeedState.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
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

 void InitializeSpeedValues(const int32 Idle, const int32 Walking, const int32 Jogging, const int32 Sprinting)
 {
  SpeedValues.Add(ESpeedState::Idle, Idle);
  SpeedValues.Add(ESpeedState::Walking, Walking);
  SpeedValues.Add(ESpeedState::Jogging, Jogging);
  SpeedValues.Add(ESpeedState::Sprinting, Sprinting);
 }


};