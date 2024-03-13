// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "ItemData.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SIGURD_API AWeapon : public AActor{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UBoxComponent* WeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FItemData WeaponData;

	UCombatAbility* LightAttack;
	UCombatAbility* HeavyAttack;

	void SetWeaponData(FItemData NewWeaponData, ABaseCharacter* Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
