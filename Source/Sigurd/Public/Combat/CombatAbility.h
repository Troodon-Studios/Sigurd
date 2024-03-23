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
	bool combable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool chainable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* Montage;
	
	virtual void Initialize(ABaseCharacter* InOwner);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Execute(FName SectionName = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ExecuteSection(FName SectionName);

	virtual void OnAnimationEnded(UAnimMontage* InMontage, bool bInterrupted);

protected:
	ABaseCharacter* Owner;
	
	void PlayAnimation(UAnimMontage* InMontage, ABaseCharacter* InOwner);
	void PlayAnimationSection(UAnimMontage* InMontage, FName SectionName, ABaseCharacter* InOwner);

	
	
};
