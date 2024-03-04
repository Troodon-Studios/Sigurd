// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "ReactionHandler.h"
#include "Components/ActorComponent.h"
#include "DataTypes/Elements.h"
#include "ConditionsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGURD_API UConditionsComponent : public UActorComponent{
	GENERATED_BODY()

private:
	
	
	void ExecuteReaction(Condition condition, Element element) {
		// Get the reaction function from the reaction map
		ReactionFunction* reactionFunction = ReactionMap.Find(TPair<Condition, Element>(condition, element));
	
		// If the reaction function exists in the map
		if (reactionFunction != nullptr && CurrentHandler != nullptr) {
			// Execute the reaction function
			(CurrentHandler->*(*reactionFunction))();
		}
	}

	




	

public:
	// Sets default values for this component's properties
	UConditionsComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elements")
	TArray<Element> Elements;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conditions")
	TArray<Condition> Conditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reactions")
	UReactionHandler* CurrentHandler;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
