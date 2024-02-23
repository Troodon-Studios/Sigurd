#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Passive,
	Combat,
	InQueue,
	Frozen,
	Investigating,
	Seeking,
	Dead
};
