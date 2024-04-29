// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "Components/BoxComponent.h"
#include "DataTypes/CombatState.h"
#include "UObject/Interface.h"
#include "CombatAbility.generated.h"

class ABaseCharacter;

UCLASS(Blueprintable, BlueprintType)
class UCombatAbility : public UObject{
	GENERATED_BODY()

public:
	UCombatAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	ECombatState AbiltyType = ECombatState::Attacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool IsChargedAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (EditCondition = "IsChargedAbility"))
	float HoldTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool Combable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool Chainable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float StaminaCost = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* Montage;
	
	virtual void Initialize(ABaseCharacter* InOwner);
	
	virtual void StartAbility(FName SectionName = NAME_None);
	
	virtual void EndAbility();
	


private:
	FTimerHandle AbilityTimer;

protected:
	ABaseCharacter* Owner;

	virtual void Execute(FName SectionName = NAME_None);

	virtual void OnAnimationEnded(UAnimMontage* InMontage, bool bInterrupted);
	
	virtual void PlayAnimationSection(UAnimMontage* InMontage, FName SectionName, ABaseCharacter* InOwner);

	virtual void StopMontage(UAnimInstance* AnimInstance ,UAnimMontage* InMontage);
	
	
};
