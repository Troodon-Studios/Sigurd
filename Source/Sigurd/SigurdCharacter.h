// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon.h"
#include "Components/CombatComponent.h"
#include "Components/ConditionComponent.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/TokenComponent.h"
#include "DataTypes/PlayerInputAction.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageableInterface.h"
#include "Logging/LogMacros.h"
#include "SigurdCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASigurdCharacter : public ACharacter /*, public IDamageableInterface*/
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	UStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Condition", meta = (AllowPrivateAccess = "true"))
	UConditionComponent* ConditionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UTokenComponent* TokenComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FInputActionValues InputActionValues;

public:
	ASigurdCharacter();

protected:

	//Movement
	void Move(const FInputActionValue& Value);
	void StartRunning() ;

	//Combat	
	void LightAttack() ;
	void HeavyAttack() ;
	UFUNCTION()
	void TakeDamageSigurd(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                      class AController* InstigatedBy, AActor* DamageCauser) ;
	/*void TakeDamage_Implementation(float damage) override;*/
	void Dodge() ;
	void Block() ;

	//Inventory
	void NextWeapon() ;
	void PreviousWeapon() ;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const{ return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const{ return FollowCamera; }
};
