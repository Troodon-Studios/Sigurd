#pragma once

#include "CoreMinimal.h"
#include "CombatState.generated.h"

UENUM()
enum ECombatState : uint8 {
	Idle,
	Attacking,
	QueuingAttack,
	AttackQueued,
	Dodging,
	Blocking,
	Parrying,
};