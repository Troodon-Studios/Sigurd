// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConditionHandler.h"
#include "Components/ActorComponent.h"
#include "Interfaces/ElementInterface.h"
#include "ConditionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGURD_API UConditionComponent : public UActorComponent, public IElementInterface{
	GENERATED_BODY()

private:


	UConditionHandler* ConditionHandler;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	TArray<ECondition> Conditions;
	// Sets default values for this component's properties
	UConditionComponent();

	virtual void BeginPlay() override;

	void AddBurn_Implementation() override;

	void AddExtinguish_Implementation() override;

	virtual void AddElement_Implementation(Element NewElement) override;

};
