#pragma once
#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ModuleTile.h"
#include "MapGen.generated.h"

using namespace std;

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
    TArray<TArray<int>> ModuleRotations;
    
    // Functions
    void FillGrid();
    int GetMapSize();
    static auto PrintMatrix(TArray<TArray<int>> Matrix) -> void;
    TArray<TArray<int>> GetNeighbours(int I, int J) const;
    bool CompareMatrix(TArray<TArray<int>> Matrix1, int PlotNum,int X, int Y);
    static TArray<TArray<int>> RotateMatrix(const TArray<TArray<int>>& Matrix);
    static int GetNeighboursCount(TArray<TArray<int>> Matrix);
    void DeleteSmallPlots();
    void Dfs(int I, int J, TArray<TArray<bool>>& Visited, TArray<FVector2D>& CurrentIsland);
    static bool IsInLargestIsland(int I, int J, const TArray<FVector2D>& LargestIsland);
    void FigureModulesPosition();

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

//// Modules

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

//// Grid

    // Grid dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FVector2D GridSize;

    // Seed for random generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int Seed;
    
    // Randomize seed for generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool DeletePlots = false;
    
    // Function to generate the grid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void GenerateGrid();

    // Function to generate the grid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void Generate();

    
    
    
//// Settings

    // Randomize seed for generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool RandomizeSeed = false;

    // Use mesh on each module
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool UseMesh = false;

    // Use color on each module
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool UseColor = false;

    // Min numer of plots walkable
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    int MinPlots;


//// DEBUG
    
};

