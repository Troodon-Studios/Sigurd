// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon.h"

#include "Characters/BaseCharacter.h"


// Sets default values
AWeapon::AWeapon(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(WeaponMesh);
}

void AWeapon::SetWeaponData(FItemData NewWeaponData, ABaseCharacter* Character){
	WeaponData = NewWeaponData;
	WeaponMesh->SetStaticMesh(WeaponData.Mesh);
	LightAttack = NewObject<UCombatAbility>(this, WeaponData.LightAttack);
	LightAttack->Initialize(Character);
	HeavyAttack = NewObject<UCombatAbility>(this, WeaponData.HeavyAttack);
	HeavyAttack->Initialize(Character);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay(){
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

