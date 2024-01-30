#include "MapGen.h"

// Sets default values
AMapGen::AMapGen(): StaticMeshModule(nullptr), ModuleMaterial(nullptr), ModuleTiles(nullptr)
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
    if (RandomizeSeed)
    {
        Seed = FMath::RandRange(0, 1000);
    }
    
    Super::BeginPlay();
    
}

// Called every frame
void AMapGen::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void AMapGen::GenerateGrid()
{
    // Resize the ModuleNumbers array to match the GridSize
    ModuleNumbers.SetNum(GridSize.X);
    ModuleRotations.SetNum(GridSize.X);
    
    for (int i = 0; i < GridSize.X; i++)
    {
        ModuleNumbers[i].SetNum(GridSize.Y);
        ModuleRotations[i].SetNum(GridSize.Y);
    }

    // Fill the ModuleNumbers array with 0s and 1s based on Perlin noise
    for (int x = 0; x < GridSize.X; x++)
    {
        for (int y = 0; y < GridSize.Y; y++)
        {
            if (x == 0 || y == 0 || x == GridSize.X - 1 || y == GridSize.Y - 1)
            {
                ModuleNumbers[x][y] = 0;
                ModuleRotations[x][y] = 0;
            }else
            {
                // Generate Perlin noise value based on cell position and seed
                const float NoiseValue = FMath::PerlinNoise2D(FVector2D((x / 10.0f) + Seed, (y / 10.0f) + Seed));

                // Map the noise value to the range [0, 1]
                const float MappedValue = (NoiseValue + 1) / 2.0f; // This line is changed

                // Set the corresponding cell in ModuleNumbers to 1 if noise value is greater than 0.5, otherwise 0
                ModuleNumbers[x][y] = (MappedValue > 0.5) ? 1 : 0;

                ModuleRotations[x][y] = 0;
            }
            
        }
    }

    // Call the FillGrid function to create the grid based on ModuleNumbers
    DeleteSmallPlots();
    FigureModulesPosition();

    // After all Grid Generation
    FillGrid();
}

void AMapGen::DeleteSmallPlots()
{
    if (!DeletePlots) return;

    const int Rows = GridSize.X;
    const int Cols = GridSize.Y;

    // Step 1
    TArray<TArray<bool>> Visited;
    Visited.SetNum(Rows);
    for (int i = 0; i < Rows; i++)
    {
        Visited[i].SetNum(Cols, false);
    }

    // Step 2
    int MaxSize = 0;
    TArray<FVector2D> LargestIsland;

    // Step 3
    for (int i = 0; i < Rows; i++)
    {
        for (int j = 0; j < Cols; j++)
        {
            if (!Visited[i][j] && ModuleNumbers[i][j] != 0)
            {
                TArray<FVector2D> CurrentIsland;
                Dfs(i, j, Visited, CurrentIsland);

                // Step 6
                if (CurrentIsland.Num() > MaxSize)
                {
                    MaxSize = CurrentIsland.Num();
                    LargestIsland = CurrentIsland;
                }
            }
        }
    }

    // Step 7
    for (int i = 0; i < Rows; i++)
    {
        for (int j = 0; j < Cols; j++)
        {
            if (!IsInLargestIsland(i, j, LargestIsland))
            {
                ModuleNumbers[i][j] = 0;
            }
        }
    }
}

void AMapGen::Dfs(const int I, const int J, TArray<TArray<bool>>& Visited, TArray<FVector2D>& CurrentIsland)
{
    const int Rows = GridSize.X;

    // Check if (i, j) is a valid cell
    if (const int Cols = GridSize.Y; I < 0 || I >= Rows || J < 0 || J >= Cols || Visited[I][J] || ModuleNumbers[I][J] == 0)
    {
        return;
    }

    // Step 4
    Visited[I][J] = true;
    CurrentIsland.Add(FVector2D(I, J));

    // Step 5
    Dfs(I - 1, J, Visited, CurrentIsland);
    Dfs(I + 1, J, Visited, CurrentIsland);
    Dfs(I, J - 1, Visited, CurrentIsland);
    Dfs(I, J + 1, Visited, CurrentIsland);
}

bool AMapGen::IsInLargestIsland(const int I, const int J, const TArray<FVector2D>& LargestIsland)
{
    for (const FVector2D& Cell : LargestIsland)
    {
        if (Cell.X == I && Cell.Y == J)
        {
            return true;
        }
    }
    return false;
}


FRotator AMapGen::GetDesiredRotation(const int X, const int Y) const
{

    // Rotate the module based on its position in the grid
    if (X == 0)
    {
        // If the module is on the left side of the grid, rotate it 0 degrees on the Z axis
        return FRotator(0, -90, 0);
    }
    else if (Y == 0)
    {
        // If the module is on the bottom side of the grid, rotate it -90 degrees on the Z axis
        return FRotator(0, 0, 0);
    }
    else if (X == GridSize.X - 1)
    {
        // If the module is on the right side of the grid, rotate it 180 degrees on the Z axis
        return FRotator(0, 90, 0);
    }
    else if (Y == GridSize.Y - 1)
    {
        // If the module is on the top side of the grid, rotate it 90 degrees on the Z axis
        return FRotator(0, 180, 0);
    }

    return FRotator(0, 0, 0);
    
}


/*
Case 1:
1

Case 2:
5
7
17

Case 3:
21
23
29
31

Case 4:
85
87
95
119
127
255

*/


// all nums 1,5,7,17,21,23,29,31,85,87,95,119,127,255
void AMapGen::FigureModulesPosition()
{
    // Ensure ModuleNumbers is properly initialized
    if (ModuleNumbers.Num() == 0 || ModuleNumbers[0].Num() == 0)
    {
        // Handle error, perhaps throw an exception or return early
        return;
    }

    for (int x = 0; x < GridSize.X; x++)
    {
        for (int y = 0; y < GridSize.Y; y++)
        {
            if (ModuleNumbers[x][y] != 0)
            {
                // cuenta la cantida de vecinos distintos de 0
                int Neighbors = 0;
                if (x > 0 && ModuleNumbers[x - 1][y] != 0)
                {
                    Neighbors++;
                }
                if (x < GridSize.X - 1 && ModuleNumbers[x + 1][y] != 0)
                {
                    Neighbors++;
                }
                if (y > 0 && ModuleNumbers[x][y - 1] != 0)
                {
                    Neighbors++;
                }
                if (y < GridSize.Y - 1 && ModuleNumbers[x][y + 1] != 0)
                {
                    Neighbors++;
                }

                switch (Neighbors)
                {
                case 0:
                    break;
                case 1:
                    ModuleNumbers[x][y] = 1;
                    break;
                case 2:
                    ModuleNumbers[x][y] = 127;
                    break;
                case 3:
                    ModuleNumbers[x][y] = 31;
                    break;
                case 4:
                    ModuleNumbers[x][y] = 255;
                    break;
                default: break;
                }
            }
                
        }
    }
}

// After all Grid Generation

void AMapGen::FillGrid()
{
    const FVector Offset = FVector(ModulesSize.X / 2.0f, ModulesSize.Y / 2.0f, ModulesSize.Z / 2.0f);

    for (int x = 0; x < GridSize.X; x++)
    {
        for (int y = 0; y < GridSize.Y; y++)
        {
            if(true)
            {

                const FVector Position = FVector(x * ModulesSize.X, y * ModulesSize.Y, 0) - Offset;
                FString ModuleName = FString::Printf(TEXT("Module[%d,%d]_%d"), x, y, ModuleNumbers[x][y]);

                StaticMeshModule = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(ModuleName));
                if (StaticMeshModule)
                {
                    StaticMeshModule->RegisterComponent();
                    StaticMeshModule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
                    StaticMeshModule->SetRelativeLocation(Position);
                    StaticMeshModule->CreationMethod = EComponentCreationMethod::Instance;

                    if (ModuleTiles->ModuleMesh.Contains(ModuleNumbers[x][y]) && ModuleTiles->ModuleMesh[ModuleNumbers[x][y]] != nullptr && UseMesh)
                    {
                        StaticMeshModule->SetStaticMesh(ModuleTiles->ModuleMesh[ModuleNumbers[x][y]]);

                    }
                    else
                    {
                        StaticMeshModule->SetStaticMesh(ModuleMesh[0]);
                    }
                    
                    UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ModuleMaterial, this);

                    
                    
                    if (ModuleTiles->ModuleColor.Contains(ModuleNumbers[x][y]) && UseColor)
                    {
                        DynamicMaterial->SetVectorParameterValue("Color",ModuleTiles->ModuleColor[ModuleNumbers[x][y]]);

                    }
                    else
                    {
                        DynamicMaterial->SetVectorParameterValue("Color", FColor::White);
                    }
                    
                    StaticMeshModule->SetMaterial(0, DynamicMaterial);
                }
                
            }
            
        }
    }
}
