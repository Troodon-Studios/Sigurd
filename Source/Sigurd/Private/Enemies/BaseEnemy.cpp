// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/BaseEnemy.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"

// Sets default values
// Constructor for the ABaseEnemy class
// Initializes the StaminaComponent, CombatComponent, HealthComponent, and WeaponMesh
ABaseEnemy::ABaseEnemy() : ABaseCharacter()
{
 // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
 PrimaryActorTick.bCanEverTick = true;

 // Set size for collision capsule
 GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

 SetupStimulusSource();
 
 // Initialize speed values
 InitializeSpeedValues(100.0, 200.0, 300.0, 400.0);
}

// Called when the game starts or when spawned
// Registers the StaminaComponent, CombatComponent, and HealthComponent
void ABaseEnemy::BeginPlay()
{
 Super::BeginPlay();
 
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

void ABaseEnemy::SetupStimulusSource()
{

 StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

 if (StimuliSource)
 {
  StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
  StimuliSource->RegisterWithPerceptionSystem();
 }
 
}

// Called when the enemy takes damage
void ABaseEnemy::TakeDamageEnemy(AActor* DamagedActor, const float Damage, const class UDamageType* DamageType,
          class AController* InstigatedBy, AActor* DamageCauser) const
{
 UObject* ObjectInstance = const_cast<UObject*>(static_cast<const UObject*>(DamageType));
 CombatComponent->TakeDamage(Damage, ObjectInstance);
}

// Returns the patrol route of the enemy
APatrolRoute* ABaseEnemy::GetPatrolRoute_Implementation()
{
 return PatrolRoute;
}

// Sets the movement speed of the enemy
void ABaseEnemy::SetMovementSpeed_Implementation(ESpeedState Speed, double& SpeedValue)
{
 SpeedValue = SpeedValues[Speed];
 GetCharacterMovement()->MaxWalkSpeed = SpeedValue;
}

// Gets the ideal range for the enemy
void ABaseEnemy::GetIdealRange_Implementation(double& _AttackRadius, double& _DefendRadius)
{
 _AttackRadius = AttackRadius;
 _DefendRadius = DefendRadius;
}

// Makes the enemy attack
void ABaseEnemy::EventAttack_Implementation(AActor* AttackTarget)
{
 CombatComponent->LightAttack();
}