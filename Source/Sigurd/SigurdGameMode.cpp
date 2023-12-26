// Copyright Epic Games, Inc. All Rights Reserved.

#include "SigurdGameMode.h"
#include "SigurdCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASigurdGameMode::ASigurdGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
