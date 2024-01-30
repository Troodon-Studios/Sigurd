#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
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

    // Function to generate the grid
    void FillGrid();
    auto GetDesiredRotation(int X, int Y) const -> FRotator;
    static auto GetDesiredColor(int Pos) -> FLinearColor;
    void DeleteSmallPlots();
    void DFS(int i, int j, TArray<TArray<bool>>& visited, TArray<FVector2D>& currentIsland);
    bool IsInLargestIsland(int i, int j, const TArray<FVector2D>& largestIsland);
    
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

    // Array to store the number of each module for generating the grid
    TArray<TArray<int>> ModuleNumbers;
    
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

