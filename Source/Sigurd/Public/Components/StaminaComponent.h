// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataTypes/StaminaState.h"
#include "StaminaComponent.generated.h"

//TODO cear caso mantener y caso toggle y cambiar mediante opcion

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGURD_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	EStaminaState StaminaState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDecayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float TickRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float DelayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float WalkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float ExhaustedSpeed;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float GetStaminaPercentage();

	void RunAction();
	void StopRunning();
	void StartStaminaDecay();
	void StopStaminaDecay();

	void DecreaseStamina(float amount);

private:
	FTimerHandle StaminaRegenTimerHandle;
	FTimerHandle StaminaDecayTimerHandle;

	void StopStaminaRegen();
	void DelayedStaminaRegen(float delay);
	void RegenStamina();
	void ExhaustedRegenStamina();
	void StartStaminaRegen();
	void DecayStamina();
	void StartExhaust();
	
protected:
	virtual void BeginPlay() override;


		
};
