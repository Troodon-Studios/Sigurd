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
	void GetSplinePointsAsWorldPosition(FVector& Location) const;

	UFUNCTION(BlueprintCallable)
	void IncrementActualIndex();
	
	APatrolRoute();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USplineComponent> Route;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	int32 PatrolIndex;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(ClampMin="-1", ClampMax="1"))
	int32 Direction;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
