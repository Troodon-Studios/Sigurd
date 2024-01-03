// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	maxHealth = 100;
	currentHealth = maxHealth;
}

void UHealthComponent::TakeDamage(float damage)
{
	currentHealth -= damage;
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



