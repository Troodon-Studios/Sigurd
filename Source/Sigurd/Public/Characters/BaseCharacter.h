#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/CombatComponent.h"
#include "Components/ConditionComponent.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/TokenComponent.h"

#include "BaseCharacter.generated.h"

//Base class for all characters in the game so that we can get the components we need
UCLASS()
class SIGURD_API ABaseCharacter : public ACharacter {
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	UStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Condition", meta = (AllowPrivateAccess = "true"))
	UConditionComponent* ConditionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UTokenComponent* TokenComponent;

protected:
	virtual void BeginPlay() override;

public:
	ABaseCharacter();

	UStaminaComponent* GetStaminaComponent() const;
	UCombatComponent* GetCombatComponent() const;
	UHealthComponent* GetHealthComponent() const;
	UConditionComponent* GetConditionComponent() const;
	UTokenComponent* GetTokenComponent() const;
};
