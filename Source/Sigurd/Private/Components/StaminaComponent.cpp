// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaminaComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent(){

	StaminaState = EStaminaState::Idle;
	MaxStamina = 100;
	CurrentStamina = MaxStamina;
	StaminaRegenRate = 0.3f;
	StaminaDecayRate = 0.3f;
	TickRate = 0.01f;
	DelayTime = 1.0f;
	RunningSpeed = 900;
	WalkingSpeed = 700;
	ExhaustedSpeed = 500;
}


// Called when the game starts*
void UStaminaComponent::BeginPlay(){
	Super::BeginPlay();

	CharacterMovement = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();

	// ...
}

void UStaminaComponent::StartStaminaRegen(){
	if (StaminaState == EStaminaState::Exhausted){
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UStaminaComponent::ExhaustedRegenStamina,
		                                       TickRate, true);
	}
	else{
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UStaminaComponent::RegenStamina,
		                                       TickRate, true);
	}
}

void UStaminaComponent::DelayedStaminaRegen(float delay){
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UStaminaComponent::StartStaminaRegen,
	                                       TickRate, false, delay);
}

void UStaminaComponent::RunAction(){
	if (StaminaState == EStaminaState::Idle){
		StaminaState = EStaminaState::Running;
		CurrentStamina -= 5;
		CharacterMovement->MaxWalkSpeed = RunningSpeed;
		StartStaminaDecay();
	}
	else if (StaminaState == EStaminaState::Running){
		StaminaState = EStaminaState::Idle;
		StopRunning();
	}
}

void UStaminaComponent::StopRunning(){
	CharacterMovement->MaxWalkSpeed = (StaminaState == EStaminaState::Exhausted) ? ExhaustedSpeed : WalkingSpeed;
	StopStaminaDecay();
}

void UStaminaComponent::StopStaminaRegen(){
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
}

void UStaminaComponent::StartStaminaDecay(){
	StopStaminaRegen();
	GetWorld()->GetTimerManager().SetTimer(StaminaDecayTimerHandle, this, &UStaminaComponent::DecayStamina, TickRate,
	                                       true);
}

void UStaminaComponent::StopStaminaDecay(){
	GetWorld()->GetTimerManager().ClearTimer(StaminaDecayTimerHandle);
	DelayedStaminaRegen(DelayTime);
}

void UStaminaComponent::DecreaseStamina(float amount){
	StopStaminaRegen();
	CurrentStamina -= amount;
	if (CurrentStamina < 0){
		CurrentStamina = 0;
		CharacterMovement->MaxWalkSpeed = ExhaustedSpeed;
		DelayedStaminaRegen(DelayTime);
	}
}

void UStaminaComponent::RegenStamina(){
	CurrentStamina += StaminaRegenRate;

	if (CurrentStamina > MaxStamina){
		StaminaState = EStaminaState::Idle;
		CurrentStamina = MaxStamina;
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}

void UStaminaComponent::ExhaustedRegenStamina(){
	CurrentStamina += StaminaRegenRate / 2;

	if (CurrentStamina > MaxStamina){
		StaminaState = EStaminaState::Idle;
		CharacterMovement->MaxWalkSpeed = WalkingSpeed;
		CurrentStamina = MaxStamina;
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}

void UStaminaComponent::DecayStamina(){
	CurrentStamina -= StaminaDecayRate;

	if (CurrentStamina < 0){
		CurrentStamina = 0;
		StaminaState = EStaminaState::Exhausted;
		StopRunning();
	} else if ( CharacterMovement->Velocity.IsNearlyZero()){
		StaminaState = EStaminaState::Idle;
		StopRunning();
	}
}

float UStaminaComponent::GetStaminaPercentage(){
	return CurrentStamina / MaxStamina;
}
