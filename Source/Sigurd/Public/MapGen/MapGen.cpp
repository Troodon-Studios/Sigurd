#include "MapGen.h"

// Sets default values
AMapGen::AMapGen(): StaticMeshModule(nullptr), ModuleMaterial(nullptr), ModuleTiles(nullptr)
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;
    
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

void AMapGen::Generate()
{

    UE_LOG(LogTemp, Warning, TEXT("Started..."));
    Offset = FVector(ModulesSize.X / 2.0f, ModulesSize.Y / 2.0f, ModulesSize.Z / 2.0f);

    UE_LOG(LogTemp, Warning, TEXT("Generating a new grid"));
    GenerateGrid();

    UE_LOG(LogTemp, Warning, TEXT("Grid Generated, deleting small plots"));
    DeleteSmallPlots();

    UE_LOG(LogTemp, Warning, TEXT("Small plots deleted, figuring modules position"));
    FigureModulesPosition();

    UE_LOG(LogTemp, Warning, TEXT("Modules positioned"));
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


void AMapGen::FigureModulesPosition()
{
    const auto Start = std::chrono::high_resolution_clock::now();

    // Create a map associating the number of neighbors with the corresponding numbers
    TMap<int, TArray<int>> NeighborsNumbersMap;
    NeighborsNumbersMap.Add(1, {1});
    NeighborsNumbersMap.Add(2, {5, 17, 1});
    NeighborsNumbersMap.Add(3, {7, 21, 5, 1, 17});
    NeighborsNumbersMap.Add(4, {29, 85, 23, 5, 7, 17, 21});
    NeighborsNumbersMap.Add(5, {31, 87, 7, 17, 21, 23, 29});
    NeighborsNumbersMap.Add(6, {95, 119, 17, 23, 29, 31});
    NeighborsNumbersMap.Add(7, {127, 31});
    NeighborsNumbersMap.Add(8, {255});

    if (ModuleNumbers.Num() == 0 || ModuleNumbers[0].Num() == 0) return;

    TArray<TFuture<void>> Futures;
    int ThreadCount = 0;

    for (int x = 0; x < GridSize.X; x++)
    {
        for (int y = 0; y < GridSize.Y; y++)
        {
            if (ModuleNumbers[x][y] != 0)
            {
                Futures.Add(Async(EAsyncExecution::ThreadPool, [this, x, y, &NeighborsNumbersMap]()
                {
                    const TArray<TArray<int>> Mat = MatrixFunctions.GetNeighbours(GridSize,x, y,ModuleNumbers);
                    int N = 0;
                    const TArray<int>& AllNums = NeighborsNumbersMap[MatrixFunctions.GetNeighboursCount(Mat)]; // Get the corresponding numbers from the map
                    while (N < AllNums.Num() && !MatrixFunctions.CompareMatrix(Mat,AllNums[N],x,y,ModuleRotations)) N++;
                    if (N == AllNums.Num())
                    {
                        UE_LOG(LogTemp, Error, TEXT("Module %d %d does not fit"), x, y);
                        MatrixFunctions.PrintMatrix(Mat);
                    }
                    else ModuleNumbers[x][y] = AllNums[N];
                }));
                ThreadCount++;
                SpawnModule(x, y);
            }
        }
    }

    for(auto& Future : Futures) Future.Get();

    const auto Stop = std::chrono::high_resolution_clock::now();
    const auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(Stop - Start);

    UE_LOG(LogTemp, Warning, TEXT("Execution time: %lld microseconds"), Duration.count());
    UE_LOG(LogTemp, Warning, TEXT("Number of threads used: %d"), ThreadCount);
}

void AMapGen::SpawnModule(const int X, const int Y)
{
    const FVector Position = FVector(X * ModulesSize.X, Y * ModulesSize.Y, 0) - Offset;
    const FString ModuleName = FString::Printf(TEXT("Module[%d,%d]_%d"), X, Y, ModuleNumbers[X][Y]);

    StaticMeshModule = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(ModuleName));
    if (!StaticMeshModule) return;

    StaticMeshModule->RegisterComponent();
    StaticMeshModule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    StaticMeshModule->SetRelativeLocation(Position);
    StaticMeshModule->SetRelativeRotation(FRotator(0, 90 * ModuleRotations[X][Y], 0));
    StaticMeshModule->CreationMethod = EComponentCreationMethod::Instance;

    UStaticMesh* MeshToUse = ModuleMesh[0];
    if (ModuleTiles->ModuleMesh.Contains(ModuleNumbers[X][Y]) && ModuleTiles->ModuleMesh[ModuleNumbers[X][Y]] != nullptr && UseMesh)
    {
        MeshToUse = ModuleTiles->ModuleMesh[ModuleNumbers[X][Y]];
    }
    StaticMeshModule->SetStaticMesh(MeshToUse);

    UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ModuleMaterial, this);
    FColor ColorToUse = FColor::White;
    if (ModuleTiles->ModuleColor.Contains(ModuleNumbers[X][Y]) && UseColor)
    {
        ColorToUse = ModuleTiles->ModuleColor[ModuleNumbers[X][Y]];
    }
    DynamicMaterial->SetVectorParameterValue("Color", ColorToUse);
    StaticMeshModule->SetMaterial(0, DynamicMaterial);
}