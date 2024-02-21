// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/AICBaseEnemy.h"
#include "Enemies/BaseEnemy.h"

// Sets default values
// Constructor for the AAICBaseEnemy class
// Initializes the AIPerception component
AAICBaseEnemy::AAICBaseEnemy(FObjectInitializer const& ObjectInitializer)
{
 // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
 PrimaryActorTick.bCanEverTick = true;

 SetAIPerception();

}

// Called when the game starts or when spawned
// Sets up the AIPerception component
void AAICBaseEnemy::BeginPlay()
{
 Super::BeginPlay();
}

// Sets the state of the enemy
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

// Called when the AIController possesses a pawn
void AAICBaseEnemy::OnPossess(APawn* InPawn)
{
 Super::OnPossess(InPawn);

 // If the pawn is a BaseEnemy and has a BehaviourTree, run the BehaviourTree and get the BlackboardComponent
 if (const ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(InPawn); BaseEnemy && BaseEnemy->BehaviourTree)
 {
  BlackboardComponent = GetBlackboardComponent();
  UseBlackboard(BaseEnemy->BehaviourTree->BlackboardAsset, BlackboardComponent);
  Blackboard = BlackboardComponent;
  RunBehaviorTree(BaseEnemy->BehaviourTree);

 }
}

// Called when the perception of the AIController is updated
void AAICBaseEnemy::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{
 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Entered")));

 // For each actor in the updated perception, check if the actor can be sensed and print a debug message
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

void AAICBaseEnemy::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
 if (Stimulus.Type == SightConfig->GetSenseID())
 {
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I see you %s"), *Actor->GetName()));
 }

 if (Stimulus.Type == HearingConfig->GetSenseID())
 {
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I hear you %s"), *Actor->GetName()));
 }

 if (Stimulus.Type == DamageConfig->GetSenseID())
 {
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I feel you %s"), *Actor->GetName()));
 }

}
// Checks if the AIController can sense a specific actor
bool AAICBaseEnemy::CanSenseActor(const AActor* Actor, const TSubclassOf<UAISense> SenseType) const
{
    TArray<AActor*> PerceivedActors;
    AIPerception->GetCurrentlyPerceivedActors(SenseType, PerceivedActors);

    return PerceivedActors.Contains(Actor);
}

// Sets up the AIPerception component
void AAICBaseEnemy::SetAIPerception()
{
 SetupSenseConfigSight();
 SetupSenseConfigHearing();
 SetupSenseConfigDamage();
}

// Sets up the sight sense configuration
void AAICBaseEnemy::SetupSenseConfigSight()
{
 SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

 if (SightConfig)
 {
  SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

  SightConfig->SightRadius = 1000.0f;
  SightConfig->LoseSightRadius = SightConfig->SightRadius + 500.0f;
  SightConfig->PeripheralVisionAngleDegrees = 90.0f;
  SightConfig->SetMaxAge(5.0f);
  SightConfig->AutoSuccessRangeFromLastSeenLocation = 1000.0f;
  SightConfig->DetectionByAffiliation.bDetectEnemies = true;
  SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
  SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

  GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
  GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAICBaseEnemy::OnTargetDetected);
  GetPerceptionComponent()->ConfigureSense(*SightConfig);
  
 }
 
}

// Sets up the hearing sense configuration
void AAICBaseEnemy::SetupSenseConfigHearing()
{
 HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

 if (HearingConfig)
 {

  HearingConfig->HearingRange = 1000.0f;
  HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
  HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
  HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
  HearingConfig->SetMaxAge(5.0f);
  GetPerceptionComponent()->ConfigureSense(*HearingConfig);
  GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAICBaseEnemy::OnTargetDetected);
  
 }
 
}

// Sets up the damage sense configuration
void AAICBaseEnemy::SetupSenseConfigDamage()
{
 DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));

 if (DamageConfig)
 {

  DamageConfig->SetMaxAge(5.0f);
  GetPerceptionComponent()->ConfigureSense(*DamageConfig);
  GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAICBaseEnemy::OnTargetDetected);
  
 }
 
}