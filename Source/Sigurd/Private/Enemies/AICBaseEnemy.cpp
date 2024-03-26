#include "Enemies/AICBaseEnemy.h"
#include "Enemies/BaseEnemy.h"

// Constructor for the AICBaseEnemy class
AAICBaseEnemy::AAICBaseEnemy(FObjectInitializer const& ObjectInitializer)
{
	// Enable ticking for this actor
	PrimaryActorTick.bCanEverTick = true;
	//SetAIPerception();
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));

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
			
			// Set up AI perception
			SetAIPerception();
		}else
		{
			UE_LOG(LogTemp, Error, TEXT("No behaviour tree found"));
		}

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
	if (BaseEnemy == nullptr) return;
	// Set up sight, hearing, and damage senses
	SetupSenseConfigSight();
	SetupSenseConfigHearing();
	SetupSenseConfigDamage();
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAICBaseEnemy::OnTargetPerceived);
}

// Set up sight sense
void AAICBaseEnemy::SetupSenseConfigSight()
{


	if(!BaseEnemy->EnemyType.bCanSee)return;
	

	// If the sight config was successfully created
	if (SightConfig)
	{
		// Set up the perception component
		const FSightValues SightValues = BaseEnemy->EnemyType.SightValues;
		
		// Configure the sight config
		SightConfig->SightRadius = SightValues.SightRadius;
		SightConfig->LoseSightRadius = SightValues.LoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = SightValues.PeripheralVisionAngleDegrees;
		DamageConfig->SetMaxAge(BaseEnemy->EnemyType.MemoryTime);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = SightValues.AutoSuccessRangeFromLastSeenLocation;
		SightConfig->DetectionByAffiliation.bDetectEnemies = SightValues.DetectionByAffiliation.bDetectEnemies;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = SightValues.DetectionByAffiliation.bDetectNeutrals;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = SightValues.DetectionByAffiliation.bDetectFriendlies;

		// Set the dominant sense for the perception component
		if (BaseEnemy->EnemyType.DominantSense == EDominantSense::Sight)
		{
			GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		}
		// Configure the perception component with the sight config
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

// Set up hearing sense
void AAICBaseEnemy::SetupSenseConfigHearing()
{
	if (!BaseEnemy->EnemyType.bCanHear)return;
	

	// If the hearing config was successfully created
	if (HearingConfig)
	{
		// Configure the hearing config
		HearingConfig->HearingRange = 1000.0f;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = BaseEnemy->EnemyType.SightValues.DetectionByAffiliation.bDetectEnemies;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = BaseEnemy->EnemyType.SightValues.DetectionByAffiliation.bDetectNeutrals;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = BaseEnemy->EnemyType.SightValues.DetectionByAffiliation.bDetectFriendlies;
		DamageConfig->SetMaxAge(BaseEnemy->EnemyType.MemoryTime);

		// Set the dominant sense for the perception component
		if (BaseEnemy->EnemyType.DominantSense == EDominantSense::Hear)
		{
			GetPerceptionComponent()->SetDominantSense(*HearingConfig->GetSenseImplementation());
		}		
		// Configure the perception component with the hearing config
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
}

// Set up damage sense
void AAICBaseEnemy::SetupSenseConfigDamage()
{
	if (!BaseEnemy->EnemyType.bCanFeel)return;
	
	// If the damage config was successfully created
	if (DamageConfig)
	{
		// Configure the damage config
		DamageConfig->SetMaxAge(BaseEnemy->EnemyType.MemoryTime);

		// Set the dominant sense for the perception component
		if (BaseEnemy->EnemyType.DominantSense == EDominantSense::Feel)
		{
			GetPerceptionComponent()->SetDominantSense(*DamageConfig->GetSenseImplementation());
		}		
		// Configure the perception component with the damage config
		GetPerceptionComponent()->ConfigureSense(*DamageConfig);
	}
}

// Called when a target is perceived
void AAICBaseEnemy::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Error, TEXT("Perceived"));
	
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
