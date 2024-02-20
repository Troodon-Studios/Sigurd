// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enums/EnemyState.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
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
	
	// AI Perception Config
	void SetAIPerception() const;
	static UAISenseConfig_Sight* CreateSenseConfigSight();
	static UAISenseConfig_Hearing* CreateSenseConfigHearing();
	static UAISenseConfig_Damage* CreateSenseConfigDamage();

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& Actors);
	bool CanSenseActor(const AActor* Actor, TSubclassOf<UAISense> SenseType) const;
	
};