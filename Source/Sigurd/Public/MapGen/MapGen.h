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

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Static mesh component
    UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Modules")
    UStaticMeshComponent* StaticMeshModule;
    
    // static mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modules")
    TArray<UStaticMesh*> ModuleMesh;
    
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

    // Function to generate the grid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void GenerateGrid();

    // Function to generate the grid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void FillGrid();
    
};

