#pragma once

#include "CoreMinimal.h"
#include "DataTypes/Elements.h"
#include "UObject/Interface.h"
#include "ElementInterface.generated.h"

class UConditionsComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UElementInterface : public UInterface{
	GENERATED_BODY()
};


class SIGURD_API IElementInterface{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Element")
	// void AddElement(Element NewElement, UConditionsComponent* Emmiter);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Element")
	void AddBurn();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Element")
	void AddExtinguish();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Element")
	void AddElement(Element NewElement);
};
