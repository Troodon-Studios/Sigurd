// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/WeaponActor.h"


// Sets default values
AWeaponActor::AWeaponActor(){

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(WeaponMesh);

}


