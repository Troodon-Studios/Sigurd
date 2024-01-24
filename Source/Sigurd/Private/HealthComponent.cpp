// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\HealthComponent.h"

#include "DamageData.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	maxHealth = 100;
	currentHealth = maxHealth;
}

void UHealthComponent::TakeDamage(float damage, EDamageType attackerType )
{
	float damageMultiplier = 1.0f;

	if(damageTable)
	{
		FDamageData* row = damageTable->FindRow<FDamageData>(
				   FName(*FString::Printf(TEXT("%d_%d"), static_cast<int32>(attackerType), static_cast<int32>(selfType))), TEXT(""));
		
		if (row)
		{
			damageMultiplier = row->multiplier;
		}
	}

	currentHealth -= damage * damageMultiplier;
	if (currentHealth <= 0)
	{
		// Die
	}
}

void UHealthComponent::Heal(float healAmount)
{
	currentHealth += healAmount;
	if (currentHealth > maxHealth)
	{
		currentHealth = maxHealth;
	}
}

float UHealthComponent::GetHealthPercentage()
{
	return currentHealth / maxHealth;
}



