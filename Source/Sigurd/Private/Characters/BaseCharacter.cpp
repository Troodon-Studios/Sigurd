// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"


// Sets default values
ABaseCharacter::ABaseCharacter() {
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	ConditionComponent = CreateDefaultSubobject<UConditionComponent>(TEXT("ConditionComponent"));
	TokenComponent = CreateDefaultSubobject<UTokenComponent>(TEXT("TokenComponent"));
}

UConditionComponent* ABaseCharacter::GetConditionComponent() const {
	return ConditionComponent;
}

UHealthComponent* ABaseCharacter::GetHealthComponent() const {
	return HealthComponent;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();

	if (StaminaComponent){
		StaminaComponent->RegisterComponent();
	}

	if (CombatComponent){
		CombatComponent->RegisterComponent();
	}

	if (HealthComponent){
		HealthComponent->RegisterComponent();
	}

	if (ConditionComponent){
		ConditionComponent->RegisterComponent();
	}
	
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

