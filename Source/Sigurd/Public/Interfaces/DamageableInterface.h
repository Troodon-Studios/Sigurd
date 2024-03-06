#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface {
	GENERATED_BODY()
};

class IDamageableInterface{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyInterface")
	void TakeDamage(float damage);
};