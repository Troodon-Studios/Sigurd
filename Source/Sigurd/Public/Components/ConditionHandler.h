#pragma once
#include "HealthComponent.h"
#include "DataTypes/Elements.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConditionHandler.generated.h"

class UConditionComponent;
class ABaseCharacter;

UCLASS()
class UConditionHandler : public UObject {
	GENERATED_BODY()

private:
	ABaseCharacter* OwnerCharacter;
	UConditionComponent* ConditionComponent;
	UHealthComponent* HealthComponent;
	UCharacterMovementComponent* MovementComponent;

public:
	UConditionHandler();

	void Initialize(ABaseCharacter* InOwnerCharacter);

	void StartBurning();
	void StartFreezing();
	void StartBlooming();

	
	void Burn();
	void Extinguish();
	void Freeze();
	void Unfreeze();
	void Bloom();


TMap<TPair<ECondition, Element>, TFunction<void()>> ConditionElementFunctionMap;};