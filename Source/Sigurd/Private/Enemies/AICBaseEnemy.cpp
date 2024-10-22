﻿#include "Enemies/AICBaseEnemy.h"
#include "Enemies/BaseEnemy.h"

// Constructor for the AICBaseEnemy class
AAICBaseEnemy::AAICBaseEnemy(FObjectInitializer const& ObjectInitializer)
{
	// Enable ticking for this actor
	PrimaryActorTick.bCanEverTick = true;
	// Set up AI perception
	SetAIPerception();
}

// Called when the game starts or when spawned
void AAICBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Set the state of the enemy
void AAICBaseEnemy::SetStateAs(const EEnemyState NewState)
{
	// Update the actual state
	ActualState = NewState;
	// Update the blackboard component with the new state
	BlackboardComponent->SetValueAsEnum(StateKn, static_cast<uint8>(ActualState));
}

// Called every frame
void AAICBaseEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the AI controller possesses a pawn
void AAICBaseEnemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


	// If the pawn is a base enemy and has a behavior tree
	if (BaseEnemy = Cast<ABaseEnemy>(InPawn); BaseEnemy)
	{
		BaseEnemy->SetUpEnemyType();
		if (BaseEnemy->EnemyType.BehaviourTree)
		{
			// Get the blackboard component
			BlackboardComponent = GetBlackboardComponent();
			// Use the blackboard associated with the behavior tree
			UseBlackboard(BaseEnemy->EnemyType.BehaviourTree->BlackboardAsset, BlackboardComponent);
			// Set the blackboard
			Blackboard = BlackboardComponent;

			// Set the combat radius and the defend radius
			BlackboardComponent->SetValueAsFloat(AttackRadiusKn, BaseEnemy->EnemyType.AttackRadius);
			BlackboardComponent->SetValueAsFloat(DefendRadiusKn, BaseEnemy->EnemyType.DefendRadius);

			// Run the behavior tree
			RunBehaviorTree(BaseEnemy->EnemyType.BehaviourTree);

			ReConfigureSenseConfig();
		}
	}
}

void AAICBaseEnemy::ReConfigureSenseConfig()
{
	// Sight
	if (BaseEnemy->EnemyType.bCanSee)
	{
		SightConfig->SightRadius = BaseEnemy->EnemyType.SightValues.SightRadius;
		SightConfig->LoseSightRadius = BaseEnemy->EnemyType.SightValues.LoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = BaseEnemy->EnemyType.SightValues.PeripheralVisionAngleDegrees;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = BaseEnemy->EnemyType.SightValues.AutoSuccessRangeFromLastSeenLocation;
		SightConfig->DetectionByAffiliation.bDetectEnemies = BaseEnemy->EnemyType.SightValues.DetectionByAffiliation.bDetectEnemies;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = BaseEnemy->EnemyType.SightValues.DetectionByAffiliation.bDetectNeutrals;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = BaseEnemy->EnemyType.SightValues.DetectionByAffiliation.bDetectFriendlies;
		SightConfig->SetMaxAge(BaseEnemy->EnemyType.MemoryTime);

		if (BaseEnemy->EnemyType.DominantSense == EDominantSense::Sight)
		{
			GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		}

	}
	else
	{
		GetPerceptionComponent()->SetSenseEnabled(SightConfig->GetSenseImplementation(), false);
	}

	// Hearing'
	if (BaseEnemy->EnemyType.bCanHear)
	{
		HearingConfig->HearingRange = BaseEnemy->EnemyType.HearingValues.HearingRange;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = BaseEnemy->EnemyType.HearingValues.DetectionByAffiliation.bDetectEnemies;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = BaseEnemy->EnemyType.HearingValues.DetectionByAffiliation.bDetectNeutrals;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = BaseEnemy->EnemyType.HearingValues.DetectionByAffiliation.bDetectFriendlies;
		HearingConfig->SetMaxAge(BaseEnemy->EnemyType.MemoryTime);

		if (BaseEnemy->EnemyType.DominantSense == EDominantSense::Hear)
		{
			GetPerceptionComponent()->SetDominantSense(*HearingConfig->GetSenseImplementation());
		}
	}
	else
	{
		GetPerceptionComponent()->SetSenseEnabled(HearingConfig->GetSenseImplementation(), false);
	}

	// Damage
	if (BaseEnemy->EnemyType.bCanFeel)
	{
		DamageConfig->SetMaxAge(BaseEnemy->EnemyType.MemoryTime);

		if (BaseEnemy->EnemyType.DominantSense == EDominantSense::Feel)
		{
			GetPerceptionComponent()->SetDominantSense(*DamageConfig->GetSenseImplementation());
		}
	}
	else
	{
		GetPerceptionComponent()->SetSenseEnabled(DamageConfig->GetSenseImplementation(), false);
	}
	
}

// Check if the AI can sense a specific actor
bool AAICBaseEnemy::CanSenseActor(const AActor* Actor, TSubclassOf<UAISense> SenseType)
{
	// For now, the AI can sense all actors
	return true;
}

// Set up AI perception
void AAICBaseEnemy::SetAIPerception()
{
	// Set up sight, hearing, and damage senses
	SetupSenseConfigSight();
	SetupSenseConfigHearing();
	SetupSenseConfigDamage();
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAICBaseEnemy::OnTargetPerceived);
}

// Set up sight sense
void AAICBaseEnemy::SetupSenseConfigSight()
{
	// Create a sight config
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	// If the sight config was successfully created
	if (SightConfig)
	{
		// Set up the perception component
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

		// Configure the sight config
		SightConfig->SightRadius = 0;
		SightConfig->LoseSightRadius = 0;
		SightConfig->PeripheralVisionAngleDegrees = 0;
		SightConfig->SetMaxAge(0.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 0.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = false;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

		// Set the dominant sense for the perception component
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		// Configure the perception component with the sight config
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

// Set up hearing sense
void AAICBaseEnemy::SetupSenseConfigHearing()
{
	// Create a hearing config
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	// If the hearing config was successfully created
	if (HearingConfig)
	{
		// Configure the hearing config
		HearingConfig->HearingRange = 0.0f;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = false;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
		HearingConfig->SetMaxAge(0.0f);
		// Configure the perception component with the hearing config
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
}

// Set up damage sense
void AAICBaseEnemy::SetupSenseConfigDamage()
{
	// Create a damage config
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));

	// If the damage config was successfully created'
	if (DamageConfig)
	{
		// Configure the damage config
		DamageConfig->SetMaxAge(0.0f);
		// Configure the perception component with the damage config
		GetPerceptionComponent()->ConfigureSense(*DamageConfig);
	}
}

// Called when a target is perceived
void AAICBaseEnemy::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	// If the stimulus was not successfully sensed, return
	if (!Stimulus.WasSuccessfullySensed()) return;

	// Handle the stimulus based on its type
	if (const auto StimulusType = Stimulus.Type; StimulusType == DamageConfig->GetSenseID()) HandleDamaged(
		Actor, Stimulus);
	else if (StimulusType == HearingConfig->GetSenseID()) HandleHeardNoise(Stimulus);
	else if (StimulusType == SightConfig->GetSenseID()) HandleSeenActor(Actor);
}

// Handle seeing an actor
void AAICBaseEnemy::HandleSeenActor(AActor* Actor)
{
	// If the enemy is in a passive, investigating, or seeking state
	if (ActualState == EEnemyState::Passive || ActualState == EEnemyState::Investigating || ActualState ==
		EEnemyState::Seeking)
	{
		// Set the attack target to the seen actor
		BlackboardComponent->SetValueAsObject(AttackKn, Actor);
		// Change the state to combat
		SetStateAs(EEnemyState::Combat);
	}
}

// Handle hearing a noise
void AAICBaseEnemy::HandleHeardNoise(const FAIStimulus& Stimulus)
{
	// If the enemy is in a passive, investigating, or seeking state
	if (ActualState == EEnemyState::Passive || ActualState == EEnemyState::Investigating || ActualState ==
		EEnemyState::Seeking)
	{
		// Set the point of interest to the location of the noise
		BlackboardComponent->SetValueAsVector(PointOfInterestKn, Stimulus.StimulusLocation);
		// Change the state to investigating
		SetStateAs(EEnemyState::Investigating);
	}
}

// Handle being damaged
void AAICBaseEnemy::HandleDamaged(AActor* Actor, FAIStimulus Stimulus)
{
	// If the enemy is in a passive, investigating, or seeking state
	if (ActualState == EEnemyState::Passive || ActualState == EEnemyState::Investigating || ActualState ==
		EEnemyState::Seeking)
	{
		// Set the attack target to the actor that damaged the enemy
		BlackboardComponent->SetValueAsObject(AttackKn, Actor);
		// Change the state to combat
		SetStateAs(EEnemyState::Combat);
	}
}
