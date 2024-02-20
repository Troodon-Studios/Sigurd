// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "PatrolRoute.generated.h"

UCLASS()
class SIGURD_API APatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	bool GetSplinePointsAsWorldPosition(FVector& OutLocation);

	UFUNCTION(BlueprintCallable)
	void IncrementActualIndex();
	
	APatrolRoute();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Patrol Route")
	TObjectPtr<USplineComponent> Route;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Patrol Route")
	bool Loop;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Patrol Route")
	int32 PatrolIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Patrol Route", meta=(ClampMin="-1", ClampMax="1"))
	int32 Direction;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
