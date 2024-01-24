// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourcesComponent.h"

#include "DamageData.h"

UResourcesComponent::UResourcesComponent()
{
	maxHealth = 100;
	currentHealth = maxHealth;
}



void UResourcesComponent::TakeDamage(float damage, EDamageType attackerType )
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

void UResourcesComponent::Heal(float healAmount)
{
	currentHealth += healAmount;
	if (currentHealth > maxHealth)
	{
		currentHealth = maxHealth;
	}
}

float UResourcesComponent::GetHealthPercentage()
{
	return currentHealth / maxHealth;
}

float UResourcesComponent::GetStaminaPercentage(){
	return currentStamina / maxStamina;
}

void UResourcesComponent::RegenStamina(float deltaTime){
	currentStamina += staminaRegenRate * deltaTime;
	if (currentStamina > maxStamina)
	{
		currentStamina = maxStamina;
	}
}

void UResourcesComponent::DecayStamina(float deltaTime){
	currentStamina -= staminaDecayRate * deltaTime;
	if (currentStamina < 0)
	{
		currentStamina = 0;
	}
}

