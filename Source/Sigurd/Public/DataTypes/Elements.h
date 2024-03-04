#pragma once

#include "CoreMinimal.h"
#include "Elements.generated.h"

UENUM(BlueprintType)
enum class Element : uint8 {
	Fire,
	Air,
	Earth,
	Water,
	Steam,
};

UENUM(BlueprintType)
enum class Reaction : uint8 {
	Evaporate,
};

UENUM(BlueprintType)
enum class Condition : uint8{
	Burning,
	Stunned,
	Slowed,
};
