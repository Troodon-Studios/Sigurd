#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ModuleTile.generated.h"

UCLASS(BlueprintType)
class UModuleTile : public UDataAsset
{
	GENERATED_BODY()

public:

	TMap<int,TArray<TArray<bool>>> Matrix;

	//Tmapo de static mesh por numero de modulo seteadas a null de inicio
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modules")
	TMap<int, UStaticMesh*> ModuleMesh;

	// Colors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modules")
	TMap<int, FColor> ModuleColor;
	
	// Constructor
	UModuleTile();

private:

	// Variables
	TArray<FColor> UsedColors;
	
	// Functions
	static FColor SetUpColors(const int ModuleNumber);
	static TArray<TArray<bool>> SetUpMatrix(int ModuleNumber);

};

