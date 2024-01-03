// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGURD_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float maxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float currentHealth;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float damage);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float healAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercentage();
		
};
