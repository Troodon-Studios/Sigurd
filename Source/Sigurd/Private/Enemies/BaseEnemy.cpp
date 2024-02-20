// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/BaseEnemy.h"


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

