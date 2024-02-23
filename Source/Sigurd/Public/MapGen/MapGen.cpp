#include "MapGen.h"

#include "PackedLevelActor/PackedLevelActor.h"


// Sets default values
AMapGen::AMapGen(): Setting(nullptr), StaticMeshModule(nullptr), AuxiliarMesh(nullptr), ModuleMaterial(nullptr)
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Initialize grid dimensions and seed
    GridSize = FVector2D(10, 10);
    Seed = 0;
}


// Called when the game starts or when spawned
void AMapGen::BeginPlay()
{
    Super::BeginPlay();
    RandomizeSeed = false;
    Generate();

}

// Called every frame
void AMapGen::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMapGen::Generate()
{
    if (RandomizeSeed)
    {
        Seed = FMath::RandRange(0, 1000);
    }
    
    Setting = NoiseSettings.Setting.GetRow<FNoiseSetting>(FString::Printf(TEXT("%s"), *NoiseSettings.Setting.RowName.ToString()));

    if (!Setting)
    {
        UE_LOG(LogTemp, Error, TEXT("Noise setting not found"));
        return;
    }

    ModulesSize = Setting->ModuleTiles->ModulesSize;
    
    UE_LOG(LogTemp, Warning, TEXT("Started..."));
    Offset = FVector(ModulesSize.X / 2.0f, ModulesSize.Y / 2.0f, ModulesSize.Z / 2.0f);

    UE_LOG(LogTemp, Warning, TEXT("Generating a new grid"));
    GenerateGrid();

    UE_LOG(LogTemp, Warning, TEXT("Grid Generated, deleting small plots"));
    DeleteSmallPlots();

    if (GeneratedMap)
    {
        GeneratedMap->Destroy();
    }

    GeneratedMap = GetWorld()->SpawnActor<APackedLevelActor>(APackedLevelActor::StaticClass(), FVector::ZeroVector,
                                                             FRotator::ZeroRotator);

    UE_LOG(LogTemp, Warning, TEXT("Small plots deleted, figuring modules position"));
    FigureModulesPosition();
    
    UE_LOG(LogTemp, Warning, TEXT("Modules positioned, generating extras"));
    GenerateExtras();

    UE_LOG(LogTemp, Warning, TEXT("Extras generated, MapGen finished"));
    
    // Print the ModuleNumbers array
    //MatrixFunctions.PrintMatrix(ModuleNumbers);
    
}

#if WITH_EDITOR
void AMapGen::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property != nullptr && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AMapGen, bGenerate))
    {
        if (bGenerate)
        {
            ExecuteGenerate();
            bGenerate = false;
        }
    }
}
#endif

void AMapGen::GenerateGrid()
{
    const float MFrequency = Setting->Frequency;
    const float MAmplitude = Setting->Amplitude;
    const float MLacunarity = Setting->Lacunarity;
    const float MPersistence = Setting->Persistence;
    
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
                const float NoiseValue = FNoise::SimplexNoise((x / 10.0f) + Seed, (y / 10.0f) + Seed,MFrequency, MAmplitude, MLacunarity, MPersistence);

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

    if (ModuleNumbers.Num() == 0 || ModuleNumbers[0].Num() == 0) return;
    
    for (int x = 0; x < GridSize.X; x++)
    {
        for (int y = 0; y < GridSize.Y; y++)
        {
            if (ModuleNumbers[x][y] != 0)
            {
                const TArray<TArray<int>> Mat = MatrixFunctions.GetNeighbours(GridSize, x, y, ModuleNumbers);
                int N = 0;
                const TArray<int>& AllNums = MatrixFunctions.NeighborsNumbersMap[MatrixFunctions.GetNeighboursCount(Mat)]; // Get the corresponding numbers from the map
                while (N < AllNums.Num() && !MatrixFunctions.CompareMatrix(Mat,AllNums[N],x,y,ModuleRotations)) N++;
                if (N == AllNums.Num())
                {
                    UE_LOG(LogTemp, Error, TEXT("Module %d %d does not fit"), x, y);
                    MatrixFunctions.PrintMatrix(Mat, true);
                }
                else
                {
                    ModuleNumbers[x][y] = AllNums[N];
                    const FVector Position = FVector(x * ModulesSize.X, y * ModulesSize.Y, 0) - Offset;
                    const FRotator Rotation = FRotator(0, 90 * ModuleRotations[x][y], 0);
                    SpawnModule(AllNums[N],Position, Rotation);
                }
            }
        }
    }

    const auto Stop = std::chrono::high_resolution_clock::now();
    const auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(Stop - Start);

    UE_LOG(LogTemp, Warning, TEXT("Execution time: %lld microseconds"), Duration.count());
}

void AMapGen::SpawnModule(const int ModuleNumber, const FVector& Position, const FRotator& Rotation)
{
    
    const FString ModuleName = FString::Printf(TEXT("Module[%d,%d]_%d"), static_cast<int>(Position.X), static_cast<int>(Position.Y), ModuleNumber);
    
    StaticMeshModule = NewObject<UStaticMeshComponent>(GeneratedMap, UStaticMeshComponent::StaticClass(), FName(ModuleName));
    if (StaticMeshModule)
    {
        StaticMeshModule->RegisterComponent();
        StaticMeshModule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        StaticMeshModule->SetWorldLocation(Position);
        StaticMeshModule->SetRelativeRotation(Rotation);
        StaticMeshModule->CreationMethod = EComponentCreationMethod::Instance;

        if (Setting->ModuleTiles->ModuleMesh.Contains(ModuleNumber) && Setting->ModuleTiles->ModuleMesh[ModuleNumber] != nullptr && UseMesh)
        {
            StaticMeshModule->SetStaticMesh(Setting->ModuleTiles->ModuleMesh[ModuleNumber]);

        }
        else
        {
            StaticMeshModule->SetStaticMesh(AuxiliarMesh);
        }
                    
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ModuleMaterial, this);
                    
        if (Setting->ModuleTiles->ModuleColor.Contains(ModuleNumber) && UseColor)
        {
            DynamicMaterial->SetVectorParameterValue("Color",Setting->ModuleTiles->ModuleColor[ModuleNumber]);

        }
        else
        {
            DynamicMaterial->SetVectorParameterValue("Color", FColor::White);
        }
                    
        StaticMeshModule->SetMaterial(0, DynamicMaterial);
    }
}

void AMapGen::GenerateExtras()
{
    // Get the player's pawn
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    // If the player's pawn is not found, return immediately
    if (!PlayerPawn) return;

    // Initialize the minimum distance to the maximum possible float value
    float MinDistance = FLT_MAX;

    // Initialize the nearest module to (0,0,0)
    FVector NearestModule = FVector(0,0,0);
    // Initialize the found flag to false
    bool Found = false;
    // Initialize the new position
    FVector NewPosition;

    // Iterate over the grid size in both x and y directions
    for (int x = 0; x < GridSize.X; x++)
    {
        for (int y = 0; y < GridSize.Y; y++)
        {
            // If the module number at the current grid position is 255
            if (ModuleNumbers[x][y] == 255)
            {
                // Calculate the position
                const FVector Position = FVector(x * ModulesSize.X, y * ModulesSize.Y, 0) - Offset;

                // Calculate the distance from the origin (0,0,0)
                if (const float Distance = FVector::DistSquared(Position, FVector::ZeroVector); Distance < MinDistance)
                {
                    // If this distance is less than the current minimum distance
                    // Update the minimum distance, the new position, the nearest module, and set the found flag to true
                    MinDistance = Distance;
                    NewPosition = Position;
                    NewPosition.Z = 800;
                    NearestModule = FVector(x, y, 0);
                    Found = true;
                }
            }
        }
    }

    // If a module was found (i.e., the found flag is true)
    if (Found)
    {
        // Set the player pawn's location to the new position
        FRotator(0, 90 * ModuleRotations[NearestModule.X][NearestModule.Y], 0);
        PlayerPawn->SetActorLocation(NewPosition);
    }
}


