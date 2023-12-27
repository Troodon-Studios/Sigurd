// Fill out your copyright notice in the Description page of Project Settings.


#include "HNS_Enemy.h"

// Sets default values
AHNS_Enemy::AHNS_Enemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHNS_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHNS_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHNS_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

