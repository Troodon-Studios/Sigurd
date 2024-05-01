#include "MapGen.h"

#include "AssetToolsModule.h"
#include "HighResScreenshot.h"
#include "ImageUtils.h"
#include "TextureGen.h"
#include "AssetRegistry/AssetRegistryModule.h"


/**
 * Constructor for the AMapGen class.
 * Initializes the class variables.
 */
AMapGen::AMapGen(): Setting(nullptr), StaticMeshModule(nullptr), GeneratedMap(nullptr), ProceduralMesh(nullptr),
                    AuxiliarMesh(nullptr),
                    ModuleMaterial(nullptr),
                    bGenerate(false),
                    MeshSectionIndex(0)
{
	PrimaryActorTick.bCanEverTick = true;
	GridSize = FVector2D(10, 10);
	Seed = 0;
}

/**
 * Called when the game starts or when spawned.
 * Initializes the RandomizeSeed variable and calls the Generate function.
 */
void AMapGen::BeginPlay()
{
	Super::BeginPlay();
	RandomizeSeed = false;
	Generate();
}

/**
 * Generates the map.
 * If RandomizeSeed is true, a random seed is generated.
 * Retrieves the noise settings and generates the grid.
 * Deletes small plots and figures out the modules' positions.
 * Generates extras.
 */
void AMapGen::Generate()
{
	if (RandomizeSeed)
	{
		Seed = FMath::RandRange(0, 1000);
	}

	Setting = NoiseSettingsTable.Setting.GetRow<FNoiseSetting>(
		FString::Printf(TEXT("%s"), *NoiseSettingsTable.Setting.RowName.ToString()));

	TextureSettings = TArray<FTextureSetting*>();
	TextureSettings.Empty();
	
	for (auto& SettingHandle : TextureSettingsTable.Settings)
	{
		if (FTextureSetting* TmpSetting = SettingHandle.GetRow<FTextureSetting>(FString::Printf(TEXT("%s"), *SettingHandle.RowName.ToString())))
	    {
	        TextureSettings.Add(TmpSetting);
	    }
	}

	if (TextureSettings.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Texture settings not found"));
		GenerateTextures = false;
	}
	
	if (!Setting)
	{
		UE_LOG(LogTemp, Error, TEXT("Noise setting not found"));
		return;
	}

	ModulesSize = Setting->ModuleTiles->ModulesSize;
	Offset = FVector(ModulesSize.X / 2.0f, ModulesSize.Y / 2.0f, ModulesSize.Z / 2.0f);

	GenerateGrid();
	DeleteSmallPlots();
	MeshSectionIndex = 0;

	if (!GeneratedMap || !ProceduralMesh)
	{
		GeneratedMap = GetWorld()->SpawnActor<APackedLevelActor>({500, 500, 0}, FRotator::ZeroRotator);
	}

	if (ProceduralMesh)
	{
		ProceduralMesh->DestroyComponent();
	}

	ProceduralMesh = NewObject<UProceduralMeshComponent>(GeneratedMap);
	ProceduralMesh->RegisterComponent();
	ProceduralMesh->AttachToComponent(GeneratedMap->GetRootComponent(),
	                                  FAttachmentTransformRules::KeepRelativeTransform);

	FigureModulesPosition();
	GenerateExtras();
}

/**
 * Generates the grid for the map.
 * Initializes the ModuleNumbers and ModuleRotations arrays.
 * Calculates the noise value for each cell in the grid.
 */
void AMapGen::GenerateGrid()
{
	const float MFrequency = Setting->NoiseValues.Frequency;
	const float MAmplitude = Setting->NoiseValues.Amplitude;
	const float MLacunarity = Setting->NoiseValues.Lacunarity;
	const float MPersistence = Setting->NoiseValues.Persistence;

	ModuleNumbers.SetNum(GridSize.X);
	ModuleRotations.SetNum(GridSize.X);


	// GRID
	for (int i = 0; i < GridSize.X; i++)
	{
		ModuleNumbers[i].SetNum(GridSize.Y);
		ModuleRotations[i].SetNum(GridSize.Y);
	}

	for (int x = 0; x < GridSize.X; x++)
	{
		for (int y = 0; y < GridSize.Y; y++)
		{
			float NoiseValue = FNoise::SimplexNoise((x / 10.0f) + Seed, (y / 10.0f) + Seed, MFrequency, MAmplitude,
			                                        MLacunarity, MPersistence);

			if (x == 0 || y == 0 || x == GridSize.X - 1 || y == GridSize.Y - 1)
			{
				ModuleNumbers[x][y] = 0;
				ModuleRotations[x][y] = 0;
			}
			else
			{
				const float MappedValue = (NoiseValue + 1) / 2.0f;
				ModuleNumbers[x][y] = (MappedValue > 0.5) ? 1 : 0;
				ModuleRotations[x][y] = 0;
			}
		}
	}
}

/**
 * Deletes small plots from the map.
 * Uses a depth-first search to find the largest island and keeps only that.
 */
void AMapGen::DeleteSmallPlots()
{
	if (!DeletePlots) return;

	const int Rows = GridSize.X;
	const int Cols = GridSize.Y;

	TArray<TArray<bool>> Visited;
	Visited.SetNum(Rows);
	for (int i = 0; i < Rows; i++)
	{
		Visited[i].SetNum(Cols, false);
	}

	int MaxSize = 0;
	TArray<FVector2D> LargestIsland;

	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			if (!Visited[i][j] && ModuleNumbers[i][j] != 0)
			{
				TArray<FVector2D> CurrentIsland;
				Dfs(i, j, Visited, CurrentIsland);

				if (CurrentIsland.Num() > MaxSize)
				{
					MaxSize = CurrentIsland.Num();
					LargestIsland = CurrentIsland;
				}
			}
		}
	}

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

/**
 * Performs a depth-first search from the given cell.
 * Used to find all cells in the same island.
 */
void AMapGen::Dfs(const int I, const int J, TArray<TArray<bool>>& Visited, TArray<FVector2D>& CurrentIsland)
{
	const int Rows = GridSize.X;
	const int Cols = GridSize.Y;

	if (I < 0 || I >= Rows || J < 0 || J >= Cols || Visited[I][J] || ModuleNumbers[I][J] == 0)
	{
		return;
	}

	Visited[I][J] = true;
	CurrentIsland.Add(FVector2D(I, J));

	Dfs(I - 1, J, Visited, CurrentIsland);
	Dfs(I + 1, J, Visited, CurrentIsland);
	Dfs(I, J - 1, Visited, CurrentIsland);
	Dfs(I, J + 1, Visited, CurrentIsland);
}

/**
 * Checks if a cell is in the largest island.
 */
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

/**
 * Figures out the position of the modules.
 * Compares the neighbors of each cell with the predefined patterns.
 * If a match is found, the module is spawned or merged.
 */
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
				const TArray<int>& AllNums = MatrixFunctions.NeighborsNumbersMap[MatrixFunctions.
					GetNeighboursCount(Mat)];
				while (N < AllNums.Num() && !MatrixFunctions.CompareMatrix(Mat, AllNums[N], x, y, ModuleRotations)) N++;
				if (N == AllNums.Num())
				{
					MatrixFunctions.PrintMatrix(Mat, true);
				}
				else
				{
					ModuleNumbers[x][y] = AllNums[N];
					const FVector Position = FVector(x * ModulesSize.X, y * ModulesSize.Y, 0) - Offset;
					const FRotator Rotation = FRotator(0, 90 * ModuleRotations[x][y], 0);

					if (MergeMeshes)
					{
						MergeMesh(AllNums[N], Position, Rotation);
					}
					else
					{
						SpawnModule(AllNums[N], Position, Rotation);
					}
				}
			}
		}
	}

	const auto Stop = std::chrono::high_resolution_clock::now();
	const auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(Stop - Start);

	UE_LOG(LogTemp, Warning, TEXT("Execution time: %lld microseconds"), Duration.count());
}

/**
 * Spawns a module at the given position with the given rotation.
 * The module's mesh and color are determined by the ModuleNumber.
 */
void AMapGen::SpawnModule(const int ModuleNumber, const FVector& Position, const FRotator& Rotation)
{
	const FString ModuleName = FString::Printf(TEXT("Module[%d,%d]_%d"), static_cast<int>(Position.X),
	                                           static_cast<int>(Position.Y), ModuleNumber);

	StaticMeshModule = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(ModuleName));
	if (StaticMeshModule)
	{
		StaticMeshModule->RegisterComponent();
		StaticMeshModule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		StaticMeshModule->SetWorldLocation(Position);
		StaticMeshModule->SetRelativeRotation(Rotation);
		StaticMeshModule->CreationMethod = EComponentCreationMethod::Instance;

		if (Setting->ModuleTiles->ModuleMesh.Contains(ModuleNumber) && Setting->ModuleTiles->ModuleMesh[ModuleNumber] !=
			nullptr && UseMesh)
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
			DynamicMaterial->SetVectorParameterValue("Color", Setting->ModuleTiles->ModuleColor[ModuleNumber]);
		}
		else
		{
			DynamicMaterial->SetVectorParameterValue("Color", FColor::White);
		}

		StaticMeshModule->SetMaterial(0, DynamicMaterial);
	}
}

/**
 * Merges a module at the given position with the given rotation.
 * The module's mesh is determined by the ModuleNumber.
 * If ApplyPostNoise is true, the module's height is adjusted based on the noise value.
 */
void AMapGen::MergeMesh(const int ModuleNumber, const FVector& Position, const FRotator& Rotation)
{
	const float MFrequency = PostNoiseValues.Frequency;
	const float MAmplitude = PostNoiseValues.Amplitude;
	const float MLacunarity = PostNoiseValues.Lacunarity;
	const float MPersistence = PostNoiseValues.Persistence;

	FTransform ActorTransform = FTransform(Rotation, Position, FVector(1, 1, 1));
	UStaticMesh* MeshToUse = Setting->ModuleTiles->ModuleMesh.Contains(ModuleNumber) && Setting->ModuleTiles->ModuleMesh
	                         [ModuleNumber] != nullptr && UseMesh
		                         ? Setting->ModuleTiles->ModuleMesh[ModuleNumber]
		                         : AuxiliarMesh;

	if (!MeshToUse) return;

	FRawMesh RawMesh;
	MeshToUse->GetSourceModel(0).LoadRawMesh(RawMesh);

	TArray<FVector> VertexPositions;
	TArray<int32> WedgeIndices;
	TArray<FVector> WedgeTangentZ;
	TArray<FVector2D> WedgeTexCoords;
	TArray<FProcMeshTangent> WedgeTangentX;

	for (const auto& Vertex : RawMesh.VertexPositions)
	{
		FVector TransformedVertex = ActorTransform.TransformPosition(
			TArray<UE::Math::TVector<double>>::ElementType(Vertex));
		if (ApplyPostNoise && TransformedVertex.Z <= ZThreshold)
		{
			const float NoiseValue = FNoise::SimplexNoise((TransformedVertex.X / 10.0f) + Seed,
			                                              (TransformedVertex.Y / 10.0f) + Seed, MFrequency, MAmplitude,
			                                              MLacunarity, MPersistence);
			TransformedVertex.Z += NoiseValue * PostNoiseAmount;
			//UE_LOG(LogTemp, Warning, TEXT("Noise value: %f"), NoiseValue);
		}
		VertexPositions.Add(TransformedVertex);
	}
	for (const auto Index : RawMesh.WedgeIndices)
	{
		WedgeIndices.Add(static_cast<int32>(Index));
	}
	for (const auto& TangentZ : RawMesh.WedgeTangentZ)
	{
		WedgeTangentZ.Add(ActorTransform.TransformVector(TArray<UE::Math::TVector<double>>::ElementType(TangentZ)));
	}
	for (const auto& TexCoord : RawMesh.WedgeTexCoords[0])
	{
		WedgeTexCoords.Add(TArray<UE::Math::TVector2<double>>::ElementType(TexCoord));
	}
	for (const auto& TangentX : RawMesh.WedgeTangentX)
	{
		WedgeTangentX.Add(FProcMeshTangent(ActorTransform.TransformVector(FVector(TangentX)), false));
	}

	ProceduralMesh->CreateMeshSection_LinearColor(MeshSectionIndex, VertexPositions, WedgeIndices, WedgeTangentZ,
	                                              WedgeTexCoords, TArray<FLinearColor>(), WedgeTangentX, true);
	ProceduralMesh->SetMaterial(MeshSectionIndex, ModuleMaterial);

	MeshSectionIndex++;
}

/**
 * Generates extras for the map.
 * Moves the player pawn to the nearest module with a number of 255.
 */
void AMapGen::GenerateExtras()
{

	// Generate Textures
	FTextureGen::NewTexture(GridSize, ExtraDim, Seed, "MapTexture_Det.png", Setting->NoiseValues);
	FTextureGen::NewTexture(GridSize, ExtraDim, Seed, "MapTexture_Glob.png", TextNoiseValues);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn) return;

	float MinDistance = FLT_MAX;
	bool Found = false;
	FVector NewPosition;

	for (int x = 0; x < GridSize.X; x++)
	{
		for (int y = 0; y < GridSize.Y; y++)
		{
			if (ModuleNumbers[x][y] == 255)
			{
				const FVector Position = FVector(x * ModulesSize.X, y * ModulesSize.Y, 0) - Offset;
				if (const float Distance = FVector::DistSquared(Position, FVector::ZeroVector); Distance < MinDistance)
				{
					MinDistance = Distance;
					NewPosition = Position;
					NewPosition.Z = 800;
					Found = true;
				}
			}
		}
	}

	if (Found)
	{
		PlayerPawn->SetActorLocation(NewPosition);
	}
}


/**
 * Called after a property was changed in the editor.
 * If the bGenerate property was changed, it calls the ExecuteGenerate function.
 */
#if WITH_EDITOR
void AMapGen::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr && PropertyChangedEvent.Property->GetFName() ==
		GET_MEMBER_NAME_CHECKED(AMapGen, bGenerate))
	{
		if (bGenerate)
		{
			ExecuteGenerate();
			bGenerate = false;
		}
	}
}
#endif
