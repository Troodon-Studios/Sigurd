// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tags/Tags.h"
#include "StaminaComponent.generated.h"



UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGURD_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer statusTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float maxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float currentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float staminaRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float staminaDecayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float tickRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float delayTime;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float GetStaminaPercentage();

	void StartRunning();
	void StartStaminaDecay();
	void StopStaminaDecay();

private:
	FTimerHandle StaminaRegenTimerHandle;
	FTimerHandle StaminaDecayTimerHandle;

	void StopStaminaRegen();
	void DelayedStaminaRegen(float delay);
	void RegenStamina();
	void ExhaustedRegenStamina();
	void StartStaminaRegen();
	void DecayStamina();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


		
};
