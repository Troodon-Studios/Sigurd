

#include "Components/HealthComponent.h"

#include "DamageData.h"
#include "Sigurd/DamageTypeInterface.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

}


void UHealthComponent::TakeDamage(float damage, EDamageType attackerType){
	float damageMultiplier = 1.0f;

	if(DamageTable)
	{
		FDamageData* row = DamageTable->FindRow<FDamageData>(
				   FName(*FString::Printf(TEXT("%d_%d"), static_cast<int32>(attackerType), static_cast<int32>(SelfType))), TEXT(""));
		
		if (row)
		{
			damageMultiplier = row->multiplier;
		}
	}

	CurrentHealth -= damage * damageMultiplier;
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

void UHealthComponent::Heal(float healAmount){
	CurrentHealth += healAmount;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

float UHealthComponent::GetHealthPercentage(){
	return CurrentHealth / MaxHealth;
}

