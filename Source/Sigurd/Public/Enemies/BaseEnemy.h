// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICBaseEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CombatComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/CapsuleComponent.h"
#include "PatrolRoute.h"
#include "Characters/BaseCharacter.h"
#include "Components/TokenComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/AICoreInterface.h"
#include "BaseEnemy.generated.h"

UCLASS()
class SIGURD_API ABaseEnemy : public ABaseCharacter, public IAICoreInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;
	
	
public:
	// Sets default values for this character's properties
	ABaseEnemy();

	// AI Variables
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviourTree;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI")
	APatrolRoute* PatrolRoute;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	double AttackRadius = 150.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	double DefendRadius = 350.0;

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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	class UAIPerceptionStimuliSourceComponent* StimuliSource;

	void SetupStimulusSource();

	
};
