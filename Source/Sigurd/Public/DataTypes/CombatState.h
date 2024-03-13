#pragma once

#include "CoreMinimal.h"
#include "CombatState.generated.h"

UENUM()
enum class ECombatState : uint8 {
	Idle,
	Attacking,
	QueuingAttack,
	AttackQueued,
	LightAttackQueued,
	HeavyAttackQueued,
	Dodging,
	Blocking,
	Parrying,
};