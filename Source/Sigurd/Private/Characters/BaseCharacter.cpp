#include "Characters/BaseCharacter.h"

ABaseCharacter::ABaseCharacter(){
	 StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	 CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	 HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	// ConditionComponent = CreateDefaultSubobject<UConditionComponent>(TEXT("ConditionComponent"));
	 TokenComponent = CreateDefaultSubobject<UTokenComponent>(TEXT("TokenComponent"));
}

void ABaseCharacter::BeginPlay(){
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
	
	// if (ConditionComponent){
	// 	ConditionComponent->RegisterComponent();
	// }
	
	if (TokenComponent){
		TokenComponent->RegisterComponent();
	}
}

UStaminaComponent* ABaseCharacter::GetStaminaComponent() const{
	return StaminaComponent;
}

UCombatComponent* ABaseCharacter::GetCombatComponent() const{
	return CombatComponent;
}

UHealthComponent* ABaseCharacter::GetHealthComponent() const{
	return HealthComponent;
}


UConditionComponent* ABaseCharacter::GetConditionComponent() const{
	return ConditionComponent;
}

UTokenComponent* ABaseCharacter::GetTokenComponent() const{
	return TokenComponent;
}


