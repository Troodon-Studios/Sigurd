// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTypes/Elements.h"
#include "Interfaces/ElementInterface.h"
#include "ElementComponent.generated.h"





UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIGURD_API UElementComponent : public UActorComponent, public IElementInterface{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UElementComponent();

	TMap<TPair<Element , Element>, Reaction> ReactionMap = {
		{TPair<Element, Element>(Element::Water, Element::Fire), Reaction::Evaporate},
		{TPair<Element, Element>(Element::Fire, Element::Water), Reaction::Evaporate},
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element")
	TArray<Element> ElementList;

	UFUNCTION(BlueprintCallable, Category = "Element")
	virtual void AddElement_Implementation(Element NewElement) override;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
