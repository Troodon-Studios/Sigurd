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

void AMapGen::PrintMatrix( TArray<TArray<int>> Matrix)
{
    for (int i = 0; i < Matrix.Num(); i++)
    {
        FString Line = "";
        for (int j = 0; j < Matrix[i].Num(); j++)
        {
            if (Matrix[i][j] == 1)
            {
                Line += "o";
            }
            else if (Matrix[i][j] == 0)
            {
                Line += "x";

            }else
            {
                Line += "i";
            }
        }
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Line);
    }
    
    UE_LOG(LogTemp, Warning, TEXT(" "));
    
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

int AMapGen::GetNeighboursCount(TArray<TArray<int>> Matrix) 
{
    int Neighbors = 0;
    for (int i = 0; i < Matrix.Num(); i++)
    {
        for (int j = 0; j < Matrix[i].Num(); j++)
        {
            if (Matrix[i][j])
            {
                Neighbors++;
            }
        }
    }
    return Neighbors;
}


TArray<TArray<int>> AMapGen::GetNeighbours(const int I, const int J) const
{

    TArray<TArray<int>> Matrix;
    Matrix.SetNum(3);
    for (int i = 0; i < 3; i++)
    {
        Matrix[i].SetNum(3, false);
    }
    //set all matrix to false
    
    
    // Vecinos arriba, abajo, izquierda y derecha
    if (I > 0 && ModuleNumbers[I - 1][J] != 0)
    {
        Matrix[0][1] = 1;
    }
    if (I < GridSize.X - 1 && ModuleNumbers[I + 1][J] != 0)
    {
        Matrix[2][1] = 1;
    }
    if (J > 0 && ModuleNumbers[I][J - 1] != 0)
    {
        Matrix[1][0] = 1;
    }
    if (J < GridSize.Y - 1 && ModuleNumbers[I][J + 1] != 0)
    {
        Matrix[1][2] = 1;
    }

    // Vecinos en las esquinas superior derecha, inferior derecha, inferior izquierda y superior izquierda
    if (I > 0 && J < GridSize.Y - 1 && ModuleNumbers[I - 1][J + 1] != 0)
    {
        Matrix[0][2] = 1;
    }
    if (I < GridSize.X - 1 && J < GridSize.Y - 1 && ModuleNumbers[I + 1][J + 1] != 0)
    {
        Matrix[2][2] = 1;
    }
    if (I < GridSize.X - 1 && J > 0 && ModuleNumbers[I + 1][J - 1] != 0)
    {
        Matrix[2][0] = 1;
    }
    if (I > 0 && J > 0 && ModuleNumbers[I - 1][J - 1] != 0)
    {
        Matrix[0][0] = 1;
    }
    
    return Matrix;
}

TArray<TArray<int>> AMapGen::RotateMatrix(const TArray<TArray<int>>& Matrix)
{
    int N = Matrix.Num();
    TArray<TArray<int>> RotatedMatrix;
    RotatedMatrix.SetNum(N);
    for (int i = 0; i < N; i++)
    {
        RotatedMatrix[i].SetNum(N);
        for (int j = 0; j < N; j++)
        {
            RotatedMatrix[i][j] = Matrix[N - j - 1][i];
        }
    }
    return RotatedMatrix;
}

bool AMapGen::CompareMatrix(TArray<TArray<int>> Matrix1, const int PlotNum, int X, int Y) const
{
    TArray<TArray<int>> Matrix2 = ModuleTiles->Matrix[PlotNum];
    Matrix1[1][1] = false;
    Matrix2[1][1] = false;

    for (int i = 0; i <= 3; i++)
    {
        bool isEqual = true;
        for (int j = 0; j < Matrix1.Num(); j++)
        {
            for (int k = 0; k < Matrix1[j].Num(); k++)
            {
                // If the element is 0 or 1, then check if they are equal
                if ((Matrix2[j][k] == 0 || Matrix2[j][k] == 1) && Matrix1[j][k] != Matrix2[j][k])
                {
                    isEqual = false;
                    break;
                }
            }
            if (!isEqual)
            {
                break;
            }
        }
        if (isEqual)
        {
            //ModuleRotations[X][Y] = i;
            return true;
        }
        Matrix2 = RotateMatrix(Matrix2);
    }

    return false;
}

// all nums 1,5,7,17,21,23,29,31,85,87,95,119,127,255
void AMapGen::FigureModulesPosition()
{
    TArray<int> AllNums = { 255,127,119,95,87,85,31,29,23,21,17,7,5,1 };
    
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

                TArray<TArray<int>> Mat = GetNeighbours(x, y);

                bool done = false;
                int N = 0;
                
                do
                {
                    done = CompareMatrix(Mat,AllNums[N],x,y);
                    N++;

                    if (N == AllNums.Num())
                    {
                        UE_LOG(LogTemp, Error, TEXT("---------"));
                        UE_LOG(LogTemp, Error, TEXT("Module %d %d does not fit"), x, y);
                        PrintMatrix(Mat);
                        UE_LOG(LogTemp, Error, TEXT("---------"));

                        done = true;    
                    }
                }
                while (done == false);
                N--;

                ModuleNumbers[x][y] = AllNums[N];

                UE_LOG(LogTemp, Warning, TEXT("Conseguido en %d iteraciones"), N);
                
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
            if(ModuleNumbers[x][y] != 0)
            {

                const FVector Position = FVector(x * ModulesSize.X, y * ModulesSize.Y, 0) - Offset;
                FString ModuleName = FString::Printf(TEXT("Module[%d,%d]_%d"), x, y, ModuleNumbers[x][y]);

                StaticMeshModule = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(ModuleName));
                if (StaticMeshModule)
                {
                    StaticMeshModule->RegisterComponent();
                    StaticMeshModule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
                    StaticMeshModule->SetRelativeLocation(Position);
                    StaticMeshModule->SetRelativeRotation(FRotator(0, 90 * ModuleRotations[x][y], 0));
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


