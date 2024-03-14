// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "Components/CombatComponent.h"
//
// #include "Characters/BaseCharacter.h"
// #include "Combat/AxeLightAttack.h"
// #include "GameFramework/CharacterMovementComponent.h"
// #include "GameFramework/Character.h"
//
// // Sets default values for this component's properties
// UCombatComponent::UCombatComponent(){
// 	CombatState = ECombatState::Idle;
// 	ComboCount = 0;
// 	CurrentWeapon = 0;
// }
//
// // Called when the game starts
// void UCombatComponent::BeginPlay(){
// 	Super::BeginPlay();
// 	HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
// 	StaminaComponent = GetOwner()->FindComponentByClass<UStaminaComponent>();
// }
//
// UCombatAbility* UCombatComponent::SetAbility(TSubclassOf<UCombatAbility> AbilityClass){
// 	UCombatAbility* Ability = NewObject<UCombatAbility>(this, AbilityClass);
// 	Ability->Initialize(Cast<ABaseCharacter>(GetOwner()));
// 	return Ability;
// }
//
// void UCombatComponent::SetWeaponAbilities(){
// 	if (WeaponDataInventory[CurrentWeapon].LightAttack && WeaponDataInventory[CurrentWeapon].HeavyAttack){
// 		CurrentAbilityKit.LightAttack = SetAbility(WeaponDataInventory[CurrentWeapon].LightAttack);
// 		CurrentAbilityKit.HeavyAttack = SetAbility(WeaponDataInventory[CurrentWeapon].HeavyAttack);
// 	}
// 	else{
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LightAttackClass or HeavyAttackClass not set"));
// 		CurrentAbilityKit.LightAttack = nullptr;
// 		CurrentAbilityKit.HeavyAttack = nullptr;
// 	}
// }
//
// void UCombatComponent::SetCurrentWeapon(){
// 	//CurrentWeaponActor->SetWeaponData(WeaponDataInventory[CurrentWeapon], Cast<ABaseCharacter>(GetOwner()));
// }
//
//
// void UCombatComponent::AddWeaponToInventory(FDataTableRowHandle Weapon){
// 	FItemData ItemData = *Weapon.GetRow<FItemData>(FString::Printf(TEXT("%s"), *Weapon.RowName.ToString()));
// 	WeaponDataInventory.Add(ItemData);
// 	AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>();
// 	NewWeapon->SetWeaponData(ItemData, Cast<ABaseCharacter>(GetOwner()));
// 	for (AWeapon* Weapon : WeaponInventory){
// 		Weapon->WeaponActor->SetActorHiddenInGame(true);
// 	}
// 	WeaponInventory.Add(NewWeapon);
// }
//
// void UCombatComponent::NextWeapon(){
// 	CurrentWeapon = (CurrentWeapon + 1) % WeaponDataInventory.Num();
//
// }
//
// void UCombatComponent::PreviousWeapon(){
// 	CurrentWeapon = (CurrentWeapon - 1 + WeaponDataInventory.Num()) % WeaponDataInventory.Num();
// 	SetCurrentWeapon();
// 	SetWeaponAbilities();
// }
//
// void UCombatComponent::ExecuteCurrentWeaponDodgeMontage(){
// 	UAnimMontage* CurrentWeaponMontage = WeaponDataInventory[CurrentWeapon].DodgeMontage;
//
// 	if (CurrentWeaponMontage){
// 		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
//
// 		if (OwnerCharacter){
// 			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
//
// 			if (AnimInstance){
// 				if (AnimInstance->Montage_IsPlaying(WeaponDataInventory[CurrentWeapon].DodgeMontage)){
// 					AnimInstance->Montage_Stop(0.25f, CurrentWeaponMontage);
// 				}
//
// 				AnimInstance->Montage_Play(CurrentWeaponMontage, 1.5);
// 			}
// 		}
// 	}
// }
//
// void UCombatComponent::ExecuteCurrentWeaponBlockMontage(){
// 	UAnimMontage* CurrentWeaponMontage = WeaponDataInventory[CurrentWeapon].BlockMontage;
//
// 	if (CurrentWeaponMontage){
// 		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
//
// 		if (OwnerCharacter){
// 			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
//
// 			if (AnimInstance){
// 				if (AnimInstance->Montage_IsPlaying(WeaponDataInventory[CurrentWeapon].BlockMontage)){
// 					AnimInstance->Montage_Stop(0.25f, CurrentWeaponMontage);
// 				}
//
// 				AnimInstance->Montage_Play(CurrentWeaponMontage, 1.5);
// 			}
// 		}
// 	}
// }
//
// void UCombatComponent::Dodge(){
// 	if (CombatState == ECombatState::Idle){
// 		CombatState = ECombatState::Dodging;
// 		ExecuteCurrentWeaponDodgeMontage();
// 	}
// }
//
// void UCombatComponent::Block(){
// 	if (CombatState == ECombatState::Idle){
// 		CombatState = ECombatState::Blocking;
// 		ExecuteCurrentWeaponBlockMontage();
// 	}
// }
//
// void UCombatComponent::Attack(UCombatAbility* Ability, ECombatState QueuedState){
// 	ChangeWeaponLight(1);
// 	if (CombatState == ECombatState::Idle && Ability){
// 		CombatState = ECombatState::Attacking;
// 		Ability->ExecuteSection(NAME_None);
// 		IncreaseComboCount();
// 	}
// 	else if (CombatState == ECombatState::QueuingAttack){
// 		CombatState = QueuedState;
// 		ChangeWeaponLight(1);
// 		ChangeWeaponLightColor(FLinearColor(0, 1, 0, 1));
// 	}
// 	else{
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid Combat State or Attack not set"));
// 	}
// }
//
// void UCombatComponent::LightAttack(){
// 	//Attack(CurrentWeaponActor->LightAttack, ECombatState::LightAttackQueued);
// }
//
// void UCombatComponent::HeavyAttack(){
// 	//Attack(CurrentWeaponActor->HeavyAttack, ECombatState::HeavyAttackQueued);
// }
//
// void UCombatComponent::EndAttack(){
// 	// Cambia el estado de combate a Idle
// 	CombatState = ECombatState::Idle;
//
// 	// Obtén una referencia al personaje y reactiva su capacidad de moverse
// 	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
// 	if (OwnerCharacter){
// 		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 800;
// 	}
// }
//
// void UCombatComponent::QueueAttack(FName SectionName){
// 	if (CombatState == ECombatState::LightAttackQueued){
// 		CombatState = ECombatState::Attacking;
// 		CurrentAbilityKit.LightAttack->ExecuteSection(SectionName);
// 	}
// 	else if (CombatState == ECombatState::HeavyAttackQueued){
// 		CombatState = ECombatState::Attacking;
// 		CurrentAbilityKit.HeavyAttack->ExecuteSection(SectionName);
// 	}
// 	else{
// 		ComboCount = 0;
// 		CombatState = ECombatState::Idle;
// 	}
// }
//
// //TODO break block and stun character if stamina is 0
// //TODO break into different functions
//
// void UCombatComponent::TakeDamage(float Damage, UObject* DamageType){
// 	switch (CombatState){
// 	case ECombatState::Blocking:
// 		StaminaComponent->DecreaseStamina(Damage);
// 		break;
// 	case ECombatState::Dodging:
// 		break;
// 	case ECombatState::Parrying:
// 		break;
// 	default:
// 		HealthComponent->TakeDamageWithType(DamageType, Damage);
//
// 		break;
// 	}
// }
//
//
// void UCombatComponent::IncreaseComboCount(){
// 	ComboCount = (ComboCount + 1) % WeaponDataInventory[CurrentWeapon].MaxComboCount;
// }
//
// void UCombatComponent::ChangeWeaponLight(float Intensity){
// 	// if (CurrentWeaponActor->WeaponActor->WeaponMesh){
// 	// 	UMaterialInstanceDynamic* OwnerMaterial = CurrentWeaponActor->WeaponActor->WeaponMesh->
// 	// 	                                                              CreateAndSetMaterialInstanceDynamic(0);
// 	// 	if (OwnerMaterial){
// 	// 		OwnerMaterial->SetScalarParameterValue("EmissiveIntensity", Intensity);
// 	// 	}
// 	// }
// }
//
// void UCombatComponent::ChangeWeaponLightColor(FLinearColor Color){
// 	// if (CurrentWeaponActor->WeaponActor->WeaponMesh){
// 	// 	UMaterialInstanceDynamic* OwnerMaterial = CurrentWeaponActor->WeaponActor->WeaponMesh->
// 	// 	                                                              CreateAndSetMaterialInstanceDynamic(0);
// 	// 	if (OwnerMaterial){
// 	// 		OwnerMaterial->SetVectorParameterValue("EmissiveColor", Color);
// 	// 	}
// 	// }
// }
