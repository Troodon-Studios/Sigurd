#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "ItemData.generated.h"

static TMap<int, FName> secuenceMap = {
	{0, FName("A1")},
	{1, FName("A2")},
	{2, FName("A3")}
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxComboCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ComboMontage;

	

	
	
};