// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaminaComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	maxStamina = 100;
	currentStamina = maxStamina;
	staminaRegenRate = 0.3f;
	staminaDecayRate = 0.3f;
	tickRate = 0.01f;
	delayTime = 1.0f;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UStaminaComponent::StartStaminaRegen(){
	if (statusTags.HasTag(Tags::Exhausted)){
		GetWorld()->GetTimerManager().SetTimer( StaminaRegenTimerHandle,	this, &UStaminaComponent::ExhaustedRegenStamina, tickRate, true);
	}else{
		GetWorld()->GetTimerManager().SetTimer( StaminaRegenTimerHandle,	this, &UStaminaComponent::RegenStamina, tickRate, true);
	}	
}

void UStaminaComponent::DelayedStaminaRegen(float delay){
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UStaminaComponent::StartStaminaRegen, tickRate, false, delay);
}

void UStaminaComponent::StartRunning(){

	currentStamina -= 5;
	StartStaminaDecay();
}

void UStaminaComponent::StopStaminaRegen(){
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
}

void UStaminaComponent::StartStaminaDecay(){
	StopStaminaRegen();
	GetWorld()->GetTimerManager().SetTimer(StaminaDecayTimerHandle, this, &UStaminaComponent::DecayStamina, tickRate, true);
}

void UStaminaComponent::StopStaminaDecay(){
	GetWorld()->GetTimerManager().ClearTimer(StaminaDecayTimerHandle);
	DelayedStaminaRegen(delayTime);
}

void UStaminaComponent::DecreaseStamina(float amount){
	currentStamina -= amount;
	if (currentStamina < 0)
	{
		currentStamina = 0;
	}
}

void UStaminaComponent::RegenStamina()
{
	currentStamina += staminaRegenRate;
	
	if (currentStamina > maxStamina)
	{
		currentStamina = maxStamina;
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}

void UStaminaComponent::ExhaustedRegenStamina(){
	currentStamina += staminaRegenRate / 2;

	if (currentStamina > maxStamina)
	{
		statusTags.RemoveTag(Tags::Exhausted);
		currentStamina = maxStamina;
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}

void UStaminaComponent::DecayStamina()
{
	currentStamina -= staminaDecayRate;

	if (currentStamina < 0)
	{
		currentStamina = 0;
		StopStaminaDecay();
		statusTags.AddTag(Tags::Exhausted);
		DelayedStaminaRegen(delayTime);
	}
}

float UStaminaComponent::GetStaminaPercentage(){
	return currentStamina / maxStamina;
}

