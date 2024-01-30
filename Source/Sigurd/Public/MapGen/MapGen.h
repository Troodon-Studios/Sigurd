#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ModuleTile.h"
#include "MapGen.generated.h"


UCLASS()
class AMapGen : public AActor
{
    GENERATED_BODY()

public:	
    // Sets default values for this actor's properties
    AMapGen();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:


    // Variables
    TArray<TArray<int>> ModuleNumbers;

    //tmap of colors
    TMap<int, FColor> ModuleColors;

    
    // Functions
    void InitializeTiles();
    void FillGrid();
    auto GetDesiredRotation(int X, int Y) const -> FRotator;
    void DeleteSmallPlots();
    void Dfs(int I, int J, TArray<TArray<bool>>& Visited, TArray<FVector2D>& CurrentIsland);
    static bool IsInLargestIsland(int I, int J, const TArray<FVector2D>& LargestIsland);
    void FigureModulesPosition();

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Static mesh component
    UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Modules")
    UStaticMeshComponent* StaticMeshModule;
    
    // static mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modules")
    TArray<UStaticMesh*> ModuleMesh;
    
    //Material
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modules")
    UMaterial* ModuleMaterial;
    
    // Grid dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modules")
    FIntVector ModulesSize;
    
    // UModuleTile DataAsset
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modules")
    UModuleTile* ModuleTiles;
    
    // Grid dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FVector2D GridSize;

    // Seed for random generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int Seed;

    // Randomize seed for generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool RandomizeSeed = false;

    // Randomize seed for generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool DeletePlots = false;
    
    
    // Function to generate the grid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void GenerateGrid();

    
};

