#pragma once
#include <vector>

#include "CoreMinimal.h"
#include "MatrixFunctions.h"
#include "GameFramework/Actor.h"
#include "Async/Async.h"
#include "NoiseSettings.h"
#include "Components/StaticMeshComponent.h"
#include "NoiseSettingsTable.h"
#include "ProceduralMeshComponent.h"
#include "RawMesh.h"
#include "PackedLevelActor/PackedLevelActor.h"
#include "NoiseGenerator.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include <chrono>

#include "MapGen.generated.h"

class UFNoiseSettings;
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
    const FNoiseSetting* Setting;
    UStaticMeshComponent* StaticMeshModule;
    FVector ModulesSize;
    AActor* GeneratedMap;
    UProceduralMeshComponent* ProceduralMesh;
    
    // Functions
    void DeleteSmallPlots();
    void Dfs(int I, int J, TArray<TArray<bool>>& Visited, TArray<FVector2D>& CurrentIsland);
    static bool IsInLargestIsland(int I, int J, const TArray<FVector2D>& LargestIsland);
    void FigureModulesPosition();
    void SpawnModule(const int ModuleNumber, const FVector& Position, const FRotator& Rotation);
    void MergeMesh(const int ModuleNumber, const FVector& Position, const FRotator& Rotation);
    void GenerateExtras();
    
public:
    
    // Called every frame
    virtual void Tick(const float DeltaTime) override
    {
        Super::Tick(DeltaTime);
    }

    // Sets default values for this actor's properties
    AMapGen();
    
//// Modules
    
    // static mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modules")
    UStaticMesh* AuxiliarMesh;
    
    //Material
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modules")
    UMaterial* ModuleMaterial;
    
    
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (DisplayName ="Delete Small Plots" ,ToolTip = "Eliminate all inaccessible areas of the map for the player, considering the largest accessible portion"))
    bool DeletePlots = true;
    
    // Randomize seed for generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ToolTip = "Randomize the seed for map generation each time a new map is Generated"))
    bool RandomizeSeed = false;

    // Use color on each module
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = ( ToolTip = "Use a static mesh for each module / Spawn each module"))
    bool MergeMeshes = false;

    // Use color on each module
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "MergeMeshes", EditConditionHides, DisplayName = "Apply PostGen Noise" , ToolTip = "Enhance the final generated mesh by introducing noise to the vertices of the mesh, adding more detail"))
    bool ApplyPostNoise = true;

    // Use color on each module
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "MergeMeshes && ApplyPostNoise", EditConditionHides, DisplayName = "PostGen Noise Amount", ToolTip = "Amount of noise to be added to the mesh"))
    float PostNoiseAmount = 10000;

    // Use color on each module
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "MergeMeshes && ApplyPostNoise", EditConditionHides, DisplayName = "PostGen Noise Threshold", ToolTip = "Threshold for the noise vertex greater than this value will be not be affected by the noise"))
    float ZThreshold = 200;
    
    UPROPERTY( EditAnywhere, BlueprintReadWrite,Category = "Settings", DisplayName = "Post Noise Values", meta = (EditCondition = "MergeMeshes && ApplyPostNoise", EditConditionHides, ToolTip = "Noise values to be used for the post generation noise"))
    FNoiseValues PostNoiseValues;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings",meta=(DisplayName = "Use Mesh", ToolTip = "Use a different mesh for each module / Use Auxiliar mesh for all modules"))
    bool UseMesh = true;

    //meta=( EditCondition = "!MergeMeshes", DisplayName = "Use Mesh")
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta=(DisplayName = "Use Color", ToolTip = "Use a different color for each module type / Use ModuleMaterial Default"))
    bool UseColor = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FNoiseSettingsTable NoiseSettings;
    
//// Editor Buttons
    
    UFUNCTION(CallInEditor, meta=(ToolTip = "Generate the map"), Category = "Generation")
    void ExecuteGenerate()
    {
        Generate();
    }

    bool bGenerate;
    int32 MeshSectionIndex;


#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};


