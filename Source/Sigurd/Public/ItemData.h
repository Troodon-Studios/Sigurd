#pragma once

#include "CoreMinimal.h"
#include "Combat/CombatAbility.h"
#include "Combat/MeleeAttack.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "ItemData.generated.h"

class AWeapon;

UENUM()
enum ESocket {
	RH_Socket UMETA(DisplayName = "Right Hand Socket"),
	LH_Socket UMETA(DisplayName = "Left Hand Socket"),
	Back_Socket UMETA(DisplayName = "Back Socket"),
	Head_Socket UMETA(DisplayName = "Head Socket"),
};
//TODO cambiar a algo de verdad
static inline FName GetSocketName(ESocket Socket) {
	switch (Socket) {
	case ESocket::RH_Socket:
		return "RH_Socket";
	case ESocket::LH_Socket:
		return "LH_Socket";
	case ESocket::Back_Socket:
		return "Back_Socket";
	case ESocket::Head_Socket:
		return "Head_Socket";
	default:
		return "RH_Socket";
	}
}

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESocket> SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> LightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> HeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> FirstAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> SecondAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> ThirdAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCombatAbility> FourthAbility;
};


