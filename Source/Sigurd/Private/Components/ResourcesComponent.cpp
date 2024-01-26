// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ResourcesComponent.h"

#include "DamageData.h"
#include "Sigurd/DamageTypeInterface.h"

UResourcesComponent::UResourcesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	maxHealth = 100;
	currentHealth = maxHealth;
	maxStamina = 100;
	currentStamina = maxStamina;
	staminaRegenRate = 10;
	staminaDecayRate = 20;
	canDecayStamina = false;
	canRegenStamina = false;
}
void UResourcesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	RegenStamina(DeltaTime);
	DecayStamina(DeltaTime);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

void UResourcesComponent::TakeDamageWithType(UObject* DamageType, float damage) {

	if(DamageType->Implements<UDamageTypeInterface>()) {
	 	EDamageType tempDamageType = IDamageTypeInterface::Execute_ProcessDamage(DamageType);
		TakeDamage(damage, tempDamageType);
	}

	
}

void UResourcesComponent::RegenStamina(float deltaTime){
	if (canRegenStamina && currentStamina < maxStamina){
		currentStamina += staminaRegenRate * deltaTime;
		if (currentStamina > maxStamina)
		{
			currentStamina = maxStamina;
		}
	}
}



void UResourcesComponent::DecayStamina(float deltaTime){
	if (canDecayStamina)
	{
		currentStamina -= staminaDecayRate * deltaTime;
		if (currentStamina < 0)
		{
			currentStamina = 0;
		}
	}
}

void UResourcesComponent::StartRunning(){
	canDecayStamina = true;
	canRegenStamina = false;
}

void UResourcesComponent::StopRunning(){
	canDecayStamina = false;
	canRegenStamina = true;
}

void UResourcesComponent::Attack(float attackCost){


	
}



