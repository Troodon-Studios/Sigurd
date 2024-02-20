// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AICBaseEnemy.h"

#include "Enemies/BaseEnemy.h"

// Sets default values
AAICBaseEnemy::AAICBaseEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    
}

// Called when the game starts or when spawned
void AAICBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AAICBaseEnemy::SetStateAs(const EEnemyState NewState)
{

	ActualState = NewState;
	BlackboardComponent->SetValueAsEnum(StateKn, static_cast<uint8>(ActualState));
    
}

// Called every frame
void AAICBaseEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAICBaseEnemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(InPawn); BaseEnemy && BaseEnemy->BehaviourTree)
	{
		RunBehaviorTree(BaseEnemy->BehaviourTree);
		BlackboardComponent = GetBlackboardComponent();
	}
    
}