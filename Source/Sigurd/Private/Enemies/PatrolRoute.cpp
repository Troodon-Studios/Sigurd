// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/PatrolRoute.h"

void APatrolRoute::GetSplinePointsAsWorldPosition(FVector& Location) const
{
	Location = Route->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}

void APatrolRoute::IncrementActualIndex()
{
	
	PatrolIndex += Direction;
	if (PatrolIndex >= Route->GetNumberOfSplinePoints())
	{
		PatrolIndex = 0;
	}
	else if (PatrolIndex < 0)
	{
		PatrolIndex = Route->GetNumberOfSplinePoints() - 1;
	}
	
}

APatrolRoute::APatrolRoute(): PatrolIndex(0), Direction(0)
{

	Route = CreateDefaultSubobject<USplineComponent>(TEXT("Route"));
	
}

// Called when the game starts or when spawned
void APatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrolRoute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

