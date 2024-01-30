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
    for (int i = 0; i < GridSize.X; i++)
    {
        ModuleNumbers[i].SetNum(GridSize.Y);
    }

    // Fill the ModuleNumbers array with 0s and 1s based on Perlin noise
    for (int x = 0; x < GridSize.X; x++)
    {
        for (int y = 0; y < GridSize.Y; y++)
        {
            // Generate Perlin noise value based on cell position and seed
            const float NoiseValue = FMath::PerlinNoise2D(FVector2D((x / 10.0f) + Seed, (y / 10.0f) + Seed));

            // Map the noise value to the range [0, 1]
            const float MappedValue = (NoiseValue + 1) / 2.0f; // This line is changed

            // Set the corresponding cell in ModuleNumbers to 1 if noise value is greater than 0.5, otherwise 0
            ModuleNumbers[x][y] = (MappedValue > 0.5) ? 1 : 0;

            // print on screen the MappedValue
        }
    }

    // Call the FillGrid function to create the grid based on ModuleNumbers
    DeleteSmallPlots();
    FillGrid();
}

void AMapGen::DeleteSmallPlots()
{
    if (!DeletePlots) return;

    int rows = GridSize.X;
    int cols = GridSize.Y;

    // Step 1
    TArray<TArray<bool>> visited;
    visited.SetNum(rows);
    for (int i = 0; i < rows; i++)
    {
        visited[i].SetNum(cols, false);
    }

    // Step 2
    int maxSize = 0;
    TArray<FVector2D> largestIsland;

    // Step 3
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (!visited[i][j] && ModuleNumbers[i][j] != 0)
            {
                TArray<FVector2D> currentIsland;
                DFS(i, j, visited, currentIsland);

                // Step 6
                if (currentIsland.Num() > maxSize)
                {
                    maxSize = currentIsland.Num();
                    largestIsland = currentIsland;
                }
            }
        }
    }

    // Step 7
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (!IsInLargestIsland(i, j, largestIsland))
            {
                ModuleNumbers[i][j] = 0;
            }
        }
    }
}

void AMapGen::DFS(int i, int j, TArray<TArray<bool>>& visited, TArray<FVector2D>& currentIsland)
{
    int rows = GridSize.X;
    int cols = GridSize.Y;

    // Check if (i, j) is a valid cell
    if (i < 0 || i >= rows || j < 0 || j >= cols || visited[i][j] || ModuleNumbers[i][j] == 0)
    {
        return;
    }

    // Step 4
    visited[i][j] = true;
    currentIsland.Add(FVector2D(i, j));

    // Step 5
    DFS(i - 1, j, visited, currentIsland);
    DFS(i + 1, j, visited, currentIsland);
    DFS(i, j - 1, visited, currentIsland);
    DFS(i, j + 1, visited, currentIsland);
}

bool AMapGen::IsInLargestIsland(int i, int j, const TArray<FVector2D>& largestIsland)
{
    for (const FVector2D& cell : largestIsland)
    {
        if (cell.X == i && cell.Y == j)
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

auto AMapGen::GetDesiredColor(const int Pos) -> FLinearColor
{

    switch (Pos)
    {
    case 0:
        // Cambiar el color del material a blanco
            return FLinearColor::White;
        break;
    case 1:
        // Cambiar el color del material a negro
            return FLinearColor::Gray;
        break;
    case 2:
        // Cambiar el color del material a rojo
            return FLinearColor::Blue;
        break;
    default:
        // En caso de que el valor no sea 0, 1 o 2, establecer el color a gris
            return FLinearColor::White;
        break;
    }

    return  FLinearColor::White;
}



void AMapGen::FillGrid()
{
    const FVector Offset = FVector(ModulesSize.X / 2.0f, ModulesSize.Y / 2.0f, ModulesSize.Z / 2.0f);

    for (int x = 0; x < GridSize.X; x++)
    {
        for (int y = 0; y < GridSize.Y; y++)
        {
            if(ModuleNumbers[x][y] != 0)
            {

                const FVector Position = FVector(x * ModulesSize.X, y * ModulesSize.Y, 0) - Offset;
                FString ModuleName = FString::Printf(TEXT("Module[%d,%d]-%d"), x, y, ModuleNumbers[x][y]);

                StaticMeshModule = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(ModuleName));
                if (StaticMeshModule)
                {
                    StaticMeshModule->RegisterComponent();
                    StaticMeshModule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
                    StaticMeshModule->SetRelativeLocation(Position);
                    StaticMeshModule->CreationMethod = EComponentCreationMethod::Instance;
                    StaticMeshModule->SetStaticMesh(ModuleMesh[0]);

                    UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ModuleMaterial, this);
                    DynamicMaterial->SetVectorParameterValue("Color", GetDesiredColor(ModuleNumbers[x][y]));
                    StaticMeshModule->SetMaterial(0, DynamicMaterial);
                }
                
            }
            
        }
    }
}
