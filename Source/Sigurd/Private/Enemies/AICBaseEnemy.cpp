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


// Checks if the AIController can sense a specific actor
bool AAICBaseEnemy::CanSenseActor(const AActor* Actor, TSubclassOf<UAISense> SenseType)
{
 
  return true;

 
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
  SightConfig->PeripheralVisionAngleDegrees = 60.0f;
  SightConfig->SetMaxAge(5.0f);
  SightConfig->AutoSuccessRangeFromLastSeenLocation = 1000.0f;
  SightConfig->DetectionByAffiliation.bDetectEnemies = true;
  SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
  SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

  GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
  GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAICBaseEnemy::OnTargetPerceived);
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
  GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAICBaseEnemy::OnTargetPerceived);
  GetPerceptionComponent()->ConfigureSense(*HearingConfig);

 }
 
}

// Sets up the damage sense configuration
void AAICBaseEnemy::SetupSenseConfigDamage()
{
 DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));

 if (DamageConfig)
 {

  DamageConfig->SetMaxAge(5.0f);
  GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAICBaseEnemy::OnTargetPerceived);
  GetPerceptionComponent()->ConfigureSense(*DamageConfig);

 }
 
}


void AAICBaseEnemy::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
    // Check if the stimulus was successfully sensed
    if(!Stimulus.WasSuccessfullySensed()) return;

    // Handle the stimulus based on its type
    if (const auto StimulusType = Stimulus.Type; StimulusType == DamageConfig->GetSenseID()) HandleDamaged(Actor, Stimulus);
    else if (StimulusType == HearingConfig->GetSenseID()) HandleHeardNoise(Stimulus);
    else if (StimulusType == SightConfig->GetSenseID()) HandleSeenActor(Actor);
 
}

void AAICBaseEnemy::HandleSeenActor(AActor* Actor)
{
 if (ActualState == EEnemyState::Passive || ActualState == EEnemyState::Investigating || ActualState == EEnemyState::Seeking)
 {
  BlackboardComponent->SetValueAsObject(AttackKn, Actor);
  SetStateAs(EEnemyState::Combat);
 }
}

void AAICBaseEnemy::HandleHeardNoise(const FAIStimulus& Stimulus)
{
 if (ActualState == EEnemyState::Passive || ActualState == EEnemyState::Investigating || ActualState == EEnemyState::Seeking)
 {
  BlackboardComponent->SetValueAsVector(PointOfInterestKn, Stimulus.StimulusLocation);
  SetStateAs(EEnemyState::Investigating);
 } 
}

void AAICBaseEnemy::HandleDamaged(AActor* Actor, FAIStimulus Stimulus)
{
 if (ActualState == EEnemyState::Passive || ActualState == EEnemyState::Investigating || ActualState == EEnemyState::Seeking)
 {
  BlackboardComponent->SetValueAsObject(AttackKn, Actor);
  SetStateAs(EEnemyState::Combat);
 }
}

