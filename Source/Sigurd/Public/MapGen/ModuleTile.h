#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ModuleTile.generated.h"

UCLASS(BlueprintType)
class UModuleTile : public UDataAsset
{
	GENERATED_BODY()

public:
	
	// Meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, UStaticMesh*> ModuleMesh;

	// Colors
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, FColor> ModuleColor;
	
	// Constructor
	UModuleTile();

private:

	// Variables
	TArray<FColor> UsedColors;
};

