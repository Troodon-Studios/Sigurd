// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGURD_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	private:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
		float MaxHealth;
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
		float CurrentHealth;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		EDamageType SelfType;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		UDataTable* DamageTable;
	
		
		void TakeDamage(float Damage, EDamageType AttackerType);
	
		UFUNCTION(BlueprintCallable, Category = "Health")
		void Heal(float HealAmount);
		UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealthPercentage();
	
	public:
	
		UFUNCTION(BlueprintCallable, Category = "Health")
		void TakeDamageWithType(UObject* DamageType, float Damage);
		
};



