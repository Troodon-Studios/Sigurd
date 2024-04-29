// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/MeleeWeapon.h"

#include "Characters/BaseCharacter.h"



// Sets default values
AMeleeWeapon::AMeleeWeapon(){
	WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollider"));
	WeaponCollider->SetupAttachment(WeaponMesh);
	WeaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay(){
	Super::BeginPlay();
	
}

