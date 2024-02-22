#include "Components/TokenComponent.h"

UTokenComponent::UTokenComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	
	const uint32 StartID = GetUniqueID();

	for (int i = 0; i < MaxTokens; i++)
	{
		Tokens.Add(new FToken(StartID + i +1,this));
	}
	
}

void UTokenComponent::ReserveTokens(UTokenComponent* Caster, const int32 Quantity)
{
	Quantity > Tokens.Num() ? AddToQueue(Caster, Quantity) : GiveToken(Caster, Quantity);
}

void UTokenComponent::ReturnTokens(UTokenComponent* Owner)
{
	RemoveTokens(Owner, AviableTokens);
}

void UTokenComponent::ReturnAllTokens()
{
	RemoveTokens(nullptr, AviableTokens);
}

void UTokenComponent::RemoveTokens(const UTokenComponent* Owner, TArray<FToken*>& TokensArray)
{
	TArray<FToken*> TokensToRemove;
	for (FToken* Token : TokensArray)
	{
		if (Token->Owner == Owner || Owner == nullptr)
		{
			Token->Owner->ReceiveToken(Token);
			TokensToRemove.Add(Token);
		}
	}
	for (FToken* Token : TokensToRemove) TokensArray.Remove(Token);
}

void UTokenComponent::PrintTokenInfo()
{
	for (const FToken* Token : Tokens)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%d, Owner: %s"), Token->UniqueID,*Token->Owner->GetName()));
	}

	for (const FToken* Token : AviableTokens)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d, Owner: %s"), Token->UniqueID,*Token->Owner->GetName()));
	}
	
}

void UTokenComponent::AddToQueue(UTokenComponent* Caster, const int32 Quantity)
{
	CastersQueue.Contains(Caster) ? CastersQueue[Caster] += Quantity : CastersQueue.Add(Caster, Quantity);
}

void UTokenComponent::GiveToken(UTokenComponent* Caster, const int32 Quantity)
{
	for (int i = 0; i < Quantity; i++)
	{
		Caster->ReceiveToken(Tokens[0]);
		Tokens.RemoveAt(0);
	}

	if (CastersQueue.Contains(Caster))
	{
		CastersQueue[Caster] -= Quantity;
		if (CastersQueue[Caster] == 0) CastersQueue.Remove(Caster);
	}
}

void UTokenComponent::ReceiveToken(FToken* Token)
{
	Token->Owner == this ? Tokens.Add(Token) : AviableTokens.Add(Token);
	if (CastersQueue.Num() > 0) GiveToken(CastersQueue.CreateIterator()->Key,1);
}

