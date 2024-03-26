// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICBaseEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CombatComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/CapsuleComponent.h"
#include "PatrolRoute.h"
#include "Slots.h"
#include "Components/TokenComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/AICoreInterface.h"
#include "BaseEnemy.generated.h"

USTRUCT(BlueprintType)
struct FSightValues : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config, meta = (UIMin = 0.0, ClampMin = 0.0))
	float SightRadius = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config, meta = (UIMin = 0.0, ClampMin = 0.0))
	float LoseSightRadius = SightRadius + 500.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config, meta=(UIMin = 0.0, ClampMin = 0.0, UIMax = 180.0, ClampMax = 180.0, DisplayName="PeripheralVisionHalfAngleDegrees"))
	float PeripheralVisionAngleDegrees = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config)
	FAISenseAffiliationFilter DetectionByAffiliation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config)
	float AutoSuccessRangeFromLastSeenLocation = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config, meta = (UIMin = 0.0, ClampMin = 0.0))
	float PointOfViewBackwardOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config, meta = (UIMin = 0.0, ClampMin = 0.0))
	float NearClippingRadius;
};

USTRUCT(BlueprintType)
struct FHearingValues : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", meta = (UIMin = 0.0, ClampMin = 0.0))
	float HearingRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config)
	FAISenseAffiliationFilter DetectionByAffiliation;
	
};

UENUM(BlueprintType)
enum class EDominantSense : uint8
{
	Sight,
	Hear,
	Feel
};

USTRUCT(BlueprintType)
struct FEnemyType : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviourTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (UIMin = 0.0, ClampMin = 0.0))
	double AttackRadius = 150.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (UIMin = 0.0, ClampMin = 0.0))
	double DefendRadius = 350.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (UIMin = 0.0, ClampMin = 0.0))
	float Health = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses")
	bool bCanSee = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses", meta = (EditCondition = "bCanSee",EditConditionHides))
	FSightValues SightValues;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses")
	bool bCanHear = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses", meta = (EditCondition = "bCanHear",EditConditionHides))
	FHearingValues HearingValues;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses")
	bool bCanFeel = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses", meta = (EditCondition = "bCanSee || bCanHear || bCanFeel"))
	EDominantSense DominantSense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses", meta = (EditCondition = "bCanSee || bCanHear || bCanFeel"),  meta = (UIMin = 0.0, ClampMin = 0.0))
	float MemoryTime = 5.0f;
	
};

UCLASS()
class SIGURD_API ABaseEnemy : public ACharacter, public IAICoreInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	UStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UTokenComponent* TokenComponent;
	
	
public:
	// Sets default values for this character's properties
	ABaseEnemy();

	// AI Variables

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI")
	APatrolRoute* PatrolRoute;

	FEnemyType EnemyType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI")
	FSlots Slot;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void TakeDamageEnemy(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
										class AController* InstigatedBy, AActor* DamageCauser) const;
public:
	
	// IAICoreInterface implementation
	virtual APatrolRoute* GetPatrolRoute_Implementation() override;
	virtual void SetMovementSpeed_Implementation(ESpeedState Speed, double& SpeedValue) override;
	virtual void GetIdealRange_Implementation(double& AttackRadius, double& DefendRadius) override;
	virtual void EventAttack_Implementation(AActor* AttackTarget) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetUpEnemyType();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	class UAIPerceptionStimuliSourceComponent* StimuliSource;

	void SetupStimulusSource();

	
};

