// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enums/EnemyState.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICBaseEnemy.generated.h"

UCLASS()
class SIGURD_API AAICBaseEnemy : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	// Sets default values for this actor's properties
	AAICBaseEnemy();
    
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UAIPerceptionComponent> AIPerception;


	UFUNCTION(BlueprintCallable)
	void SetStateAs(EEnemyState NewState);
    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FName AttackKn = "AttackTarget";
	FName StateKn = "State";
	FName PointOfInterestKn = "PointOfInterest";
	FName AttackRadiusKn = "AttackRadius";
	FName DefendRadiusKn = "DefendRadius";
	EEnemyState ActualState;
	UBlackboardComponent* BlackboardComponent;

};