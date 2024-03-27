// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "UObject/Object.h"
#include "AxeLightAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SIGURD_API UAxeLightAttack : public UCombatAbility{

	GENERATED_BODY()

public:
	UAxeLightAttack(){
		Description = "This is a axe light attack";
	}

	virtual void Execute(FName SectionName) override{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Description);
		PlayAnimation(Montage, Owner);
	}
	
};
