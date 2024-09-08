#pragma once
#include "CoreMinimal.h"
#include "ModuleTile.h"
#include "Engine/DataTable.h"
#include "NoiseSettings.generated.h"


USTRUCT(BlueprintType)
struct FNoiseValues : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "10.0"), DisplayName = "Frequency")
	float Frequency = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"), DisplayName = "Amplitude")
	float Amplitude = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "10.0"), DisplayName = "Lacunarity")
	float Lacunarity = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"), DisplayName = "Persistence")
	float Persistence = 0.5f;
	
};

USTRUCT(BlueprintType)
struct FNoiseSetting : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, DisplayName = "Noise Values")
	FNoiseValues NoiseValues;

	// UModuleTile DataAsset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UModuleTile* ModuleTiles;

};

USTRUCT(BlueprintType)
struct FTextureSetting : public FTableRowBase
{
	GENERATED_BODY()

	//UPROPERTY( EditAnywhere, BlueprintReadOnly, DisplayName = "Name")
	FString Name = "Texture";

	UPROPERTY( EditAnywhere, BlueprintReadWrite, DisplayName = "Opacity", meta = (ClampMin = "1.0", ClampMax = "100.0"))
	float Opacity = 100;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, DisplayName = "Scatter", meta = (ClampMin = "1.0", ClampMax = "100.0"))
	float Scatter = 10;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, DisplayName = "Noise Values")
	FNoiseValues NoiseValues;

};

USTRUCT(BlueprintType)
struct FElementsValue : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Grass = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Gravel = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Metal = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Rug = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Sand = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Snow = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Stone = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Water = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Wood = 0.0f;
};

UENUM(BlueprintType)
enum class EMaterialTextureType : uint8
{
	Base,
	Detail,
	Extra,
	Any
};


USTRUCT(BlueprintType)
struct FMaterialTextures : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Diffuse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ARM;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Normal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float Scale = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMaterialTextureType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FElementsValue ElementsValue;

	FColor* MipData;
	
};