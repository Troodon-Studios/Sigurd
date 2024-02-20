// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/AICBaseEnemy.h"
#include "Enemies/BaseEnemy.h"

// Sets default values
AAICBaseEnemy::AAICBaseEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	SetAIPerception();
	
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

void AAICBaseEnemy::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{
//TODO : Handle the perception update for each sense
	
	for (const auto Actor : Actors)
	{
		if (CanSenseActor(Actor, UAISense_Sight::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I see you %s"), *Actor->GetName()));
		}
		if (CanSenseActor(Actor, UAISense_Hearing::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I hear you %s"), *Actor->GetName()));
		}
		if (CanSenseActor(Actor, UAISense_Damage::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I feel you %s"), *Actor->GetName()));
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I cant sense you %s"), *Actor->GetName()));
	}
	
}

bool AAICBaseEnemy::CanSenseActor(const AActor* Actor, const TSubclassOf<UAISense> SenseType) const
{
    TArray<AActor*> PerceivedActors;
    AIPerception->GetCurrentlyPerceivedActors(SenseType, PerceivedActors);

    return PerceivedActors.Contains(Actor);
}


void AAICBaseEnemy::SetAIPerception() const
{
	
	UAISenseConfig_Sight* SightConfig = CreateSenseConfigSight();
	UAISenseConfig_Hearing* HearingConfig = CreateSenseConfigHearing();
	UAISenseConfig_Damage* DamageConfig = CreateSenseConfigDamage();

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->ConfigureSense(*HearingConfig);
	AIPerception->ConfigureSense(*DamageConfig);

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AAICBaseEnemy::OnPerceptionUpdated);
	
}

UAISenseConfig_Sight* AAICBaseEnemy::CreateSenseConfigSight()
{
	UAISenseConfig_Sight* SightConfig = NewObject<UAISenseConfig_Sight>();
	SightConfig->SightRadius = 1000.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 1000.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	return SightConfig;
}

UAISenseConfig_Hearing* AAICBaseEnemy::CreateSenseConfigHearing()
{
	UAISenseConfig_Hearing* HearingConfig = NewObject<UAISenseConfig_Hearing>();
	HearingConfig->HearingRange = 2000.0f;
	HearingConfig->SetMaxAge(5.0f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	return HearingConfig;
}

UAISenseConfig_Damage* AAICBaseEnemy::CreateSenseConfigDamage()
{
	UAISenseConfig_Damage* DamageConfig = NewObject<UAISenseConfig_Damage>();
	DamageConfig->SetMaxAge(5.0f);

	return DamageConfig;
}