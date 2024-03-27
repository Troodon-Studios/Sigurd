#pragma once
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyStructs.generated.h"

USTRUCT(BlueprintType)
struct FSightValues : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config, meta = (UIMin = 0.0, ClampMin = 0.0))
	float SightRadius = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config, meta = (UIMin = 0.0, ClampMin = 0.0))
	float LoseSightRadius = SightRadius + 500.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config, meta=(UIMin = 0.0, ClampMin = 0.0, UIMax = 180.0, ClampMax = 180.0, DisplayName="PeripheralVisionHalfAngleDegrees"))
	float PeripheralVisionAngleDegrees = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config)
	FAISenseAffiliationFilter DetectionByAffiliation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config)
	float AutoSuccessRangeFromLastSeenLocation = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config, meta = (UIMin = 0.0, ClampMin = 0.0))
	float PointOfViewBackwardOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config, meta = (UIMin = 0.0, ClampMin = 0.0))
	float NearClippingRadius;
};

USTRUCT(BlueprintType)
struct FHearingValues : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", meta = (UIMin = 0.0, ClampMin = 0.0))
	float HearingRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config)
	FAISenseAffiliationFilter DetectionByAffiliation;
	
};

UENUM(BlueprintType)
enum class EDominantSense : uint8
{
	Sight,
	Hear,
	Feel
};

USTRUCT(BlueprintType)
struct FEnemyType : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviourTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (UIMin = 0.0, ClampMin = 0.0))
	double AttackRadius = 150.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (UIMin = 0.0, ClampMin = 0.0))
	double DefendRadius = 350.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (UIMin = 0.0, ClampMin = 0.0))
	float Health = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses")
	bool bCanSee = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses", meta = (EditCondition = "bCanSee",EditConditionHides))
	FSightValues SightValues;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses")
	bool bCanHear = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses", meta = (EditCondition = "bCanHear",EditConditionHides))
	FHearingValues HearingValues;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses")
	bool bCanFeel = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses", meta = (EditCondition = "bCanSee || bCanHear || bCanFeel"))
	EDominantSense DominantSense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses", meta = (EditCondition = "bCanSee || bCanHear || bCanFeel"),  meta = (UIMin = 0.0, ClampMin = 0.0))
	float MemoryTime = 5.0f;
	
};