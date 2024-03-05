/**
* @file TokenComponent.h
 * @brief This file contains the declaration of the UTokenComponent class and the FToken struct.
 *
 * The UTokenComponent class is a custom ActorComponent that can be spawned in blueprints. It manages tokens, 
 * which are represented by the FToken struct. Each token has a unique ID and an owner, which is a UTokenComponent.
 * The UTokenComponent class provides functionality to reserve tokens, return tokens...
 * It also maintains a queue of casters, each associated with a quantity of tokens.
 * Tokens are used to limit the number of NPCs that can attack other Pawn at the same time.
 * 
 * The FToken struct represents a token with a unique ID and an owner.
 */

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TokenComponent.generated.h"

struct FToken;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTokenComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTokenComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Token")
	int32 MaxTokens = 3;

	UFUNCTION(BlueprintCallable, Category="Token")
	bool ReserveTokens(UTokenComponent* Caster, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category="Token")
	void ReturnTokens(UTokenComponent* Owner);

	UFUNCTION(BlueprintCallable, Category="Token")
	void ReturnAllTokens();

	UFUNCTION(BlueprintCallable, Category="Token")
	void PrintTokenInfo();

	UFUNCTION(BlueprintCallable, Category="Token")
	int32 GetAviablleTokens(UTokenComponent* Target) const;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	void AddToQueue(UTokenComponent* Caster, int32 Quantity);
	void GiveToken(UTokenComponent* Caster, int32 Quantity);
	void ReceiveToken(FToken* Token);
	static void RemoveTokens(const UTokenComponent* Owner, TArray<FToken*>& TokensArray);

	TArray<FToken*> Tokens;
	TArray<FToken*> AvailableTokens;
	TMap<UTokenComponent*, int32> CastersQueue;
};

USTRUCT(BlueprintType)
struct FToken
{
	explicit FToken(const int ID = 0, UTokenComponent* NewOwner = nullptr) : UniqueID(ID), Owner(NewOwner)
	{
	}

	GENERATED_BODY()

	uint32 UniqueID;
	UTokenComponent* Owner;
};
