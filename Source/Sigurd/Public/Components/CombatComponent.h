// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapon.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIGURD_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UCombatComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AWeapon> DefaultWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	AWeapon* EquippedWeapon;



	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(FDataTableRowHandle Weapon);

	void LightAttack();
	void HeavyAttack();
	
};
