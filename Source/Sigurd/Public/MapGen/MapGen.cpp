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

    FillGrid();
    
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

                
                // Set the static mesh of the static mesh component to the module mesh
                //StaticMeshModule->SetStaticMesh(ModuleMesh[ModuleNumbers[x][y]]);

                //Set Mesh 0 with colors
                StaticMeshModule->SetStaticMesh(ModuleMesh[0]);
                
                // Crear un nuevo material dinámico a partir de un material base
                UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ModuleMaterial, this);


                // Set the color parameter of the dynamic material to the desired color
                DynamicMaterial->SetVectorParameterValue("Color", GetDesiredColor(ModuleNumbers[x][y]));
                
                // Establecer el material del componente de malla estática
                StaticMeshModule->SetMaterial(0, DynamicMaterial);

                //SetRotation
                StaticMeshModule->SetRelativeRotation(GetDesiredRotation(x,y));
                
            }
        }
    }
}
