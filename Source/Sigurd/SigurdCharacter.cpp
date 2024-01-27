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

ASigurdCharacter::ASigurdCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	ResourcesComponent = CreateDefaultSubobject<UResourcesComponent>(TEXT("ResourcesComponent"));
		
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
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	
}

void ASigurdCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (ResourcesComponent) {
		ResourcesComponent->RegisterComponent();
	}

	OnTakeAnyDamage.AddDynamic(this, &ASigurdCharacter::TakeDamage);

	//Add Input Mapping Context
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		playerController->bShowMouseCursor = true;
		playerController->DefaultMouseCursor = EMouseCursor::Default;
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASigurdCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Moving
		EnhancedInputComponent->BindAction(MoveByClickAction, ETriggerEvent::Triggered, this, &ASigurdCharacter::MoveClick);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASigurdCharacter::MoveAxis);

		//Combat
		//quick mele
		EnhancedInputComponent->BindAction(QckMeleAction, ETriggerEvent::Started, this, &ASigurdCharacter::QuickAttack);

		//heavy mele
		EnhancedInputComponent->BindAction(HvyMeleAction, ETriggerEvent::Triggered, this, &ASigurdCharacter::HeavyAttack);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ASigurdCharacter::StartRunning);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASigurdCharacter::StopRunning);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASigurdCharacter::MoveAxis(const FInputActionValue& Value)
{
	if (CanMove==false || MoveByClick==true)
	{
		return;
	}
	
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
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

void ASigurdCharacter::MoveClick(const FInputActionValue& Value)
{
	if (CanMove==false || MoveByClick==false)
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Move by click"));
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, Hit);

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	ACharacter* ControlledCharacter = GetCharacterMovement()->GetCharacterOwner();
	if (ControlledCharacter != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledCharacter->GetActorLocation()).GetSafeNormal();
		ControlledCharacter->AddMovementInput(WorldDirection, 1.0, false);
	}

	return;

}

void ASigurdCharacter::QuickAttack(const FInputActionValue& Value)
{
	// TODO check stamina and if can attack
	if (CanAttack==false)
	{
		return;
	}

	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (playerController->IsInputKeyDown(EKeys::Gamepad_FaceButton_Left))
		{
			//CanAttack = false;
			FOutputDeviceNull ar;
			this->CallFunctionByNameWithArguments(TEXT("MeleAttack2"), ar, NULL, true);
		}
		else
		{
			//CanAttack = false;
			FOutputDeviceNull ar;
			this->CallFunctionByNameWithArguments(TEXT("MeleAttack"), ar, NULL, true);
		}
	}
	

	
}

void ASigurdCharacter::HeavyAttack(const FInputActionValue& Value)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Attack: %f"), Value.Get<float>());

	if (Value.Get<float>() == 0 )
	{

		if (HeavyMeleValue > 0.2f)
		{
			//HeavyMeleValue = 1;
			FOutputDeviceNull ar;
			this->CallFunctionByNameWithArguments(TEXT("HeavyAttack"), ar, NULL, true);
			//CanAttack = false;

		}
		else 
		{
			HeavyMeleValue = 0;
		}
		
	}
	else if( HeavyMeleValue < 1 && CanAttack==true)
	{

		//increase heavy attack value by delta time
		HeavyMeleValue += GetWorld()->GetDeltaSeconds();
		if (HeavyMeleValue > 0.2f)
		{
			CanMove = false;
		}
		
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Value: %f"), HeavyMeleValue);

}

void ASigurdCharacter::TakeDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, class AController *InstigatedBy, AActor *DamageCauser) {

	if (ResourcesComponent)
	{
		UObject* ObjectInstance = const_cast<UObject*>(static_cast<const UObject*>(DamageType));
		ResourcesComponent->TakeDamageWithType(ObjectInstance ,Damage);
	}
}

