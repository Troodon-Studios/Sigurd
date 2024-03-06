

#include "Components/HealthComponent.h"

#include "DamageData.h"
#include "Sigurd/DamageTypeInterface.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

}


void UHealthComponent::TakeDamage(float Damage, EDamageType AttackerType){
	float damageMultiplier = 1.0f;

	if(DamageTable)
	{
		FDamageData* row = DamageTable->FindRow<FDamageData>(
				   FName(*FString::Printf(TEXT("%d_%d"), static_cast<int32>(AttackerType), static_cast<int32>(SelfType))), TEXT(""));
		
		if (row)
		{
			damageMultiplier = row->multiplier;
		}
	}

	CurrentHealth -= Damage * damageMultiplier;
	if (CurrentHealth <= 0)
	{
		// Die
	}
}


void UHealthComponent::TakeDamageWithType(UObject* DamageType, float Damage){
	if(DamageType->Implements<UDamageTypeInterface>()) {
		EDamageType tempDamageType = IDamageTypeInterface::Execute_ProcessDamage(DamageType);
		TakeDamage(Damage, tempDamageType);
	}
}

void UHealthComponent::Heal(float HealAmount){
	CurrentHealth += HealAmount;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}



void UHealthComponent::StartDPS(float Damage,float Rate, float Duration, EDamageType AttackerType){
	// Create a timer delegate
	FTimerDelegate DamageDelegate = FTimerDelegate::CreateUObject(this, &UHealthComponent::TakeDamage, Damage, AttackerType);

	// Set the timer to call the delegate every second for the specified duration
	GetWorld()->GetTimerManager().SetTimer(DPSTimerHandle, DamageDelegate, Rate, true, 0.0f);

	// Set a timer to stop the DPS after the specified duration
	GetWorld()->GetTimerManager().SetTimer(StopDPSTimerHandle, FTimerDelegate::CreateLambda([this](){
		GetWorld()->GetTimerManager().ClearTimer(DPSTimerHandle);
	}), Duration, false);
}

float UHealthComponent::GetHealthPercentage(){
	return CurrentHealth / MaxHealth;
}

