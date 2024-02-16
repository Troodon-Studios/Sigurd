// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Weapon.h"

#include "ItemData.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}



void AWeapon::UpdateWeaponMesh()
{
	FName rowName = weaponData.Item.RowName;
	const FItemData* ItemData = weaponData.Item.GetRow<FItemData>(FString::Printf(TEXT("%s"), *rowName.ToString()));
	if (ItemData)
	{
		WeaponMesh->SetStaticMesh(ItemData->Mesh);
	}
	else
	{
	}
}

void AWeapon::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);
	UpdateWeaponMesh();
}
