
#include "Components/ConditionHandler.h"

#include "Components/ConditionComponent.h"

UConditionHandler::UConditionHandler(){
	ConditionElementFunctionMap.Add(TPair<ECondition, Element>(ECondition::None, Element::Fire), [this](){StartBurning();});
	ConditionElementFunctionMap.Add(TPair<ECondition, Element>(ECondition::Burn, Element::Ice), [this](){Extinguish();});
	ConditionElementFunctionMap.Add(TPair<ECondition, Element>(ECondition::Burn, Element::Fire), [this](){StartBurning();});
	ConditionElementFunctionMap.Add(TPair<ECondition, Element>(ECondition::None, Element::Ice), [this](){StartFreezing();});
	ConditionElementFunctionMap.Add(TPair<ECondition, Element>(ECondition::Freeze, Element::Fire), [this](){Unfreeze();});
	ConditionElementFunctionMap.Add(TPair<ECondition, Element>(ECondition::Freeze, Element::Ice), [this](){StartFreezing();});
}

void UConditionHandler::Initialize(UConditionComponent* ParentConditionComponent){
	this->ConditionComponent = ParentConditionComponent;
	this->HealthComponent = ConditionComponent->GetOwner()->FindComponentByClass<UHealthComponent>();
	this->MovementComponent = ConditionComponent->GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
}

void UConditionHandler::StartBurning(){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartBurning"));
	GetWorld()->GetTimerManager().SetTimer(ConditionEffectInfos[ECondition::Burn].TimerHandle, this, &UConditionHandler::Burn, 1.0f, true, 0.0f);
	ConditionComponent->Conditions.Add(ECondition::Burn);
	//Stop burning after 5 seconds
	GetWorld()->GetTimerManager().SetTimer(ConditionEffectInfos[ECondition::Extinguish].TimerHandle, this, &UConditionHandler::Extinguish, 5.0f, false, 5.0f);
}

void UConditionHandler::StartFreezing(){
	GetWorld()->GetTimerManager().SetTimer(ConditionEffectInfos[ECondition::Freeze].TimerHandle, this, &UConditionHandler::Freeze, 1.0f, false, 0.0f);
	ConditionComponent->Conditions.Add(ECondition::Freeze);
	//Stop freezing after 5 seconds
	GetWorld()->GetTimerManager().SetTimer(ConditionEffectInfos[ECondition::Unfreeze].TimerHandle, this, &UConditionHandler::Unfreeze, 5.0f, false, 5.0f);
}

void UConditionHandler::StartBlooming(){
}

void UConditionHandler::Burn(){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Burn"));
	HealthComponent->TakeDamage(5, EDamageType::Fire);
}

void UConditionHandler::Extinguish(){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Extinguish"));
	GetWorld()->GetTimerManager().ClearTimer(ConditionEffectInfos[ECondition::Burn].TimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ConditionEffectInfos[ECondition::Extinguish].TimerHandle);
	ConditionComponent->Conditions.Remove(ECondition::Burn);
}

void UConditionHandler::Freeze(){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Freeze"));
	MovementComponent->MaxWalkSpeed = 300.0f;
}

void UConditionHandler::Unfreeze(){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unfreeze"));
	MovementComponent->MaxWalkSpeed = 600.0f;
	GetWorld()->GetTimerManager().ClearTimer(ConditionEffectInfos[ECondition::Freeze].TimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ConditionEffectInfos[ECondition::Unfreeze].TimerHandle);
	ConditionComponent->Conditions.Remove(ECondition::Freeze);
}

void UConditionHandler::Bloom(){
	HealthComponent->Heal(ConditionEffectInfos[ECondition::Bloom].Damage);
}
