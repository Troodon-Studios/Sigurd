// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/CombatComponent.h"
#include "Components/ConditionComponent.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/TokenComponent.h"

#include "BaseCharacter.generated.h"

UCLASS()
class SIGURD_API ABaseCharacter : public ACharacter {
	GENERATED_BODY()

protected:
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	UStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Condition", meta = (AllowPrivateAccess = "true"))
	UConditionComponent* ConditionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UTokenComponent* TokenComponent;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UConditionComponent* GetConditionComponent() const;
	UHealthComponent* GetHealthComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
