// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "ResourcesComponent.generated.h"



UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGURD_API UResourcesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourcesComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float maxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float currentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float staminaRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float staminaDecayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	bool canRegenStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	bool canDecayStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	EDamageType selfType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	UDataTable* damageTable;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float damage, EDamageType attackerType);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float healAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercentage();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float GetStaminaPercentage();

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void TakeDamageWithType(UObject* DamageType, float Damage);
	
	void RegenStamina(float deltaTime);

	void DecayStamina(float deltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void StartRunning();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void StopRunning();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void Attack(float attackCost);
};
