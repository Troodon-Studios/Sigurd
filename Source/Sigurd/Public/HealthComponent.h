// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
		
};
