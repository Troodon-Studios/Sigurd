#pragma once
#include <vector>

#include "CoreMinimal.h"
#include "MatrixFunctions.h"
#include "GameFramework/Actor.h"
#include "Async/Async.h"
#include "Components/StaticMeshComponent.h"
#include "ModuleTile.h"
#include "NoiseGenerator.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include <chrono>
#include "MapGen.generated.h"

using namespace std;

UCLASS()
class AMapGen : public AActor
{
    GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    
    // Variables
    TArray<TArray<int>> ModuleNumbers;
    TArray<TArray<int>> ModuleRotations;
    FMatrixFunctions MatrixFunctions = FMatrixFunctions();
    FVector Offset;
    
    // Functions
    void DeleteSmallPlots();
    void Dfs(int I, int J, TArray<TArray<bool>>& Visited, TArray<FVector2D>& CurrentIsland);
    static bool IsInLargestIsland(int I, int J, const TArray<FVector2D>& LargestIsland);
    void FigureModulesPosition();
    void SpawnModule(const int ModuleNumber, const FVector& Position, const FRotator& Rotation);
    void GenerateExtras();
    
public:
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    // Sets default values for this actor's properties
    AMapGen();
    
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

    // Grid dimensions, make it so than in editor you can lock the aspect ratio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FVector2D GridSize;

    // Seed for random generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int Seed;
    
    // Function to generate the grid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void GenerateGrid();

    // Function to generate the grid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void Generate();

//// Settings

    // Delete small plots
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", DisplayName = "Delete Small Plots")
    bool DeletePlots = false;
    
    // Randomize seed for generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool RandomizeSeed = false;

    // Use mesh on each module
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool UseMesh = false;

    // Use color on each module
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool UseColor = false;

//// Noise Settings

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings", meta = (ClampMin = "0.0", ClampMax = "1.0"), DisplayName = "Frequency")
    float MFrequency = 0.5f; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings", meta = (ClampMin = "0.0", ClampMax = "1.0"), DisplayName = "Amplitude")
    float MAmplitude = 0.3f;  

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings", meta = (ClampMin = "0.0", ClampMax = "10.0"), DisplayName = "Lacunarity")
    float MLacunarity = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings", meta = (ClampMin = "0.0", ClampMax = "1.0"), DisplayName = "Persistence")
    float MPersistence = 0.5f;


    
    
};