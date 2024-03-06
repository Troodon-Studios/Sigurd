#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Types.h"
#include "DamageTypeInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDamageTypeInterface : public UInterface {
	GENERATED_BODY()
};

class IDamageTypeInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyInterface")
	const EDamageType ProcessDamage();
};
