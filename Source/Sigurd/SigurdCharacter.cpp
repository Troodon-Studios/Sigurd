// Copyright Epic Games, Inc. All Rights Reserved.

#include "SigurdCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/InputSettings.h"
#include "Misc/OutputDeviceNull.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASigurdCharacter

ASigurdCharacter::ASigurdCharacter(){
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	ConditionComponent = CreateDefaultSubobject<UConditionComponent>(TEXT("ConditionComponent"));
	TokenComponent = CreateDefaultSubobject<UTokenComponent>(TEXT("TokenComponent"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("RH_Socket"));

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
}

void ASigurdCharacter::BeginPlay(){
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

	if (ConditionComponent){
		ConditionComponent->RegisterComponent();
	}

	OnTakeAnyDamage.AddDynamic(this, &ASigurdCharacter::TakeDamageSigurd);
	
	if (APlayerController* playerController = Cast<APlayerController>(Controller)){
		playerController->bShowMouseCursor = true;
		playerController->DefaultMouseCursor = EMouseCursor::Default;
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())){
			Subsystem->AddMappingContext(InputActionValues.DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASigurdCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)){


		EnhancedInputComponent->BindAction(InputActionValues.MoveAction, ETriggerEvent::Triggered, this, &ASigurdCharacter::Move);

		EnhancedInputComponent->BindAction(InputActionValues.RunAction, ETriggerEvent::Started, this, &ASigurdCharacter::StartRunning);

		EnhancedInputComponent->BindAction(InputActionValues.LightAttackAction , ETriggerEvent::Started, this, &ASigurdCharacter::LightAttack);
		EnhancedInputComponent->BindAction(InputActionValues.HeavyAttackAction, ETriggerEvent::Started, this, &ASigurdCharacter::HeavyAttack);
		


		EnhancedInputComponent->BindAction(InputActionValues.NextWeaponAction, ETriggerEvent::Started, this,
		                                   &ASigurdCharacter::NextWeapon);
		EnhancedInputComponent->BindAction(InputActionValues.PreviousWeaponAction, ETriggerEvent::Started, this,
		                                   &ASigurdCharacter::PreviousWeapon);

		EnhancedInputComponent->BindAction(InputActionValues.DodgeAction, ETriggerEvent::Started, this, &ASigurdCharacter::Dodge);
		EnhancedInputComponent->BindAction(InputActionValues.BlockAction, ETriggerEvent::Started, this, &ASigurdCharacter::Block);
	}
	else{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void ASigurdCharacter::Move(const FInputActionValue& Value){

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr){
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void ASigurdCharacter::StartRunning(){
	StaminaComponent->RunAction();
}


void ASigurdCharacter::LightAttack(){
	CombatComponent->Attack();
}

void ASigurdCharacter::HeavyAttack(){
	CombatComponent->Attack();
}


void ASigurdCharacter::TakeDamageSigurd(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                        class AController* InstigatedBy, AActor* DamageCauser) {
	UObject* ObjectInstance = const_cast<UObject*>(static_cast<const UObject*>(DamageType));
	CombatComponent->TakeDamage(Damage, ObjectInstance);
}

/*void ASigurdCharacter::TakeDamage_Implementation(float damage){
	IDamageableInterface::TakeDamage_Implementation(damage);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CustomTakeDamage_Implementation"));
}*/

void ASigurdCharacter::NextWeapon() {
	if (CombatComponent->WeaponInventory.Num() != 0){
		CombatComponent->NextWeapon();
		WeaponMesh->SetStaticMesh(CombatComponent->WeaponInventory[CombatComponent->CurrentWeapon].Mesh);
	}
}

void ASigurdCharacter::PreviousWeapon() {
	if (CombatComponent->WeaponInventory.Num() != 0){
		CombatComponent->PreviousWeapon();
		WeaponMesh->SetStaticMesh(CombatComponent->WeaponInventory[CombatComponent->CurrentWeapon].Mesh);
	}
}

void ASigurdCharacter::Dodge() {
	CombatComponent->Dodge();
}

void ASigurdCharacter::Block() {
	CombatComponent->Block();
}
