// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/PatrolRoute.h"

bool APatrolRoute::GetSplinePointsAsWorldPosition(FVector& OutLocation)
{
    if (Route != nullptr && Route->GetNumberOfSplinePoints() > 0)
    {
        OutLocation = Route->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
        return true;
    }
    return false;
}
void APatrolRoute::IncrementActualIndex()
{
	const int NumPoints = Route->GetNumberOfSplinePoints();
    PatrolIndex = (PatrolIndex + Direction + NumPoints) % NumPoints;

    if (!Loop && (PatrolIndex == 0 || PatrolIndex == NumPoints - 1))
    {
        Direction *= -1;
    }
}

APatrolRoute::APatrolRoute(): Loop(true), PatrolIndex(0), Direction(1)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Route = CreateDefaultSubobject<USplineComponent>(TEXT("RouteObject"));
	Route->SetupAttachment(RootComponent);
	
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

