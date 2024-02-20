// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CombatComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/CapsuleComponent.h"
#include "PatrolRoute.h"
#include "AICBaseEnemy.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS()
class SIGURD_API ABaseEnemy : public ACharacter
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

	
public:
	// Sets default values for this character's properties
	ABaseEnemy();

	// AI Variables
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviourTree;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI")
	AAIcBaseEnemy* AIController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI")
	TObjectPtr<APatrolRoute> PatrolRoute;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void TakeDamageEnemy(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
										class AController* InstigatedBy, AActor* DamageCauser);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
