#pragma once
#include "CoreMinimal.h"
#include "ReactionHandler.generated.h"


UCLASS()
class SIGURD_API UReactionHandler : public UObject
{
	GENERATED_BODY()
public:
	virtual void Burn(){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Burn"));
	}

	virtual void Wet(){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Wet"));
	}

	virtual void Petrify(){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Petrify"));
	}
	
};
