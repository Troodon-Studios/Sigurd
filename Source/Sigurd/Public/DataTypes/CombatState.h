#pragma once

#include "CoreMinimal.h"
#include "CombatState.generated.h"

UENUM()
enum class ECombatState : uint8 {
	Idle,
	ChargingAttack,
	Attacking,
	QueuingAttack,
	AttackQueued,
	ExecuteQueuedAttack,
	Dodging,
	Blocking,
	Parrying,
};

UENUM()
enum class EAttackState : uint8{
	Idle,
	LightAbility,
	HeavyAbility,
	FirstAbility,
	SecondAbility,
	ThirdAbility,
	FourthAbility,
};
