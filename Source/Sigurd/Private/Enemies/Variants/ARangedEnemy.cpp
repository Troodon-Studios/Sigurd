// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Variants/ARangedEnemy.h"


// Sets default values
AARangedEnemy::AARangedEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AARangedEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARangedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AARangedEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

