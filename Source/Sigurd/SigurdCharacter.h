// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
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
class ASigurdCharacter : public ABaseCharacter /*, public IDamageableInterface*/
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FInputActionValues InputActionValues;
	
	ASigurdCharacter();

	//Movement
	void Move(const FInputActionValue& Value);
	void StartRunning();
	void StopRunning();
	
	//Combat	
	
	void StartLightAbility();
	void StartHeavyAbility();

	void StartFirstAbility();
	void StartSecondAbility();
	void StartThirdAbility();
	void StartFourthAbility();

	void EndAbility();

	void StartDodge();
	void StartParry();
	void StartBlock();
	void Block();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamageSigurd(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                      class AController* InstigatedBy, AActor* DamageCauser) ;
	
	/*void TakeDamage_Implementation(float damage) override;*/
	

	
	
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	
	virtual void BeginPlay() override;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const{ return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const{ return FollowCamera; }
};
