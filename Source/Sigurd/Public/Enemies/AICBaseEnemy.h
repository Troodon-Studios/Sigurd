// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "AICBaseEnemy.generated.h"

UCLASS()
class SIGURD_API AIcBaseEnemy : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AIcBaseEnemy();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UAIPerceptionComponent> AIPerception;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
