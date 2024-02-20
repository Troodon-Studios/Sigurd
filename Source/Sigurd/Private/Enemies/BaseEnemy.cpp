// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/BaseEnemy.h"

#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("RH_Socket"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetAIPerception();
	
	InitializeSpeedValues(100.0, 200.0, 300.0, 400.0);
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (StaminaComponent){
		StaminaComponent->RegisterComponent();
	}

	if (CombatComponent){
		CombatComponent->RegisterComponent();
	}

	if (HealthComponent){
		HealthComponent->RegisterComponent();
	}

	//OnTakeAnyDamage.AddDynamic(this, &ABaseEnemy::TakeDamageEnemy);	
}


// Called every frame
void ABaseEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::TakeDamageEnemy(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
										class AController* InstigatedBy, AActor* DamageCauser) {
	UObject* ObjectInstance = const_cast<UObject*>(static_cast<const UObject*>(DamageType));
	CombatComponent->TakeDamage(Damage, ObjectInstance);
}

void ABaseEnemy::SetAIPerception()
{
    // Create and configure sight sense
    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    SightConfig->SightRadius = 1000.0f;
    SightConfig->LoseSightRadius = SightConfig->SightRadius + 500.0f;
    SightConfig->PeripheralVisionAngleDegrees = 90.0f;
    SightConfig->SetMaxAge(5.0f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 1000.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // Create and configure hearing sense
    UAISenseConfig_Hearing* HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
    HearingConfig->HearingRange = 2000.0f;
    HearingConfig->SetMaxAge(5.0f);
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // Create and configure damage sense
    UAISenseConfig_Damage* DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
    DamageConfig->SetMaxAge(5.0f);

    // Add the configured senses to the perception component
    AIPerception->ConfigureSense(*SightConfig);
    AIPerception->ConfigureSense(*HearingConfig);
    AIPerception->ConfigureSense(*DamageConfig);
}

APatrolRoute* ABaseEnemy::GetPatrolRoute_Implementation()
{
	return PatrolRoute;
}

void ABaseEnemy::SetMovementSpeed_Implementation(ESpeedState Speed, double& SpeedValue)
{
	SpeedValue = SpeedValues[Speed];
	GetCharacterMovement()->MaxWalkSpeed = SpeedValue;
}

void ABaseEnemy::GetIdealRange_Implementation(double& AttackRadius, double& DefendRadius)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GetIdealRange_Implementation"));
}

void ABaseEnemy::EventAttack_Implementation(AActor* AttackTarget)
{
	CombatComponent->Attack();
}

