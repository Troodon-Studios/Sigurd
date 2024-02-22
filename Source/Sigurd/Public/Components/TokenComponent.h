// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TokenComponent.generated.h"


struct FToken;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGURD_API UTokenComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTokenComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Token")
	int32 MaxTokens = 3;

	UFUNCTION(BlueprintCallable, Category="Token")
	void ReserveTokens(UTokenComponent* Caster, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category="Token")
	void ReturnTokens(UTokenComponent* Owner);

	UFUNCTION(BlueprintCallable, Category="Token")
	void ReturnAllTokens();

	UFUNCTION(BlueprintCallable, Category="Token")
	void PrintTokenInfo();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

private:

	void AddToQueue(UTokenComponent* Caster, int32 Quantity);
	void GiveToken(UTokenComponent* Caster, int32 Quantity);
	void ReceiveToken(FToken* Token);
	static void RemoveTokens(const UTokenComponent* Owner, TArray<FToken*>& TokensArray);

	TArray<FToken*> Tokens;
	TArray<FToken*> AviableTokens;
	TMap<UTokenComponent*, int32> CastersQueue;
	
};


USTRUCT(BlueprintType)
struct FToken
{
	explicit FToken(const int ID = 0, UTokenComponent* NewOwner = nullptr) : UniqueID(ID), Owner(NewOwner) {}

	GENERATED_BODY()

	uint32 UniqueID;
	UTokenComponent *Owner;

};