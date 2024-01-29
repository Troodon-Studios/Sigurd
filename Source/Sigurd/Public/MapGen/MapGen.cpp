#include "MapGen.h"

// Sets default values
AMapGen::AMapGen(): StaticMeshModule(nullptr), ModuleMesh(nullptr)
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
void AMapGen::GenerateGrid()
{

    const int Pos_X = GridSize.X;
    const int Pos_Y = GridSize.Y;

    // Calculate the offset based on the size of the modules
    const FVector Offset = FVector(ModulesSize.X / 2.0f, ModulesSize.Y / 2.0f, ModulesSize.Z / 2.0f);

    // Loop to spawn each module
    for (int x = 0; x < Pos_X; x++)
    {
        for (int y = 0; y < Pos_Y; y++)
        {
            // Calculate the position of the module
            const FVector Position = FVector(x * ModulesSize.X, y * ModulesSize.Y, 0) - Offset;
                
            // Create a new static mesh component
            StaticMeshModule = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), ("Position: %s", *Position.ToString()));
            if (StaticMeshModule)
            {
                StaticMeshModule->RegisterComponent();
                StaticMeshModule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

                // Set the local position of the static mesh to the calculated position
                StaticMeshModule->SetRelativeLocation(Position);

                StaticMeshModule->CreationMethod = EComponentCreationMethod::Instance;

                if (ModuleMesh)
                {
                    StaticMeshModule->SetStaticMesh(ModuleMesh);
                }
            }
        }
    }
    
}
