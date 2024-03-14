// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/MeleeWeapon.h"

#include "Characters/BaseCharacter.h"



// Sets default values
AMeleeWeapon::AMeleeWeapon(){
	WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollider"));
	WeaponCollider->SetupAttachment(WeaponMesh);

	WeaponCollider->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnWeaponColliderBeginOverlap);

}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay(){
	Super::BeginPlay();
	
}

void AMeleeWeapon::OnWeaponColliderBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){

	if (OtherActor == Cast<AActor>(Owner))
		return;

	//print other actor
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName());


}



