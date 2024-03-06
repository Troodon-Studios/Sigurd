#pragma once

#include "CoreMinimal.h"
#include "StaminaState.generated.h"

UENUM()
enum EStaminaState : uint8 {
	Resting,
	Running,
	Exhausted,
};