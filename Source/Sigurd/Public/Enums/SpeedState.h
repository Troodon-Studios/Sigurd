#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESpeedState : uint8
{
	Idle,
	Walking,
	Jogging,
	Sprinting
};
