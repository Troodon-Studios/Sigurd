// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AICBaseEnemy.h"


// Sets default values
AIcBaseEnemy::AIcBaseEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

}

// Called when the game starts or when spawned
void AIcBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AIcBaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

