#pragma once

#include "CoreMinimal.h"
#include "StaminaState.generated.h"

UENUM()
enum class EStaminaState : uint8 {
	Idle,
	Running,
	Exhausted,
};