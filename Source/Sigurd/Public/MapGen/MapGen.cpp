#include "MapGen.h"

// Sets default values
AMapGen::AMapGen(): StaticMeshModule(nullptr)
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


void AMapGen::GenerateGrid()
{
    // Resize the outer array to match the grid size
    ModuleNumbers.SetNum(GridSize.X);

    // Iterate over each cell in the grid
    for (int x = 0; x < GridSize.X; x++)
    {
        // Resize the inner array to match the grid size
        ModuleNumbers[x].SetNum(GridSize.Y);

        for (int y = 0; y < GridSize.Y; y++)
        {
            // Check if the cell is on the edge of the grid
            if (x == 0 || y == 0 || x == GridSize.X - 1 || y == GridSize.Y - 1)
            {
                // Check if the cell is in one of the four corners of the grid
                if ((x == 0 && y == 0) || (x == 0 && y == GridSize.Y - 1) || (x == GridSize.X - 1 && y == 0) || (x == GridSize.X - 1 && y == GridSize.Y - 1))
                {
                    // If it is, set the corresponding cell in ModuleNumbers to 3
                    ModuleNumbers[x][y] = 2;
                }
                else
                {
                    // If it's not, set the corresponding cell in ModuleNumbers to 1
                    ModuleNumbers[x][y] = 1;
                }
            }
            else
            {
                // If it's not on the edge, set the corresponding cell in ModuleNumbers to 0
                ModuleNumbers[x][y] = 0;
            }
        }
    }
}

void AMapGen::FillGrid()
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

            //make Text module[x,y]
            FString ModuleName = FString::Printf(TEXT("Module[%d,%d]"), x, y);
            
            // Create a new static mesh component
            StaticMeshModule = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(ModuleName));
            if (StaticMeshModule)
            {
                StaticMeshModule->RegisterComponent();
                StaticMeshModule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

                // Set the local position of the static mesh to the calculated position
                StaticMeshModule->SetRelativeLocation(Position);

                StaticMeshModule->CreationMethod = EComponentCreationMethod::Instance;


                StaticMeshModule->SetStaticMesh(ModuleMesh[ModuleNumbers[x][y]]);
                

                // Rotate the module based on its position in the grid
                if (x == 0)
                {
                    // If the module is on the left side of the grid, rotate it 0 degrees on the Z axis
                    StaticMeshModule->SetRelativeRotation(FRotator(0, -90, 0));
                }
                else if (y == 0)
                {
                    // If the module is on the bottom side of the grid, rotate it -90 degrees on the Z axis
                    StaticMeshModule->SetRelativeRotation(FRotator(0, 0, 0));
                }
                else if (x == Pos_X - 1)
                {
                    // If the module is on the right side of the grid, rotate it 180 degrees on the Z axis
                    StaticMeshModule->SetRelativeRotation(FRotator(0, 90, 0));
                }
                else if (y == Pos_Y - 1)
                {
                    // If the module is on the top side of the grid, rotate it 90 degrees on the Z axis
                    StaticMeshModule->SetRelativeRotation(FRotator(0, 180, 0));
                }
            }
        }
    }
}