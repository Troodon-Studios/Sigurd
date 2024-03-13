// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "UObject/Interface.h"
#include "CombatAbility.generated.h"

class ABaseCharacter;

UCLASS(Blueprintable, BlueprintType)
class UCombatAbility : public UObject{
	GENERATED_BODY()

public:
	UCombatAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* Montage;
	
	void Initialize(ABaseCharacter* InOwner);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Execute();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ExecuteSection(FName SectionName);

protected:
	ABaseCharacter* Owner;
	
	static void PlayAnimation(UAnimMontage* Montage, ABaseCharacter* Owner);
	static void PlayAnimationSection(UAnimMontage* Montage, FName SectionName, ABaseCharacter* Owner);

	
	
};
