#pragma once

#include "CoreMinimal.h"
#include "Components/ReactionHandler.h"
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
	None,
	Burning,
	Stunned,
	Slowed,
};

typedef void (UReactionHandler::*ReactionFunction)();

typedef TMap<TPair<Condition, Element>, ReactionFunction> ReactionMapType;

extern inline ReactionMapType ReactionMap = {
	{TPair<Condition, Element>(Condition::None, Element::Fire), &UReactionHandler::Burn},
	{TPair<Condition, Element>(Condition::None, Element::Water), &UReactionHandler::Wet},
   };

extern ReactionMapType ReactionMap;

