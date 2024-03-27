// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "HealthComponent.h"
// #include "ItemData.h"
// #include "DataTypes/CombatState.h"
// #include "StaminaComponent.h"
// #include "Combat/CombatAbility.h"
// #include "Combat/Weapon.h"
// #include "Components/ActorComponent.h"
// #include "CombatComponent.generated.h"
//
//
// struct AbilityKit{
//
// 	UCombatAbility* LightAttack;
// 	UCombatAbility* HeavyAttack;
// 	UCombatAbility* Dodge;
// 	UCombatAbility* Block;
// 	
// 	UCombatAbility* Ability1;
// 	UCombatAbility* Ability2;
// 	UCombatAbility* Ability3;
// 	UCombatAbility* Ability4;
// 	
// };
//
// UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
// class SIGURD_API UCombatComponent : public UActorComponent
// {
// 	GENERATED_BODY()
//
// private:
//
//
// 	int ComboCount = 0;
//
// 	AbilityKit CurrentAbilityKit;
//
// 	void Attack(UCombatAbility* Ability, ECombatState QueuedState);
//
// public:	
// 	// Sets default values for this component's properties
// 	UCombatComponent();
//
// 	TEnumAsByte<ECombatState> CombatState;
//
// 	int CurrentWeapon = -1;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
// 	TArray<AWeapon*> WeaponInventory;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
// 	TArray<FItemData> WeaponDataInventory;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
// 	TArray<TSubclassOf<UCombatAbility>> AbilityInventory; 
// 	
// 	UFUNCTION(BlueprintCallable, Category = "Weapon")
// 	void AddWeaponToInventory(FDataTableRowHandle Weapon);
//
// 	UFUNCTION(BlueprintCallable, Category = "Weapon")
// 	void NextWeapon();
//
// 	UFUNCTION(BlueprintCallable, Category = "Weapon")
// 	void PreviousWeapon();
// 	
// 	void ExecuteCurrentWeaponDodgeMontage();
// 	void ExecuteCurrentWeaponBlockMontage();
// 	
// 	UStaminaComponent* StaminaComponent;
// 	UHealthComponent* HealthComponent;
//
// 	UFUNCTION(BlueprintCallable, Category = "Combat")
// 	void Dodge();
// 	UFUNCTION(BlueprintCallable, Category = "Combat")
// 	void Block();
// 	
// 	void LightAttack();
// 	void HeavyAttack();
// 	
// 	void EndAttack();
//
// 	void QueueAttack(FName SectionName);
//
// 	UFUNCTION(BlueprintCallable, Category = "Combat")
// 	void TakeDamage(float Damage, UObject* DamageType);
//
// 	void IncreaseComboCount();
//
// 	void ChangeWeaponLight(float Intensity);
// 	void ChangeWeaponLightColor(FLinearColor Color);
//
// protected:
// 	// Called when the game starts
// 	virtual void BeginPlay() override;
//
// 	UCombatAbility* SetAbility(TSubclassOf<UCombatAbility> AbilityClass);
//
// 	void SetWeaponAbilities();
//
// 	void SetCurrentWeapon();
// 	
// };
