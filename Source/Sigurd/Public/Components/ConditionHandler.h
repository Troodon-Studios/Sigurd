#pragma once
#include "HealthComponent.h"
#include "DataTypes/Elements.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConditionHandler.generated.h"

class UConditionComponent;

UCLASS()
class UConditionHandler : public UObject {
	GENERATED_BODY()

private:
	UHealthComponent* HealthComponent;
	UConditionComponent* ConditionComponent;
	UCharacterMovementComponent* MovementComponent;

public:
	UConditionHandler();

	void Initialize(UConditionComponent* ParentConditionComponent);

	void StartBurning();
	void StartFreezing();
	void StartBlooming();

	
	void Burn();
	void Extinguish();
	void Freeze();
	void Unfreeze();
	void Bloom();


TMap<TPair<ECondition, Element>, TFunction<void()>> ConditionElementFunctionMap;};