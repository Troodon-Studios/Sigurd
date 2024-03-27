#pragma once

#include "CoreMinimal.h"
#include "CombatState.generated.h"

UENUM()
enum class ECombatState : uint8 {
	Idle,
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
	LightAttack,
	HeavyAttack,
	FirstAbility,
	SecondAbility,
	ThirdAbility,
	FourthAbility,
};
