#include "Components/TokenComponent.h"

// Default constructor for the UTokenComponent class.
// Initializes the component and creates a set of tokens.
UTokenComponent::UTokenComponent()
{
	// Enable ticking for this component.
	PrimaryComponentTick.bCanEverTick = true;

	// Get a unique ID for the starting token.
	const uint32 StartID = GetUniqueID();

	// Create a set of tokens and add them to the Tokens array.
	for (int i = 0; i < MaxTokens; i++)
	{
		Tokens.Add(new FToken(StartID + i + 1, this));
	}
}

// Method to reserve a certain quantity of tokens for a specific caster.
// If the requested quantity is more than the available tokens, the caster is added to the queue.
void UTokenComponent::ReserveTokens(UTokenComponent* Caster, const int32 Quantity)
{
	Quantity > Tokens.Num() ? AddToQueue(Caster, Quantity) : GiveToken(Caster, Quantity);
}

// Method to return the available tokens from a specific owner.
void UTokenComponent::ReturnTokens(UTokenComponent* Owner)
{
	RemoveTokens(Owner, AviableTokens);
}

// Method to return all available tokens.
void UTokenComponent::ReturnAllTokens()
{
	RemoveTokens(nullptr, AviableTokens);
}

// Method to remove tokens from a specific owner or all owners if no owner is specified.
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

// Method to add a caster to the queue with a specific quantity of tokens to be reserved.
void UTokenComponent::AddToQueue(UTokenComponent* Caster, const int32 Quantity)
{
	CastersQueue.Contains(Caster) ? CastersQueue[Caster] += Quantity : CastersQueue.Add(Caster, Quantity);
}

// Method to give a specific quantity of tokens to a caster.
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

// Method to receive a token.
// If the token's owner is this component, the token is added to the Tokens array.
// Otherwise, it is added to the AvailableTokens array.
// If there are casters in the queue, it gives a token to the first caster in the queue.
void UTokenComponent::ReceiveToken(FToken* Token)
{
	Token->Owner == this ? Tokens.Add(Token) : AviableTokens.Add(Token);
	if (CastersQueue.Num() > 0) GiveToken(CastersQueue.CreateIterator()->Key, 1);
}

// Method to get the available tokens for a specific target.
int32 UTokenComponent::GetAviablleTokens(UTokenComponent* Target) const
{
	return (Target == this) ? Tokens.Num() :
	AviableTokens.FilterByPredicate([Target](const FToken* Token){ return Token->Owner == Target; }).Num();
}
//// ONLY FOR DEBUGGING PURPOSES ////
// Method to print information about all tokens.
void UTokenComponent::PrintTokenInfo()
{
	for (const FToken* Token : Tokens)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
		                                 FString::Printf(
			                                 TEXT("%d, Owner: %s"), Token->UniqueID, *Token->Owner->GetName()));
	}

	for (const FToken* Token : AviableTokens)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		                                 FString::Printf(
			                                 TEXT("%d, Owner: %s"), Token->UniqueID, *Token->Owner->GetName()));
	}
}

//// UNUSED METHODS ////
void UTokenComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTokenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
