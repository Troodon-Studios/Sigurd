#pragma once

#include "CoreMinimal.h"
#include "Elements.generated.h"

class UConditionsComponent;
class UReactionHandler;

UENUM(BlueprintType)
enum class Element : uint8 {
	Fire,
	Ice,
	Caos,
	Nature,
};

UENUM(BlueprintType)
enum class ECondition : uint8 {
	Burn,
	Extinguish,
	Freeze,
	Unfreeze,
	Bloom,
	None,
};

struct ConditionEffectInfo{
	FTimerHandle TimerHandle;
	float Duration;
	float Damage;	
};

inline TMap<ECondition, ConditionEffectInfo> ConditionEffectInfos = {
	{ECondition::Burn, ConditionEffectInfo{FTimerHandle(), 5.0f, 5.0f}},
	{ECondition::Extinguish, ConditionEffectInfo{FTimerHandle(), 0.0f, 0.0f}},
	{ECondition::Freeze, ConditionEffectInfo{FTimerHandle(), 5.0f, 0.0f}},
	{ECondition::Unfreeze, ConditionEffectInfo{FTimerHandle(), 0.0f, 0.0f}},
	{ECondition::Bloom, ConditionEffectInfo{FTimerHandle(), 5.0f, 0.0f}},
};


