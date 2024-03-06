#pragma once

#include "CoreMinimal.h"
#include "Elements.generated.h"

class UConditionsComponent;
class UReactionHandler;

UENUM(BlueprintType)
enum class Element : uint8 {
	Fire,
	Water,
	Oil,
	Air,
};

UENUM(BlueprintType)
enum class ECondition : uint8 {
	Burn,
	Extinguish,
	None,
};

struct ConditionEffectInfo{
	ECondition Condition;
	FTimerHandle TimerHandle;
	float Duration;
	float Damage;	
};

inline TArray<ConditionEffectInfo> ConditionEffectInfos = {
	{ECondition::Burn, FTimerHandle(), 5.0f, 5.0f},
	{ECondition::Extinguish, FTimerHandle(), 0.0f, 0.0f}
};


