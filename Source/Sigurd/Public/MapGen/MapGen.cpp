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
    Generate();
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
    
    UE_LOG(LogTemp, Warning, TEXT("Modules positioned, generating extras"));
    GenerateExtras();

    UE_LOG(LogTemp, Warning, TEXT("Extras generated, MapGen finished"));
    
    // Print the ModuleNumbers array
    //MatrixFunctions.PrintMatrix(ModuleNumbers);
    
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
    
    StaticMeshModule = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(ModuleName));
    if (StaticMeshModule)
    {
        StaticMeshModule->RegisterComponent();
        StaticMeshModule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        StaticMeshModule->SetWorldLocation(Position);
        StaticMeshModule->SetRelativeRotation(Rotation);
        StaticMeshModule->CreationMethod = EComponentCreationMethod::Instance;

        if (ModuleTiles->ModuleMesh.Contains(ModuleNumber) && ModuleTiles->ModuleMesh[ModuleNumber] != nullptr && UseMesh)
        {
            StaticMeshModule->SetStaticMesh(ModuleTiles->ModuleMesh[ModuleNumber]);

        }
        else
        {
            StaticMeshModule->SetStaticMesh(ModuleMesh[0]);
        }
                    
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ModuleMaterial, this);
                    
        if (ModuleTiles->ModuleColor.Contains(ModuleNumber) && UseColor)
        {
            DynamicMaterial->SetVectorParameterValue("Color",ModuleTiles->ModuleColor[ModuleNumber]);

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
    // Obtener el Pawn del jugador
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!PlayerPawn) return;

    // Inicializar la distancia mínima con un valor muy grande
    float MinDistance = FLT_MAX;

    //ModuleNumbers[x][y];
    FVector NearestModule = FVector(0,0,0);
    bool Found = false;
    FVector NewPosition;
    
    // Iterar a través de todos los módulos para encontrar el más cercano al origen
    for (int x = 0; x < GridSize.X; x++)
    {
        for (int y = 0; y < GridSize.Y; y++)
        {
            if (ModuleNumbers[x][y] == 255)
            {
                // Calcular la distancia entre el módulo actual y el origen
                const FVector Position = FVector(x * ModulesSize.X, y * ModulesSize.Y, 0) - Offset;
                // Calcular la distancia entre el módulo actual y el origen del mundo
                
                const float Distance = FVector::DistSquared(Position, FVector::ZeroVector);
                // Si la distancia es menor que la distancia mínima actual, actualizar la distancia mínima y el módulo más cercano
                if (Distance < MinDistance)
                {
                    MinDistance = Distance;
                    NewPosition = Position;
                    NewPosition.Z = 800;
                    NearestModule = FVector(x, y, 0);
                    Found = true;
                }
            }
        }
    }
    
    // Si encontramos un módulo 255
    if (Found)
    {
        // Mover el Pawn del jugador 100 unidades por encima del módulo 255 más cercano al origen
        const FRotator Rotation = FRotator(0, 90 * ModuleRotations[NearestModule.X][NearestModule.Y], 0);

        PlayerPawn->SetActorLocation(NewPosition);
        //PlayerPawn->SetActorRotation(Rotation);
    }
}