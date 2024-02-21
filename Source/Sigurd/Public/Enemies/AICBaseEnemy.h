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
	explicit AAICBaseEnemy(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	
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
	void SetAIPerception();
	void SetupSenseConfigSight();
	void SetupSenseConfigHearing();
	void SetupSenseConfigDamage();

	UAISenseConfig_Sight* SightConfig;
	UAISenseConfig_Hearing* HearingConfig;
	UAISenseConfig_Damage* DamageConfig;
	
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& Actors);
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
	bool CanSenseActor(const AActor* Actor, TSubclassOf<UAISense> SenseType) const;
	
};