
#include "Components/ConditionHandler.h"

#include "Components/ConditionComponent.h"

UConditionHandler::UConditionHandler(){
	ConditionElementFunctionMap.Add(TPair<ECondition, Element>(ECondition::None, Element::Fire), [this](){StartBurning();});
	ConditionElementFunctionMap.Add(TPair<ECondition, Element>(ECondition::Burn, Element::Water), [this](){Extinguish();});
}

void UConditionHandler::Initialize(UConditionComponent* ParentConditionComponent, UHealthComponent* ParentHealthComponent){
	this->ConditionComponent = ParentConditionComponent;
	this->HealthComponent = ParentHealthComponent;
}

void UConditionHandler::StartBurning(){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartBurning"));
	GetWorld()->GetTimerManager().SetTimer(ConditionEffectInfos[0].TimerHandle, this, &UConditionHandler::Burn, 1.0f, true, 1.0f);
	ConditionComponent->Conditions.Add(ECondition::Burn);
	//Stop burning after 5 seconds
	GetWorld()->GetTimerManager().SetTimer(ConditionEffectInfos[1].TimerHandle, this, &UConditionHandler::Extinguish, 5.0f, false, 5.0f);
}

void UConditionHandler::Burn(){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Burn"));
	HealthComponent->TakeDamage(5, EDamageType::Fire);
}

void UConditionHandler::Extinguish(){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Extinguish"));
	GetWorld()->GetTimerManager().ClearTimer(ConditionEffectInfos[0].TimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ConditionEffectInfos[1].TimerHandle);
	ConditionComponent->Conditions.Remove(ECondition::Burn);
}