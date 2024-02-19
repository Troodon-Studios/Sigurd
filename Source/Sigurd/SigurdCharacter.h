// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ResourcesComponent.h"
#include "Components/StaminaComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageableInterface.h"
#include "Logging/LogMacros.h"
#include "SigurdCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASigurdCharacter : public ACharacter /*, public IDamageableInterface*/
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	//weapon mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UResourcesComponent> BP_ResourcesComponentClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	UStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveByClickAction;

	/** Combat Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QckMeleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HvyMeleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NextWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PreviousWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BlockAction;
	
	/** CanMove and CanAttack booleans */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actions, meta = (AllowPrivateAccess = "true"))
	bool CanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actions, meta = (AllowPrivateAccess = "true"))
	bool CanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actions, meta = (AllowPrivateAccess = "true"))
	bool MoveByClick = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actions, meta = (AllowPrivateAccess = "true"))
	float HeavyMeleValue = 0;
	
public:
	ASigurdCharacter();
	

protected:

	/** Called for movement input */
	void MoveAxis(const FInputActionValue& Value);
	void MoveClick(const FInputActionValue& Value);

	void Attack();
	void QuickAttack(const FInputActionValue& Value);
	void HeavyAttack(const FInputActionValue& Value);

	UFUNCTION(Category = "Combat")
	void TakeDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, class AController *InstigatedBy, AActor *DamageCauser);

	/*void TakeDamage_Implementation(float damage) override;*/

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void NextWeapon();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PreviousWeapon();

	void StartRunning();
	void StopRunning();
	void CheckExhaustion();

	void Dodge();
	void Block();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	FVector CachedDestination;
	float FollowTime; // For how long it has been pressed
};

