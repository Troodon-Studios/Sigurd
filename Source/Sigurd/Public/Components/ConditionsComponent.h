// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTypes/Elements.h"
#include "ConditionsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGURD_API UConditionsComponent : public UActorComponent{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UConditionsComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conditions")
	TArray<Condition> Conditions;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
