#pragma once
#include "HealthComponent.h"
#include "DataTypes/Elements.h"
#include "ConditionHandler.generated.h"

class UConditionComponent;

UCLASS()
class UConditionHandler : public UObject {
	GENERATED_BODY()

private:
		UHealthComponent* HealthComponent;
	UConditionComponent* ConditionComponent;

public:
	UConditionHandler();

	void Initialize(UConditionComponent* ParentConditionComponent, UHealthComponent* ParentHealthComponent);

	void StartBurning();
	void Burn();
	void Extinguish();

TMap<TPair<ECondition, Element>, TFunction<void()>> ConditionElementFunctionMap;};