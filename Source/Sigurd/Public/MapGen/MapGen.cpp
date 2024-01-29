#include "MapGen.h"

// Sets default values
AMapGen::AMapGen()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // initialize the static mesh component
    
    // Initialize grid dimensions and seed
    GridSize = FVector2D(10, 10);
    ModulesSize = FIntVector(10, 10, 10);
    Seed = 0;
    
}

// Called when the game starts or when spawned
void AMapGen::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void AMapGen::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Function to generate the grid
void AMapGen::GenerateGrid() const
{
    // Get the world
    if (UWorld* World = GetWorld())
    {
        const int Pos_X = GridSize.X;
        const int Pos_Y = GridSize.Y;

        // Calculate the offset based on the size of the modules
        const FVector Offset = FVector(ModulesSize.X / 2.0f, ModulesSize.Y / 2.0f, ModulesSize.Z / 2.0f);

        for (int n = 0 ; n < Pos_X ; n++)
        {
            for (int m = 0 ; m < Pos_Y ; m++)
            {
                // Calculate the position of the module
                const FVector Position = FVector(n * ModulesSize.X, m * ModulesSize.Y, 0) - Offset;

                // spawn the module on the calculated position
                World->SpawnActor<AActor>(Module, Position, FRotator::ZeroRotator);
                
                
            }
        }
    }
}
