// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class SIGURD_API AMeleeWeapon : public AWeapon{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeleeWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UBoxComponent* WeaponCollider;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
